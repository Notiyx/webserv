/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_poll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:25:11 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/04 19:08:40 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <AllException.hpp>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <vector>

class E_poll {
	private:
		std::vector<int> clients;
		int				 epoll_fd;
	public:
		E_poll();
		~E_poll();
		void epollInit(int serv_fd);
		void epollLaunch(int max, int serv_fd);
};