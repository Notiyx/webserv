/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:09:37 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/07 08:15:17 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <HTTPResponse.hpp>
#include <NameSpace.hpp>
#include <Config.hpp>
#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>

class CGI {
	private:
	std::string method;
	std::string path;
	std::string body;
	std::string path_info;
	std::string query_string;
	Config conf;
	std::map<std::string, std::string> header;
	std::vector<std::string> env;
	public:
		CGI(const std::string& method,
		const std::string& path,
		const std::string& body,
		const std::string& path_info,
		const std::string& query_string,
		const Config& conf,
		const std::map<std::string, std::string>& header);
		~CGI();
		void buildEnv();
		void execCGI();
};