/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 02:03:30 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/06 17:58:18 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Request.hpp>

Request::Request(std::string request, Config conf, int client_fd) : conf(conf), client_fd(client_fd) {
	parse(request);
};

Request::~Request() {};


void Request::parse(std::string request) {
		std::istringstream iss(request);
		std::string meth;
		std::string line;
		iss >> meth >> path;
		if (meth == "GET")
			method = GET;
		else if (meth == "POST")
			method = POST;
		else if (meth == "DELETE")
			method = DELETE;
		else
			method = OTHER;
		std::string key;
		std::string content;
		std::getline(iss, line);
		while(std::getline(iss, line) && line != "\r" && !line.empty())
		{
			if (line.find(':') == std::string::npos)
			{
				sendError(420, "Bad request");
				return ;
			}
			std::istringstream issl(line);
			std::getline(issl, key, ':');
			if (line.find("multipart/form-data") != std::string::npos)
			{
				std::getline(issl, content);
				size_t pos = content.find("boundary=");
				if (pos != std::string::npos)
				{
					std::string tmp = content.substr(pos + 9);
					boundary = "--" + tmp;
				}
			} else
				std::getline(issl, content);
			key = utils::trim(key);
			content = utils::trim(content);
			header.insert(std::make_pair(key, content));
		}
		size_t pos = request.find("\r\n\r\n");
		if (pos == std::string::npos)
		{
			sendError(430, "Bad request");
			return ;
		} 
		if (header.find("Content-Length") != header.end())
		{
			size_t length = std::atoi(header["Content-Length"].c_str());
			body = request.substr(pos + 4);
			if (body.size() < length)
			{
				sendError(431, "Body too short");
				return;
			}
		}
};

void Request::sendError(int code, std::string msg) {
	HTTPResponse error(code, msg);
	try {
		error.send(client_fd);
	} catch (const fdError &e) {
		std::cerr << e.what() << std::endl;
		return ;
	}
};

void Request::parseBodyPart(std::string part) {
	data newReq;
	size_t posContentType = part.find("Content-Type");
	if (posContentType != std::string::npos)
	{
		size_t start = part.find(":", posContentType);
		if (start != std::string::npos)
		{
			start++;
			size_t end = part.find("\r\n", start);
			if (end == std::string::npos)
                end = part.length();
			newReq.contentType = part.substr(start, end - start);
			newReq.contentType = utils::trim(newReq.contentType);
		}
	}
	size_t posContentDispo = part.find("Content-Disposition");
	if (posContentDispo != std::string::npos)
	{
		size_t start = part.find("filename=\"", posContentDispo);
		if (start != std::string::npos)
		{
			start += 10;
			size_t end = part.find("\"", start);
			if (end != std::string::npos)
			{
				newReq.filename = part.substr(start, end - start);
				newReq.filename = utils::trim(newReq.filename);
			}
		}
	}
	size_t start = part.find("\r\n\r\n");
	if (start != std::string::npos)
	{
		start += 4;
		newReq.content.insert(newReq.content.end(), part.begin() + start, part.end());
	}
	bodyParts.push_back(newReq);
};

std::string Request::getMethod() {
	if (method == GET)
		return ("GET");
	else if (method == POST)
		return ("POST");
	else if (method == DELETE)
		return ("DELETE");
	else
		return (NULL);
};




void Request::parseBody() {
	std::string delimiteur = boundary;
	std::string end_delimiteur = "--" + boundary + "--";
	size_t pos = 0;
	while ((pos = body.find(delimiteur, pos)) != std::string::npos)
	{
		pos += delimiteur.length();
		size_t next = body.find(boundary, pos);
		std::string part;
		if (next == std::string::npos)
		{
			part = body.substr(pos);
			parseBodyPart(part);
			break ;
		}
		part = body.substr(pos, next - pos);
		parseBodyPart(part);
		if (body.substr(next, end_delimiteur.length()) == end_delimiteur)
			break ;
		pos = next;
	}
};

void Request::parseHeader() {
		bool ishost = false;
		for(std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it)
		{
			if ((method == GET || method == DELETE) && !body.empty())
			{
				sendError(401, "Bad Request");
				return ;
			}
			if (it->first == "Host")
			{
				ishost = true;
				if (it->second != conf.getHostAndPort())
				{
					sendError(402, "Bad Request");
					return ;
				}
			}
			if (method == POST && it->first == "Content-Length")
			{
				size_t size = std::atoi(it->second.c_str());
				//faut cast mieux mais il est tard
				if (size > static_cast<size_t>(50000000) || size > static_cast<size_t>(std::numeric_limits<int>::max()))
				{
					sendError(413, "Payload Too Large");
					return ;
				}
				if (body.length() != size)
				{
					sendError(403, "Bad Request");
					return ;
				}
			}
			if (method == POST && it->first == "Content-Type")
			{
				if (it->second != "application/x-www-form-urlencoded"
					&& (it->second.find("multipart/form-data;") == std::string::npos))
				{
					std::map<std::string, IS_Location>::iterator itLoc = conf.getLocation(path);
					if (itLoc->second.getUploadEnable())
						sendError(415, "Uploads are disabled on this location");
					else
						sendError(404, "Bad Request");
					return ;
				}
			}
		}
		if (!ishost)
		{
			sendError(405, "Bad Request");
			return ;
		}
};

