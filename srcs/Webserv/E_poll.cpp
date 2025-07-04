/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_poll.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:28:19 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/04 19:23:23 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <E_poll.hpp>
#include <NameSpace.hpp>

E_poll::E_poll() : clients() , epoll_fd(-1) {};

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

void E_poll::epollLaunch(int maxClients, int serv_fd) {
	struct epoll_event events[maxClients];
	//n = le nombre de clients qui ont besoin de trigger un event
	int trigger = epoll_wait(epoll_fd, events, maxClients, -1);
	for (int i = 0; i < trigger; i++)
	{
		if (events[i].data.fd == serv_fd)
		{
			struct sockaddr_in client_addr;
       		socklen_t addr_len = sizeof(client_addr);
			int client_fd = accept(serv_fd, (struct sockaddr*)&client_addr, &addr_len);
			utils::check_syscall(client_fd, "accept");
			struct epoll_event event;
			event.events = EPOLLIN;
			event.data.fd = client_fd;
			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
				throw std::runtime_error("Error: epoll_ctl");
			clients.push_back(client_fd);
		}
		//je verifie si il y a EPPOLIN dans les flags d'events
		if (events[i].events & EPOLLIN)
		{
			
		}
	}
};
