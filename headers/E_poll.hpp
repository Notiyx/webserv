/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_poll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:25:11 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/04 20:32:14 by nmetais          ###   ########.fr       */
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
		int		epoll_fd;
	public:
		E_poll();
		~E_poll();
		void epollInit(int serv_fd);
		void epollLaunch(int serv_fd);
};