std::string Request::getContentType() {
	for(std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it)
	{
		if (it->first == "Content-Type")
			return (std::string(it->second));
	}
	return (NULL);
};

bool fileExist(std::string path)
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

std::string	Request::getUniqueFilename(std::string directory, std::string filename)
{
	std::string name;
	std::string extension;
	size_t pos = filename.find_last_of('.');
	if (pos != std::string::npos)
	{
		name = filename.substr(0, pos);
		extension = filename.substr(pos);
	}
	std::string newName = filename;
	int count = 1;
	while (fileExist(directory + "/" + newName))
	{
		if (count > 1000)
		{
			sendError(500, "Internal server Error");
			return (NULL);
		}
		std::ostringstream oss;
		oss << name << "_" << count << extension;
		newName = oss.str();
		count++;
	}
	std::cout << newName << std::endl;
	return (newName);
}

bool Request::executeUpload(data part, std::string uploadPath) {
	if (!part.filename.empty())
	{
		if (part.filename.find("..") != std::string::npos ||
			part.filename.find('/') != std::string::npos ||
			part.filename.find('\\') != std::string::npos) 
			{
			sendError(400, "Bad Request: Invalid filename");
			return (false);
			}
		std::string uniqueFilename = getUniqueFilename("front/uploads", part.filename);
		std::string fullpath = uploadPath + "/" + uniqueFilename;
		std::cout << "fullpath " << uploadPath << std::endl;
		struct stat st;
		if (stat("front/uploads", &st) == -1)
			mkdir("front/uploads", 0755);
		std::ofstream file(fullpath.c_str(), std::ios::binary);
		if (!file.is_open()) 
		{
			sendError(500, "Internal Server Error");
			close(client_fd);
			return (false);
		}
		file.write(&part.content[0], part.content.size());
		file.close();
	} else 
	{
		std::map<std::string, IS_Location>::iterator it = conf.getLocation(path);
		std::string root = it->second.getRoot();
		std::string index = it->second.getIndex();
		std::string fullpath = root + "/" + index;
		std::ofstream file(fullpath.c_str(), std::ios::out | std::ios::app);
		if (!file.is_open()) {
			sendError(500, "Internal Server Error");
			close(client_fd);
			return (false);
		}
		file.write(&part.content[0], part.content.size());
		file.close();
	}
	return (true);
};


bool Request::executePost(std::string filename) {
	std::ofstream file(filename.c_str(), std::ios::out | std::ios::app);
	if (!file.is_open()) {
		sendError(500, "Internal Server Error");
		close(client_fd);
		return (false);
	}
	std::istringstream iss(body);
	std::string data;
	while(std::getline(iss, data, '&'))
		file << data << "\n";
	file.close();
	return (true);
};


void Request::execute() {
	HTTPResponse valid;
	std::string res;
	std::map<std::string, IS_Location>::iterator it = conf.getLocation(path);
	std::string root = it->second.getRoot();
	std::string index = it->second.getIndex();
	std::string fullpath = root + "/" + index;
	if (method == GET)
	{
		std::cout << "GET" << std::endl;
/* 		if (it->second.getDirectoryListing() 
			&& it->second.getIndex().empty() && utils::fileExist(fullpath))
		{
			res = valid.buildDirectoryList(fullpath);
		}
		else */
		res = valid.buildGet(fullpath);
	} else if (method == POST) {

		if (getContentType() == "application/x-www-form-urlencoded")
		{
			std::cout << "POST" << std::endl;
			res = valid.buildPost();
			if (!executePost(fullpath))
				return ;
		}
		else if (getContentType().find("multipart/form-data") != std::string::npos)
		{
			std::cout << "UPLOAD" << std::endl;
			std::vector<data>::iterator part = bodyParts.begin();
			for(; part != bodyParts.end(); ++part)
			{
				if (!executeUpload(*part, it->second.getUploadPath()))
					return ;
			}
			res = valid.buildPost();
		}
	} else if (method == DELETE) {
		std::cout << "DELETE" << std::endl;
	}else {
		sendError(405, "Method Not Allowed");
		return ;
	}
	utils::sender(client_fd, res);
	close(client_fd);
};
