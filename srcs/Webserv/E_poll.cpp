/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_poll.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:28:19 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/08 08:17:30 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <E_poll.hpp>
#include <NameSpace.hpp>
#include <HTTPResponse.hpp>
#include <Request.hpp>

E_poll::E_poll(Config conf) : epoll_fd(-1), conf(conf){};

E_poll::~E_poll() {};

void E_poll::epollInit(int serv_fd) {
	struct epoll_event event;
	epoll_fd = epoll_create1(0);
	utils::check_syscall(epoll_fd, "epoll create");
	//event en lecture en mode attente de conexion
	event.events = EPOLLIN;
	event.data.fd = serv_fd;
	//trop la flemme les error custom la jpp
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_fd, &event) == -1)
		throw std::runtime_error("Error: epoll_ctl");
};



bool E_poll::isValidRequest(int client_fd, std::string &request) {
	std::string req;
	char* buffer;
	try {
	    buffer = new char[50000000];
	}
	catch (const std::bad_alloc& e) {
	    std::cerr << "Erreur d'allocation mémoire : " << e.what() << std::endl;
		return (false);
	}
	int	 readed = 0;
	int  bytes;
	bool contentLength = false;
	int  length = 0;
	while (request.find("\r\n\r\n") == std::string::npos)
	{
		bytes = read(client_fd, buffer, sizeof(buffer));
		if (bytes <= 0)
		{
			throw std::runtime_error("Error : read failed");
			delete [] buffer;
			return (false);
		}
		request.append(buffer, bytes);
		readed += bytes;
		if (readed > 50000000) {
			HTTPResponse error(413, "Payload Too Large", this->conf);
			try {
				error.send(client_fd);
			} catch (const fdError &e) {
				std::cerr << e.what() << std::endl;
			}
			delete [] buffer;
			return (false);
		}
	}
	size_t end_header = request.find("\r\n\r\n");
	std::string headers = request.substr(0, end_header);
	bool isChunked = false;
	if (headers.find("Transfer-Encoding: chunked") != std::string::npos)
		isChunked = true;
	size_t pos = headers.find("Content-Length");
	if (pos != std::string::npos)
	{
		size_t start = headers.find_first_not_of(" ", pos + 15);
		size_t end = headers.find("\r\n", start);
		std::string length_str = headers.substr(start, end - start);
		length = std::atoi(length_str.c_str());
		contentLength = true;
	}
if (isChunked)
{
	while (request.find("0\r\n\r\n", end_header) == std::string::npos)
	{
		bytes = read(client_fd, buffer, sizeof(buffer));
		if (bytes <= 0)
		{
			throw std::runtime_error("Error: read failed on chunked body");
			delete [] buffer;
			return (false);
		}
		request.append(buffer, bytes);
		if (request.size() > 50000000)
		{
			HTTPResponse error(413, "Payload Too Large", this->conf);
			try {
				error.send(client_fd);
			} catch (const fdError &e) {
				std::cerr << e.what() << std::endl;
			}
			delete [] buffer;
			return (false);
		}
	}
	} else if (contentLength) {
		size_t total = end_header + 4 + length;
		while (request.size() < total) 
		{
			bytes = read(client_fd, buffer, sizeof(buffer));
			if (bytes <= 0)
			{
				throw std::runtime_error("Error: read failed on body");
				delete [] buffer;
				return (false);
			}
			request.append(buffer, bytes);
			if (request.size() > 50000000)
			{
				HTTPResponse error(413, "Payload Too Large", this->conf);
				try {
					error.send(client_fd);
				} catch (const fdError &e) {
					std::cerr << e.what() << std::endl;
				}
				delete [] buffer;
				return (false);
			}
		}
	}
	std::istringstream iss(request);
	std::string method, path, version;
	iss >> method >> path >> version;
	if (method.empty() || path.empty() || version.empty() || version != "HTTP/1.1") {
		HTTPResponse error(400, "Bad Request", this->conf);
		try {
			error.send(client_fd);
		} catch (const fdError &e) {
			std::cerr << e.what() << std::endl;
		}
		delete [] buffer;
		return (false);
	}
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		HTTPResponse error(405, "Method Not Allowed", this->conf);
		try {
			error.send(client_fd);
		} catch (const fdError &e) {
			std::cerr << e.what() << std::endl;
		}
		delete [] buffer;
		return (false);
	}
	std::map<std::string, IS_Location>::iterator  location = conf.getBestLocation(path);
	if (location == conf.locationEnd())
	{
		HTTPResponse error(404, "Not Found", this->conf);
		try {
			error.send(client_fd);
		} catch (const fdError &e) {
			std::cerr << e.what() << std::endl;
		}
		delete [] buffer;
		return (false);
	}
	if ((method == "GET" && !location->second.getLocationGetMethod()) ||
		(method == "POST" && !location->second.getLocationPostMethod()) ||
		(method == "DELETE" && !location->second.getLocationDeleteMethod()))
	{
		HTTPResponse error(405, "Method not Allowed", this->conf);
		try {
			error.send(client_fd);
		} catch (const fdError &e) {
			std::cerr << e.what() << std::endl;
		}
		delete [] buffer;
		return (false);
	}
	delete [] buffer;
	return (true);
};

void E_poll::LaunchRequest(int client_fd, std::string& request) {
	if (request.empty())
		return ;
	Request req(request, conf, client_fd);
	req.parseHeader();
	if (req.getMethod() == "POST" && !req.getChunk())
	{
		if(!req.parseBody())
		{
			close(client_fd);
			return ;
		}
	}
	if (req.getChunk())
	{
		req.unChunk();
	}
	req.execute();
};


void E_poll::epollExec(int serv_fd) {
	struct epoll_event events[1024];
	//trigger = le nombre de clients qui ont besoin de trigger un event
	int trigger = epoll_wait(epoll_fd, events, 1024, -1);
	for (int i = 0; i < trigger; i++)
	{
		int fd = events[i].data.fd;
		if (fd == serv_fd)
		{
			struct sockaddr_in client_addr;
       		socklen_t addr_len = sizeof(client_addr);
			int client_fd = accept(serv_fd, (struct sockaddr*)&client_addr, &addr_len);
			utils::check_syscall(client_fd, "accept");
			struct epoll_event req;
			req.events = EPOLLIN;
			req.data.fd = client_fd;
			utils::check_syscall(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &req), "epoll_ctl1");
		}
		else if (events[i].events & EPOLLIN)
		{
			try {
				std::string request;
				bool valid = isValidRequest(fd, request);
				if(valid)
				{
					LaunchRequest(fd, request);
				}
				else
					close(fd);
			} catch (const std::runtime_error& e) {
				std::cerr << e.what() << std::endl;
				close(fd);
				return ;
			}
		}
		else if (events[i].events & EPOLLOUT)
		{
			struct epoll_event req;
			req.events = EPOLLIN;
			req.data.fd = fd;
			epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &req);
		}
	}
};
