/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 23:41:48 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/07 21:38:41 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <HTTPResponse.hpp>

HTTPResponse::HTTPResponse() : msg(""), res(0) {};

HTTPResponse::HTTPResponse(int res, std::string msg, Config &conf)
    : msg(msg), res(res), conf(conf) {}

HTTPResponse::~HTTPResponse() {};

std::string HTTPResponse::buildDirectoryListHtml(std::string path) {
	DirectoryListing directoryList;
	directoryList.setListing(path.substr(1) + "/");
	std::map<std::string, IS_FolderList> list = directoryList.getListing();
	std::map<std::string, IS_FolderList>::iterator it = list.begin();
	std::ifstream file("front/directoryList.html", std::ios::binary);
	if (!file.is_open())
	{
		std::ostringstream oss;
		oss << "HTTP/1.1 404 Not Found\r\n";
        oss << "Content-Length: 0\r\n";
        oss << "Connection: close\r\n\r\n";
        return oss.str();
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();
	std::string placeholder = "<!-- INSERT HERE -->";
	size_t pos = content.find(placeholder);
	if (pos == std::string::npos)
		throw std::runtime_error("Internal Server Error: template missing placeholder");
	std::ostringstream templates;
	for(; it != list.end(); ++it)
	{
		templates << "<div class=\"directory-row\">";
		templates << "<div class=\"directory-name\"><a href=\"" << path << "/" << it->second.getFolderName() << "\">"<< it->second.getFolderName() << "</a></div>";
		templates << "<div class=\"directory-size\">" << it->second.getFolderSize() << " " << it->second.getFolderSuffix() << "</div>";
		templates << "<div class=\"directory-date\">" << it->second.getFolderLastEdit() << "</div>";
		templates << "</div>";
	}
	content.replace(pos, placeholder.length(), templates.str());
	return (content);
};

std::string HTTPResponse::buildDirectoryList(std::string path) {
	std::string htmlBody = buildDirectoryListHtml(path);
	std::ostringstream response;
	response << "HTTP/1.1 200 Created\r\n";
	response << "Content-Length: " << htmlBody.size() << "\r\n";
	response << "Connection: close\r\n\r\n";
	response << htmlBody;
	return (response.str());
};

std::string HTTPResponse::buildResponse() {
	std::stringstream iss;
	if (conf.getSpecificErrorPage(res).size() > 0) {
		std::ifstream file(conf.getSpecificErrorPage(res).c_str());
		iss << "HTTP/1.1 " << res << " " << msg << "\r\n";
		iss << "content-Type: text/html\r\n";
		iss << "connection: close\r\n";
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
		return (iss.str());
	} else {
		iss << "HTTP/1.1 " << res << " " << msg << "\r\n";
		iss << "content-Type: text/html\r\n";
		iss << "connection: close\r\n";
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
		oss << "Connection: close\r\n\r\n";
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
	oss << "Connection: close\r\n\r\n";
	oss<< file.rdbuf();
	file.close();
	std::cout << oss.str() << std::endl;
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
