/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 23:41:36 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/06 06:38:23 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <NameSpace.hpp>
#include <AllException.hpp>
#include <DirectoryListing.hpp>

class HTTPResponse {
	private:
		std::string msg;
		int			res;
	public:
		HTTPResponse();
		HTTPResponse(int res, std::string msg);
		~HTTPResponse();
		std::string buildResponse();
		std::string buildDirectoryList(std::string path);
		std::string buildGet(std::string filename);
		std::string buildPost();
		void send(int client_fd);
		void send(std::string filename, int client_fd);
};