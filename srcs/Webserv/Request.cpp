/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 02:03:30 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/05 14:37:56 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Request.hpp>

Request::Request(std::string request, Config conf, int client_fd) : conf(conf), client_fd(client_fd) {
	parse(request);
};

Request::~Request() {};

void Request::parse(std::string request) {
		std::istringstream iss(request);
		std::string meth;
		std::string line;
		iss >> meth >> path;
		if (meth == "GET")
			method = GET;
		else if (meth == "POST")
			method = POST;
		else if (meth == "DELETE")
			method = DELETE;
		else
			method = OTHER;
		std::string key;
		std::string content;
		std::getline(iss, line);
		while(std::getline(iss, line) && line != "\r" && !line.empty())
		{
			if (line.find(':') == std::string::npos)
			{
				HTTPResponse error(400, "Bad Request");
				try {
					error.send(client_fd);
				} catch (const fdError &e) {
					std::cerr << e.what() << std::endl;
				}
			}
			std::istringstream issl(line);
			std::getline(issl, key, ':');
			std::getline(issl, content);
			key = utils::trim(key);
			content = utils::trim(content);
			content.erase(0, content.find_first_not_of(" "));
			header.insert(std::make_pair(key, content));
		}
		std::ostringstream oss;
		oss << iss.rdbuf();
		body = oss.str();
};

void Request::sendError(int code, std::string msg) {
	HTTPResponse error(code, msg);
	try {
		error.send(client_fd);
	} catch (const fdError &e) {
		std::cerr << e.what() << std::endl;
		return ;
	}
};

void Request::parseHeader() {
		bool ishost = false;
		for(std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it)
		{
			if ((method == GET || method == DELETE) && !body.empty())
			{
				sendError(400, "Bad Request");
				return ;
			}
			if (it->first == "Host")
			{
				ishost = true;
				if (it->second != conf.getHostAndPort())
				{
					std::cout << "|" << it->second << "|" << std::endl;
					std::cout << "|" << conf.getHostAndPort() << "|" << std::endl;
					sendError(400, "Bad Request");
					return ;
				}
			}
			if (method == POST && it->first == "Content-Length")
			{
				size_t size = std::atoi(it->second.c_str());
				if (body.length() != size)
				{
					sendError(400, "Bad Request");
					return ;
				}
			}
			if (method == POST && it->first == "Content-Type")
			{
				if (it->second != "application/x-www-form-urlencoded"
					&& it->second != "multipart/form-data")
				{
					sendError(400, "Bad Request");
					return ;
				}
			}
		}
		if (!ishost)
		{
			sendError(400, "Bad Request");
			return ;
		}
};

void Request::execute() {
	if (method == GET)
	{
		std::map<std::string, IS_Location>::iterator it = conf.getLocation(path);
		std::string filename = it->second.getIndex();
		HTTPResponse valid;
		std::string res = valid.buildGet(filename);
		utils::sender(client_fd, res);
		close(client_fd);
	} else if (method == POST) {
		std::cout << "POST" << std::endl;
	} else if (method == DELETE) {
		std::cout << "DELETE" << std::endl;
	}else {
		sendError(405, "Method Not Allowed");
		return ;
	}
};
