/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_poll.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:28:19 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/04 20:47:02 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <E_poll.hpp>
#include <NameSpace.hpp>

E_poll::E_poll() : epoll_fd(-1) {};

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

void E_poll::epollLaunch(int serv_fd) {
	struct epoll_event events[1024];
	//n = le nombre de clients qui ont besoin de trigger un event
	int trigger = epoll_wait(epoll_fd, events, 1024, -1);
	for (int i = 0; i < trigger; i++)
	{
		struct epoll_event event;
		if (events[i].data.fd == serv_fd)
		{
			struct sockaddr_in client_addr;
       		socklen_t addr_len = sizeof(client_addr);
			int client_fd = accept(serv_fd, (struct sockaddr*)&client_addr, &addr_len);
			utils::check_syscall(client_fd, "accept");
			event.events = EPOLLIN;
			event.data.fd = client_fd;
			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
				throw std::runtime_error("Error: epoll_ctl");
		}
		//je verifie si il y a EPPOLIN dans les flags d'events
		else if (events[i].events & EPOLLIN)
		{
			event.events = EPOLLOUT;
			epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[i].data.fd, &event);
		}
		else if (events[i].events & EPOLLOUT)
		{
			event.events = EPOLLIN;
			epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[i].data.fd, &event);
		}
	}
};
