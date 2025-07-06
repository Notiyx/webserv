/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 23:41:48 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/06 07:18:48 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <HTTPResponse.hpp>

        // <div class="directory-row">
        //     <div class="directory-name"><a href="../config/">Config/</a></div>
        //     <div class="directory-size">-</div>
        //     <div class="directory-date">2025-06-30 10:10</div>
        // </div>

HTTPResponse::HTTPResponse() : msg(""), res(0) {};

HTTPResponse::HTTPResponse(int res, std::string msg): msg(msg), res(res) {};

HTTPResponse::~HTTPResponse() {};

std::string HTTPResponse::buildDirectoryList(std::string path) {
	DirectoryListing directoryList;
	directoryList.setListing(path);
	// std::map<std::string, IS_FolderList> list = directoryList.getListing();
	// std::map<std::string, IS_FolderList>::iterator it = list.begin();
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
	{
		//faut catch et renvoyer une erreur 500 mais il est 7h du mat j'ai trop la flemme aled
		throw std::runtime_error("Internal Server Error: template missing placeholder");
	}
	content.erase(pos, placeholder.length());
	//j'insere mon html dynamiquement
	//faut faire une fonction build 
	//directoryHTML et build la response de la requete ici
	//flemme la
	return (content);
};

std::string HTTPResponse::buildResponse() {
	std::stringstream iss;
	iss << "HTTP/1.1 " << res << " " << msg << "\r\n";
	iss << "content-Type: text/html\r\n";
	iss << "connection: close\r\n";
	iss << "\r\n";
	iss << "<html><body><h1>" << res << " " << msg << "</h1></body></html>";
	return iss.str();
}

std::string HTTPResponse::buildPost() {
		std::ostringstream oss;
		oss << "HTTP/1.1 201 Created\r\n";
		oss << "Content-Length: 0\r\n";
		oss << "Connection: close\r\n\r\n";
		return oss.str();
};


std::string HTTPResponse::buildGet(std::string filename) {
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
