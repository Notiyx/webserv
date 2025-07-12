/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:10:18 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/12 19:19:31 by tlonghin         ###   ########.fr       */
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

volatile sig_atomic_t g_timeout = 0;

void sigalrm_handler(int) {
    g_timeout = 1;
}

void CGI::childPostCGI(int pipe_post[2]) {
		close(pipe_post[0]);
		write(pipe_post[1], body.data(), body.size());
		close(pipe_post[1]);
		std::exit(0);
};


void CGI::childCGI(int fd[2]) {
	int pipe_post[2];
	if (method == "POST")
	{
		if (pipe(pipe_post) == -1)
			std::exit(1);
		pid_t pid = fork();
		if (pid == -1)
			std::exit(1);
		if (pid == 0)
			childPostCGI(pipe_post);
		if (dup2(pipe_post[0], STDIN_FILENO) == -1)
			std::exit(1);
		close(pipe_post[0]);
		close(pipe_post[1]);
	}
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		std::exit(1);
	close(fd[0]);
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
	std::exit(1);
};

std::string CGI::dadCGI(int fd[2], pid_t pid, int *ActivCGI) {
		close(fd[1]);
		std::string cgi_res;
		char buffer[1000];
		ssize_t bytes_read;
		g_timeout = 0;
		signal(SIGALRM, sigalrm_handler);
		int status;
		waitpid(pid, &status, 0);
		alarm(5);
		ssize_t total = 0;
		while((bytes_read = read(fd[0], buffer, sizeof(buffer))) > 0)
		{
			total += bytes_read;
			if (total > 1000000)
			{
				kill(pid, SIGKILL);
				close(fd[0]);
				(*ActivCGI)--;
				throw std::runtime_error("Internal Server Error");
			}
			cgi_res.append(buffer, bytes_read);
		}
		alarm(0);
		close(fd[0]);
		(*ActivCGI)--;
		if (g_timeout) {
			kill(pid, SIGKILL);
			throw std::runtime_error("Internal Server Error");
		}
		if(!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			throw std::runtime_error("Internal Server Error");
		return (cgi_res);
};

std::string  CGI::execCGI() {
	int fd[2];
	std::string body;
	if (pipe(fd) == -1)
		throw(std::runtime_error("Internal server Error"));
	static int ActivCGI = 0;
	ActivCGI++;
	if (ActivCGI >= 10)
	{
		ActivCGI--;
		throw(std::runtime_error("Internal server Error"));
	}
	pid_t pid = fork();
	if (pid == -1)
	{
		ActivCGI--;
		throw(std::runtime_error("Internal server Error"));
	}
	if (pid == 0)
	{
		try {
			childCGI(fd);
		} catch (std::runtime_error& e) {throw(std::runtime_error("Internal server Error"));}
	}
	else {
		try {
			body = dadCGI(fd, pid, &ActivCGI);
		} catch (std::runtime_error& e) {throw(std::runtime_error("Internal server Error"));}
	}
	return (body);
};
