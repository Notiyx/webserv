/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_poll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:25:11 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/09 19:45:17 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <AllException.hpp>
#include <HTTPResponse.hpp>
#include <InstanceInterface.hpp>
#include <Config.hpp>
#include <sys/epoll.h>
#include <string.h>
#include <netinet/in.h>
#include <vector>
#include <unistd.h>


class E_poll {
	private:
		std::map<int, IS_Client> client_map;
		int		epoll_fd;
        Config  conf;
	public:
		E_poll(Config conf);
		~E_poll();
		bool readClient(int client_fd, IS_Client &client);
		void epollInit(int serv_fd);
		void epollExec(int serv_fd);
		bool isValidRequest(int client_fd, IS_Client &client);
		bool launchRequest(int client_fd, IS_Client &client);
		void sendError(int client_fd, int code, const std::string &msg);
};