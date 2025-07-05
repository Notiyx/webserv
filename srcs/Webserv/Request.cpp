/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 02:03:30 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/05 18:25:10 by nmetais          ###   ########.fr       */
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
				sendError(400, "Bad request");
				return ;
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

std::string Request::getContentType() {
	for(std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it)
	{
		if (it->first == "Content-Type")
			return (std::string(it->second));
	}
	return (NULL);
};

bool Request::executePost(std::string filename) {
	std::ofstream file(filename.c_str(), std::ios::out | std::ios::app);
	if (!file.is_open()) {
		sendError(500, "Internal Server Error");
		close(client_fd);
		return (false);
	}
	std::istringstream iss(body);
	std::string data;
	while(std::getline(iss, data, '&'))
		file << data << "\n";
	file.close();
	return (true);
};


void Request::execute() {
	HTTPResponse valid;
	std::string res;
	std::map<std::string, IS_Location>::iterator it = conf.getLocation(path);
	std::string root = it->second.getRoot();
	std::string index = it->second.getIndex();
	std::string fullpath = root + "/" + index;
	if (method == GET)
	{
		std::cout << "GET" << std::endl;
		res = valid.buildGet(fullpath);
	} else if (method == POST) {
		if (getContentType() == "application/x-www-form-urlencoded")
		{
			std::cout << "POST" << std::endl;
			res = valid.buildPost();
			if (!executePost(fullpath))
				return ;
		}
	} else if (method == DELETE) {
		std::cout << "DELETE" << std::endl;
	}else {
		sendError(405, "Method Not Allowed");
		return ;
	}
	utils::sender(client_fd, res);
	close(client_fd);
};
