/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_poll.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:28:19 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/09 03:58:54 by nmetais          ###   ########.fr       */
/*   Updated: 2025/07/08 08:17:30 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <E_poll.hpp>
#include <NameSpace.hpp>
#include <HTTPResponse.hpp>
#include <Request.hpp>

E_poll::E_poll(Config conf) : epoll_fd(-1), conf(conf){};

E_poll::~E_poll() {};

void E_poll::sendError(int client_fd, int code, const std::string &msg) {
    HTTPResponse error(code, msg, this->conf);
    try {
        error.send(client_fd);
    } catch (const fdError &e) {
        std::cerr << e.what() << std::endl;
    }
}

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

bool	E_poll::readClient(int client_fd, IS_Client &client) {
	char buffer[4096];
	int  bytes = read(client_fd, buffer, sizeof(buffer));
	if (bytes <= 0)
		return (false);
	client.appendBuffer(buffer, bytes);
	//Client valid headers + Content-Length recup + Chunked request detect
	if (!client.getHeader())
	{
		size_t pos = client.getBuffer().find("\r\n\r\n");
		if (pos != std::string::npos)
		{
			client.setHeader(true);
			std::string headers = client.getBuffer().substr(0, pos + 4);
			size_t cont_pos = headers.find("Transfer-Encoding:");
			if (cont_pos != std::string::npos)
			{
				cont_pos = headers.find("chunked", cont_pos);
				if (cont_pos != std::string::npos)
					client.setChunk(true);
			}
			cont_pos = headers.find("Content-Length:");
			if (cont_pos != std::string::npos)
			{
				size_t start = headers.find_first_of("0123456789", cont_pos);
				size_t end = headers.find("\r\n", start);
				int len = std::atoi(headers.substr(start, end -start).c_str());
				client.setLength(len);
			}
			else
				client.setLength(0);
		}
	}
	//Body type detect + end read detect
	if (client.getHeader()) {
		size_t body_start = client.getBuffer().find("\r\n\r\n") + 4;
		if (client.getChunk())
		{
			if (client.getBuffer().find("0\r\n\r\n", body_start) != std::string::npos)
				return (true);
		}
		else if (client.getBuffer().size() >= body_start + client.getLength())
			return (true);
	}
	return (false);
};

bool E_poll::isValidRequest(int client_fd, IS_Client &client) {
	std::istringstream iss(client.getBuffer());
	std::string method, path, version;
	iss >> method >> path >> version;
	if (method.empty() || path.empty() || version.empty() || version != "HTTP/1.1") {
		sendError(client_fd, 401, "Bad Request");
		return (false);
	}
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		sendError(client_fd, 405, "Method Not Allowed");
		return (false);
	}
	std::string line;
	std::string empty;
	bool host = false;
	bool length = false;
	std::getline(iss, empty);
	while(std::getline(iss, line) && line != "\r")
	{
		if (line.find("Host:") != std::string::npos)
			host = true;
		if (line.find("Content-Length:") != std::string::npos)
			length = true;
	}
	if (!host || (length && client.getChunk()))
	{
		sendError(client_fd, 402, "Bad Request");
		return (false);
	}
	std::map<std::string, IS_Location>::iterator  location = conf.getBestLocation(path);
	if (location == conf.locationEnd())
	{
		sendError(client_fd, 404, "Not Found");
		return (false);
	}
	if ((method == "GET" && !location->second.getLocationGetMethod()) ||
		(method == "POST" && !location->second.getLocationPostMethod()) ||
		(method == "DELETE" && !location->second.getLocationDeleteMethod()))
	{
		sendError(client_fd, 405, "Method Not Allowed");
		return (false);
	}
	return (true);
};

void E_poll::launchRequest(int client_fd, IS_Client &client) {
	Request req(client, conf, client_fd);
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
				IS_Client &client = client_map[fd];
				if (!readClient(fd, client))
				{
					close(fd);
					return ;
				}
				
				if(isValidRequest(fd, client))
					launchRequest(fd, client);
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
