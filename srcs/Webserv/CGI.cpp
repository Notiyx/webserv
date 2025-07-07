/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:10:18 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/07 08:26:45 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <CGI.hpp>

CGI::CGI(const std::string& method,
		const std::string& path,
		const std::string& body,
		const std::string& path_info,
		const std::string& query_string,
		const Config& conf,
		const std::map<std::string, std::string>& header)
		: method(method), path(path), body(body), path_info(path_info), query_string(query_string), conf(conf), header(header)
		{
			buildEnv();
		};
		
CGI::~CGI() {};

void CGI::buildEnv() {
	env.push_back("REQUEST_METHOD=" + method);
	env.push_back("SCRIPT_FILENAME=" + path);
	env.push_back("SERVER_NAME=" + conf.getServName());
	std::ostringstream oss;
	oss << conf.getPort();
	env.push_back("SERVER_PORT=" + oss.str());
	env.push_back("PATH_INFO=" + path_info);
	env.push_back("QUERY_STRING=" + query_string);
	env.push_back("SCRIPT_NAME=" + path);
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	for(std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it)
	{
		if (it->first == "Content-Length")
			continue ;
		else
		{
			std::string key = it->first;
			for (std::string::iterator c = key.begin(); c != key.end(); ++c)
			{
				if(*c == '-')
					*c = '_';
				else
   					*c = std::toupper(static_cast<unsigned char>(*c));
			}
			if (it->first == "Content-Type")
				env.push_back(key + "=" + it->second);
			else
				env.push_back("HTTP_" + key + "=" + it->second);
		}
	}
	if (method == "POST")
	{
		std::ostringstream oss;
		oss << body.size();
		env.push_back("CONTENT_LENGTH=" + oss.str());
	}
};

void CGI::execCGI() {
	int fd[2];
	pid_t pid;
	if (pipe(fd) == -1)
		throw(std::runtime_error("Internal server Error"));
	pid = fork();
	if (pid == -1)
		throw(std::runtime_error("Internal server Error"));
	if (pid == 0)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit(1);
		close(fd[1]);
		std::vector<char*> envp;
		for(size_t i = 0; i < env.size(); ++i)
			envp.push_back(const_cast<char*>(env[i].c_str()));
		envp.push_back(NULL);
		std::string p = path.substr(1);
		char *arv[] = {
			const_cast<char*>("/usr/bin/python3"),
			const_cast<char*>(p.c_str()),
			NULL
		};
		execve(arv[0], arv, envp.data());
		exit(1);
	} 
	else {
		close(fd[1]);
		std::string cgi_res;
		char buffer[1000];
		size_t bytes_read;
		
		while((bytes_read = read(fd[0], buffer, sizeof(buffer))))
			cgi_res.append(buffer, bytes_read);
		if (bytes_read == static_cast<size_t>(-1))
			throw std::runtime_error("Internal Server Error");
		close(fd[0]);
		int status;
		waitpid(pid, &status, 0);
		if(!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			throw std::runtime_error("Internal Server Error");

		std::cout << "CGI RES: \n" << cgi_res << std::endl;
	}
};
