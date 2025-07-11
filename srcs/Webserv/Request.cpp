/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 02:03:30 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/11 21:48:20 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <Request.hpp>

Request::Request(IS_Client &client, Config conf, int client_fd) : conf(conf), client_fd(client_fd) {
	parse(client);
};

Request::~Request() {};

void Request::pathCGI() {
	size_t posExt = path.find(".py");
	if (posExt == std::string::npos)
		return ;
	this->isCGI = true;
	posExt += 3;
	size_t posQuery = path.find('?');
	
	std::string before_query;
    if (posQuery == std::string::npos)
		before_query = path;
    else
		before_query = path.substr(0, posQuery);
		
		
	std::string script_path = before_query.substr(0, posExt);
	
    std::string path_info = "";
    if (posExt < before_query.size())
        path_info = before_query.substr(posExt);
		
    std::string query_string = "";
    if (posQuery != std::string::npos)
		query_string = path.substr(posQuery + 1);
		
	this->path = script_path;
	this->path_info = path_info;
	this->query_string = query_string;
};

void Request::parse(IS_Client &client) {
		std::istringstream iss(client.getBuffer());
		std::string meth;
		std::string line;
		this->isCGI = false;
		this->isChunked = false;
		iss >> meth >> this->path;
		pathCGI();
		if (meth == "GET")
			this->method = GET;
		else if (meth == "POST")
			this->method = POST;
		else if (meth == "DELETE")
			this->method = DELETE;
		else
			this->method = OTHER;
		std::string key;
		std::string content;
		std::getline(iss, line);
		while(std::getline(iss, line) && line != "\r" && !line.empty())
		{
			if (line.find(':') == std::string::npos)
			{
				sendError(400, "Bad request");
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
			if (line.find("Transfer-Encoding") != std::string::npos && line.find("chunked") != std::string::npos)
			{
				this->isChunked = true;
			}
			key = utils::trim(key);
			content = utils::trim(content);
			header.insert(std::make_pair(key, content));
		}
		size_t pos = client.getBuffer().find("\r\n\r\n");
		if (pos == std::string::npos)
		{
			sendError(400, "Bad request");
			return ;
		} 
		if (header.find("Content-Length") != header.end() || this->isChunked)
		{
			size_t length = std::atoi(header["Content-Length"].c_str());
			body = client.getBuffer().substr(pos + 4);
			if (body.size() < length)
			{
				sendError(431, "Body too short");
				return;
			}
		}
};

void Request::sendError(int code, std::string msg) {
	HTTPResponse error(code, msg, conf);
	try {
		error.send(this->client_fd);
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

bool Request::parseBody() {
	std::string delimiteur = boundary;
	std::string end_delimiteur = "--" + boundary + "--";
	size_t pos = 0;
	if (body.empty() || body == "\n")
	{
		sendError(400, "Bad Request");
		return (false);
	}
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
	return (true);
};


bool Request::parseHeader() {
	bool ishost = false;
	for(std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); ++it)
	{
		if ((this->method == GET || this->method == DELETE) && !body.empty())
		{
			sendError(400, "Bad Request");
			return (false);
		}
		if (it->first == "Host")
		{
			ishost = true;
			if (it->second != conf.getHostAndPort())
			{
				sendError(400, "Bad Request");
				return (false);
			}
		}
		if (this->method == POST && it->first == "Content-Length")
		{
			size_t size = std::atoi(it->second.c_str());
			if (size > conf.getMaxBodySize() || size > static_cast<size_t>(std::numeric_limits<int>::max()))
			{
				sendError(413, "Content Too Large");
				return (false);
			}
			if (body.length() != size)
			{
				sendError(400, "Bad Request");
				return (false);
			}
		}
		if (this->method == POST && it->first == "Content-Type")
		{
			if (it->second != "application/x-www-form-urlencoded"
				&& (it->second.find("multipart/form-data;") == std::string::npos))
			{
				std::map<std::string, IS_Location>::iterator  itLoc = conf.getLocation(path);
				if (itLoc->second.getUploadEnable())
					sendError(415, "Uploads are disabled on this location");
				else
					sendError(400, "Bad Request");
				return (false);
			}
		}

	}
	if (!ishost) {
		sendError(400, "Bad Request");
		return (false);
	}
	return (true);
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
		std::string uniqueFilename = getUniqueFilename(uploadPath, part.filename);
		std::string fullpath = uploadPath + "/" + uniqueFilename;
		struct stat st;
		if (stat(uploadPath.c_str(), &st) == -1)
			mkdir(uploadPath.c_str(), 0755);
		std::ofstream file(fullpath.c_str(), std::ios::binary);
		if (!file.is_open())
		{
			sendError(500, "Internal Server Error");
			return (false);
		}
		file.write(&part.content[0], part.content.size());
		file.close();
	} else 
	{
		std::map<std::string, IS_Location>::iterator  it = conf.getLocation(path);
		std::string root = it->second.getRoot();
		std::string index = it->second.getIndex();
		if (root.empty())
			root = conf.getDefaultRoot();
		if (index.empty())
			index = conf.getDefaultIndex();
		std::string fullpath = root + "/" + index;
		std::ofstream file(fullpath.c_str(), std::ios::out | std::ios::app);
		if (!file.is_open()) {
			sendError(500, "Internal Server Error");
			return (false);
		}
		file.write(&part.content[0], part.content.size());
		file.close();
	}
	return (true);
};

bool	Request::unChunk() {
	std::string newBody;
	std::string hexa;

	header.erase("Transfer-Encoding");
	while(!body.empty())
	{
		size_t end_line = body.find("\r\n");
		if (end_line == std::string::npos)
			return (false);
		hexa = body.substr(0, end_line);
		body.erase(0, end_line + 2);
		char *fail;
		long size = std::strtol(hexa.c_str(), &fail, 16);
		if (*fail != '\0' || size < 0)
			return (false);
		if (size == 0)
			break ;
		if (body.size() < static_cast<size_t>(size + 2))
			return (false);
		newBody.append(body, 0, size);
		if (body[size] != '\r' || body[size + 1] != '\n')
			return (false);
		body.erase(0, size + 2);
	}
	body = newBody;
	return (true);
};

bool Request::executePost(std::string filename) {
	std::ofstream file(filename.c_str(), std::ios::out | std::ios::app);
	if (!file.is_open()) {
		sendError(500, "Internal Server Error");
		return (false);
	}
	std::istringstream iss(body);
	std::string data;
	while(std::getline(iss, data, '&'))
		file << data << "\n";
	file.close();
	return (true);
};

std::string Request::pathManager(const std::string& root, const std::string& path) {
	if(root.empty())
		return (path);
	std::string newRoot = root;
	std::string newPath = path;

	if (!newRoot.empty() && newRoot[newRoot.size() - 1] == '/')
		newRoot = newRoot.substr(0, newRoot.size() - 1);
	if (!newPath.empty() && newPath[0] == '/')
		newPath = newPath.substr(1);
	return newRoot + "/" + newPath;
};

std::string Request::deleteFiles(std::string filename) {
	if (filename.find("..") != std::string::npos ||
	filename.find('\\') != std::string::npos) 
	{
		sendError(400, "Bad Request: Invalid filename");
		return ("");
	}
	if (access(filename.c_str(), F_OK) != 0) 
	{
		sendError(404, "File Not Found");
		return ("");
	}
	if (remove(filename.c_str()) != 0) 
	{
		sendError(500, "Could Not Delete File");
		return ("");
	}
	return("HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
};

bool Request::getChunk() {
	return(this->isChunked);
};

std::string Request::directoryListPath(std::string& root, std::string& path) {
	return (root + path);
};

void Request::execute(IS_Client &client) {
	HTTPResponse valid;
	std::string res;
	std::string pathCopy = path;
	std::map<std::string, IS_Location>::iterator  it;
	if (method == DELETE)
		it = conf.getLocationDelete(pathCopy);
	else
		it = conf.getBestLocation(pathCopy);
	std::map<std::string, IS_Location>::iterator  itD = conf.getDirectoryLocation(pathCopy);
	it = it != this->conf.locationEnd() ? it : itD;
	if (it == this->conf.locationEnd())
	{
		sendError(404, "Not Found");
		return ;
	}
	std::string root = it->second.getRoot();
	std::string index = it->second.getIndex();
	if (root.empty())
		root = conf.getDefaultRoot();
	if (index.empty())
		index = conf.getDefaultIndex();
	if (method == DELETE)
		root += "/uploads";
	std::string fullpath;
	if (!it->second.getDirectoryListing())
		fullpath = pathManager(root, pathCopy);
	else		
		fullpath = directoryListPath(root, pathCopy);
	int redirectCode = it->second.getCodeRedirect();
	if (redirectCode != -1)
	{
		std::string url = it->second.getPathRedirect();
		if (url.size() == 0)
		{
			sendError(400, "Bad Request");
			return ;
		}
		res = valid.redirect(redirectCode, url);
		utils::sender(client_fd, res);
		return ;
	}
	if (this->isCGI)
	{
		try {
			CGI cgi(getMethod(), path, body, path_info, query_string, conf, header);
			body = cgi.execCGI();
			res = valid.buildCGI(body);
		} catch (std::runtime_error& e) { sendError(500, e.what()); return ;}
	}
	else if (method == GET)
	{
		std::cout << utils::getDateCurrent() << " - Get detected" << std::endl;
		std::string dir = client.getDir();
		if (it->second.getDirectoryListing() && (dir.empty() || dir != "/"))
		{
			fullpath = utils::removeIsSpaceBetween(fullpath.c_str());
			if (root.find("/") == std::string::npos)
				root += "/";
			if (fullpath.find(it->second.getRoot()) != std::string::npos && fullpath != root)
			{
				fullpath = fullpath.substr(root.size());
				client.setCurrentDirectoryName(fullpath);
				fullpath = client.getDir() + "/" + fullpath;
			}
		}
		if (it->second.getDirectoryListing() && utils::isDirectory(fullpath) && !this->isCGI)
		{
			try {
				res = valid.buildDirectoryList(it->first, fullpath, root, client);
			}
			catch (const std::runtime_error& e) { std::cerr << e.what() << std::endl; sendError(500, "Internal Server Error"); }
		} else if (it->second.getDirectoryListing() && !this->isCGI)
		{
			if (!utils::isFile(fullpath) && !this->isCGI)
			{
				if (utils::isFile(fullpath))
				res = valid.buildGet(fullpath);
			}
			else 
				res = valid.buildGet(fullpath);
		}
		else if (!this->isCGI)
		{
			fullpath += index;
			res = valid.buildGet(fullpath);
		} else
		{
			res = valid.buildGet(fullpath);	
		}
	} else if (method == POST) 
	{
		std::cout << utils::getDateCurrent() << " - Post detected" << std::endl;
		if (getContentType() == "application/x-www-form-urlencoded")
		{
			res = valid.buildPost();
			if (!executePost(fullpath))
				return ;
		}
		else if (getContentType().find("multipart/form-data") != std::string::npos && !this->isCGI)
		{
			std::vector<data>::iterator part = bodyParts.begin();
			for(; part != bodyParts.end(); ++part)
			{
				if (!executeUpload(*part, it->second.getUploadPath()))
					return ;
			}
			res = valid.buildPost();
		}
	} else if (method == DELETE) {
		std::cout << utils::getDateCurrent() << " - Delete detected" << std::endl;
		res = deleteFiles(fullpath);
		if (res.empty())
			return ;
	}else {
		sendError(405, "Method Not Allowed");
		return ;
	}
	utils::sender(client_fd, res);
};
