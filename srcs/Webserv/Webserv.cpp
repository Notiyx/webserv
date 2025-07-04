/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:48:10 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/04 19:22:21 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

Webserv::Webserv() {}

void    Webserv::setConfig(const Config newConf) {
    this->conf = newConf;
}

void Webserv::launchServ() {
    std::cout << "server: " << conf.getServName() << " launched. Port: " << conf.getPort() << std::endl;
    while (true)
    {
        //epoll class avec init ici
        //code dans le while temporaire
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(serv_fd, (struct sockaddr*)&client_addr, &addr_len);
        utils::check_syscall(client_fd, "accept");
        char buffer[70000];
        size_t byte = read(client_fd, buffer, sizeof(buffer) - 1);
        while (byte)
        {
            if (byte > 0)
            {
                buffer[byte] = '\0';
                if (strncmp(buffer, "GET", 3) == 0)
                    std::cout << buffer << std::endl;
            }
            byte = read(client_fd, buffer, sizeof(buffer) - 1);
        }
    }
};

void Webserv::setupServ() {
    //AF_INET = Ipv4, 
    //SOCK_STREAM = TCP
    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    utils::check_syscall(serv_fd, "socket");
    //F_SETFL = modifier comportement d'un fd
    //0_NONBLOCK = Fd en mode non bloquant pour eviter les freeze
    //check_syscall(fcntl(serv_fd, F_SETFL, O_BLOCK), "fcntl");
    sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(conf.getPort());
	//le serv ecoute sur toutes les ips disponibles;
	addr.sin_addr.s_addr = INADDR_ANY;
    utils::check_syscall(bind(serv_fd, (sockaddr *)&addr, sizeof(addr)), "bind");
    utils::check_syscall(listen(serv_fd, SOMAXCONN), "listen");
};

Webserv::~Webserv() {}

