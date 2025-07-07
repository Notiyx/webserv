/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_poll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:25:11 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/06 19:32:01 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <AllException.hpp>
#include <HTTPResponse.hpp>
#include <Config.hpp>
#include <sys/epoll.h>
#include <string.h>
#include <netinet/in.h>
#include <vector>
#include <unistd.h>

class E_poll {
	private:
		int		epoll_fd;
        Config  conf;
	public:
		E_poll(Config conf);
		~E_poll();
		void epollInit(int serv_fd);
		void epollExec(int serv_fd);
		bool isValidRequest(int client_fd, std::string &request);
		void LaunchRequest(int client_fd, std::string& request);
		void sendError(int code, std::string msg);
};