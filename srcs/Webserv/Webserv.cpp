/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:48:10 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/10 19:11:11 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>
#include <E_poll.hpp>
#include <csignal>

bool g_servOn = false;

Webserv::Webserv() : serv_fd(-1) {}

void    Webserv::setConfig(const Config newConf) {
    this->conf = newConf;
}

void signal_handler(int code) {
    (void) code;
    g_servOn = false;
}

void Webserv::launchServ() {
    std::cout << "server: " << conf.getServName() << " launched. Port: " << conf.getPort() << std::endl;
    E_poll poll(conf);
    poll.epollInit(serv_fd);
    signal(SIGINT, signal_handler);
    g_servOn = true;
    try {
        while (g_servOn)
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
    int opt = 1;
    setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    utils::check_syscall(bind(serv_fd, (sockaddr *)&addr, sizeof(addr)), "bind");
    utils::check_syscall(listen(serv_fd, SOMAXCONN), "listen");
};

Webserv::~Webserv() {
    if (this->serv_fd > -1)
        close(this->serv_fd);
}

