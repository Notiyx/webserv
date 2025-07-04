/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:48:10 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/04 06:24:27 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

Webserv::Webserv() {}

void    Webserv::setConfig(const Config newConf) {
    this->conf = newConf;
}

//Fonction qui protege et envois la bonne erreure si un fd fail;
void check_syscall(int ret, const char* syscall_name) {
    if (ret < 0)
        throw SetupServError(std::string(syscall_name) + " failed: " + strerror(errno));
}

void Webserv::launchServ() {
    std::cout << "server: " << conf.getServName() << " launched. Port: " << conf.getPort() << std::endl;
    while (true)
    {
        
    }
};

void Webserv::setupServ() {
    //AF_INET = Ipv4, 
    //SOCK_STREAM = TCP
    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    check_syscall(serv_fd, "socket");
    //F_SETFL = modifier comportement d'un fd
    //0_NONBLOCK = Fd en mode non bloquant pour eviter les freeze
    check_syscall(fcntl(serv_fd, F_SETFL, O_NONBLOCK), "fcntl");
    sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(conf.getPort());
	//le serv ecoute sur toutes les ips disponibles;
	addr.sin_addr.s_addr = INADDR_ANY;
    check_syscall(bind(serv_fd, (sockaddr *)&addr, sizeof(addr)), "bind");
    check_syscall(listen(serv_fd, SOMAXCONN), "listen");
};

Webserv::~Webserv() {}

