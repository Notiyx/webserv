/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:48:10 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/05 04:19:23 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>
#include <E_poll.hpp>

Webserv::Webserv() {}

void    Webserv::setConfig(const Config newConf) {
    this->conf = newConf;
}

void Webserv::launchServ() {
    std::cout << "server: " << conf.getServName() << " launched. Port: " << conf.getPort() << std::endl;
    E_poll poll(conf);
    poll.epollInit(serv_fd);
    try {
        while (true)
            poll.epollExec(serv_fd);
    } catch (const fdError& e) {
        std::cerr << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
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

