/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 01:56:13 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/08 06:39:49 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <CGI.hpp>
#include <HTTPResponse.hpp>
#include <NameSpace.hpp>
#include <Config.hpp>
#include <cstdlib>
#include <sstream>
#include <map>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits>


class Request {
	private:
		Config conf;
		int	   client_fd;
		struct data {
			std::string filename;
			std::string contentType;
			std::vector<char> content;
		
			data() : filename(""), contentType(""), content() {}
		};
		enum Method {GET, POST, DELETE, OTHER};
		Method method;
		std::string path;
		std::map<std::string, std::string> header;
		std::vector<data> bodyParts;
		std::string body;
		std::string boundary;
		std::string path_info;
		std::string query_string;
		std::vector<std::string> env;
		bool 		isCGI;
		bool		isChunked;
		void parse(std::string request);
	public:
		Request(std::string request, Config conf, int client_fd);
		~Request();
		void pathCGI();
		void parseHeader();
		bool unChunk();
		void parseBodyPart(std::string part);
		bool parseBody();
		void sendError(int code, std::string msg);
		std::string pathManager(const std::string& root, const std::string& path);
		void execute();
		std::string getContentType();
		bool getChunk();
		std::string getMethod();
		bool executePost(std::string filename);
		bool executeUpload(data part, std::string uploadPath);
		std::string deleteFiles(std::string filename);
		std::string	getUniqueFilename(std::string directory, std::string filename);
};