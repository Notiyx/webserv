/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 23:41:48 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/11 21:38:00 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <HTTPResponse.hpp>

HTTPResponse::HTTPResponse() : msg(""), res(0) {};

HTTPResponse::HTTPResponse(int res, std::string msg, Config &conf)
    : msg(msg), res(res), conf(conf) {}

HTTPResponse::~HTTPResponse() {};

static std::string chooseMsg(int code) {
	std::string msg;
	switch (code){
		case 300:
			msg = " Multiple Choices";
			break ;
		case 301:
			msg = " Moved Permanently";
			break ;
		case 302:
			msg = " Found";
			break ;
		case 303:
			msg = " See Other";
			break ;
		case 307:
			msg = " Temporary Redirect";
			break ;
		case 308:
			msg = " Permanent Redirect";
			break ;
	}
	return (msg);
};

std::string HTTPResponse::redirect(int redirectCode, std::string url){
	std::ostringstream oss;
	std::string redirectmsg = chooseMsg(redirectCode);
	oss << "HTTP/1.1 " << redirectCode << " " << redirectmsg << "\r\n";
    oss << "Location: " << url << "\r\n";
    oss << "Content-Length: 0\r\n\r\n";
	return (oss.str());
};


std::string HTTPResponse::buildDirectoryListHtml(std::string route, std::string path, std::string root, IS_Client &client) {
	DirectoryListing directoryList;
	std::string lastPath;
	try {
		client.setOldDir(route);
		client.setDir(path);
		directoryList.setListing(path + "/");
	}
	catch (const DirectoryListError& e)
	{
		throw(DirectoryListError(e));
	}
	std::map<std::string, IS_FolderList> list = directoryList.getListing();
	std::map<std::string, IS_FolderList>::iterator it = list.begin();
	std::ifstream file("front/directoryList.html", std::ios::binary);
	if (!file.is_open())
	{
		std::ostringstream oss;
		oss << "HTTP/1.1 404 Not Found\r\n";
        oss << "Content-Length: 0\r\n";
        oss << "Connection: close\r\n\r\n";
		file.close();
        return oss.str();
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();
	std::string placeholder = "<!-- INSERT HERE -->";
	size_t pos = content.find(placeholder);
	if (pos == std::string::npos)
	{
		file.close();
		throw std::runtime_error("Internal Server Error: template missing placeholder");
	}
	std::ostringstream templates;
	if (!path.empty() && path[path.size() - 1] == '/')
		path = path.substr(0, path.size() - 1);
	if (path.compare(0, root.size(), root.c_str()) == 0) {
		path = path.substr(root.size() + 1);
	}
	for(; it != list.end(); ++it)
	{
		templates << "<div class=\"directory-row\">";
		if (it == list.begin())
			templates << "<div class=\"directory-name\"><a href=\"" << client.getOldDir() + "/" << "\">"<< it->second.getFolderName()<< "</a></div>";
		else
			templates << "<div class=\"directory-name\"><a href=\"" << route + "/" + it->second.getFolderName() << "\">"<< it->second.getFolderName()<< "</a></div>";
		if (it != list.begin())
		{
			templates << "<div class=\"directory-size\">" << it->second.getFolderSize() << " " << it->second.getFolderSuffix() << "</div>";
			templates << "<div class=\"directory-date\">" << it->second.getFolderLastEdit() << "</div>";
		}
		templates << "</div>";
	}
	content.replace(pos, placeholder.length(), templates.str());
	file.close();
	return (content);
};

std::string HTTPResponse::buildDirectoryList(std::string route, std::string path, std::string root, IS_Client &client) {
	try {
		std::string htmlBody = buildDirectoryListHtml(route, path, root, client);
		std::ostringstream response;
		response << "HTTP/1.1 200 Created\r\n";
		response << "Content-Length: " << htmlBody.size() << "\r\n";
		response << "connection: keep-alive\r\n\r\n";
		response << htmlBody;
		return (response.str());
	} catch (const DirectoryListError& e)
	{
		throw(std::runtime_error(e.what()));
	}
};

std::string HTTPResponse::buildResponse() {
	std::stringstream iss;
	if (conf.getSpecificErrorPage(res).size() > 0) {
		std::ifstream file(conf.getSpecificErrorPage(res).c_str());
		iss << "HTTP/1.1 " << res << " " << msg << "\r\n";
		iss << "Content-Type: text/html\r\n";
		iss << "Connection: close\r\n";
		iss << "\r\n";
		if (!file.is_open())
		{
			iss << "<html><body><h1>" << res << " " << msg << "</h1></body></html>";	
			return (iss.str());
		}
		std::string valueRead;
		while (std::getline(file, valueRead)) {
			iss << valueRead << "\r\n";
		}
		file.close();
		return (iss.str());
	} else {
		iss << "HTTP/1.1 " << res << " " << msg << "\r\n";
		iss << "Content-Type: text/html\r\n";
		iss << "Connection: close\r\n";
		iss << "\r\n";
		iss << "<html><body><h1>" << res << " " << msg << "</h1></body></html>";	
	}
	return (iss.str());
}

std::string HTTPResponse::buildPost() {
		std::ostringstream oss;
		oss << "HTTP/1.1 303 See other\r\n";
		oss << "Location: /\r\n";
		oss << "Content-Length: 0\r\n";
		oss << "Connection: keep-alive\r\n\r\n";
		return oss.str();
};

std::string HTTPResponse::buildCGI(std::string bodyCGI) {
	std::ostringstream oss;
	oss << "HTTP/1.1 200 OK\r\n";
	oss << "Content-Type: text/html\r\n";
	oss << "Content-Length: " << bodyCGI.size() << "\r\n";
	oss << "Connection: keep-alive\r\n\r\n";
	oss << bodyCGI;
	return oss.str();
};

std::string HTTPResponse::buildGet(std::string filename) {
	if (utils::isFile(filename.substr(1)))
		filename = filename.substr(1);
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		std::ostringstream oss;
		oss << "HTTP/1.1 404 Not Found\r\n";
        oss << "Content-Length: 0\r\n";
        oss << "Connection: close\r\n\r\n";
        return oss.str();
	}
	std::ostringstream oss;
	oss << "HTTP/1.1 200 OK\r\n";
	oss << "Content-Type: text/html\r\n";
	//je me met a la fin du fichier, je recup la position et je remet au debut
	file.seekg(0, std::ios::end);
	size_t length = file.tellg();
	file.seekg(0, std::ios::beg);
	oss << "Content-Length: " << length << "\r\n";
	oss << "connection: keep-alive\r\n\r\n";
	oss << file.rdbuf();
	file.close();
	return oss.str();
}

void HTTPResponse::send(int client_fd) {
		std::string res = buildResponse();
		ssize_t total_sent = 0;
		ssize_t to_send = res.size();
		while (total_sent < to_send)
		{
			ssize_t sent = ::send(client_fd, (res.c_str() + total_sent), to_send - total_sent, 0);
			if (sent == -1)
			{
				if (errno == EINTR) continue ;
				if (errno == EAGAIN || errno == EWOULDBLOCK)
					break ;
				utils::check_syscall(sent, "send");
			}
			total_sent += sent;
		}
};
