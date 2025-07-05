/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 01:56:13 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/05 06:39:27 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <HTTPResponse.hpp>
#include <NameSpace.hpp>
#include <Config.hpp>
#include <cstdlib>
#include <sstream>
#include <map>
#include <unistd.h>

class Request {
	private:
		Config conf;
		int	   client_fd;
		enum Method {GET, POST, DELETE, OTHER};
		Method method;
		std::string path;
		std::map<std::string, std::string> header;
		std::string body;
		void parse(std::string request);
	public:
		Request(std::string request, Config conf, int client_fd);
		~Request();
		void parseHeader();
		void sendError(int code, std::string msg);
		void execute();
};