/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 23:41:36 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/09 22:36:49 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <NameSpace.hpp>
#include <AllException.hpp>
#include <Config.hpp>
#include <DirectoryListing.hpp>

class HTTPResponse {
	private:
		std::string msg;
		int			res;
		Config		conf;
	public:
		HTTPResponse();
		HTTPResponse(int res, std::string msg, Config &conf);
		~HTTPResponse();
		std::string buildResponse();
		std::string buildFile(std::string path);
		std::string buildDirectoryList(std::string path);
		std::string buildDirectoryListHtml(std::string path);
		std::string buildGet(std::string filename);
		std::string buildPost();
		std::string redirect(int redirectCode, std::string url);
		std::string buildCGI(std::string bodyCGI);
		void send(int client_fd);
};