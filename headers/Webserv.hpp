/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:35:47 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/09 20:52:34 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <Config.hpp>
#include <AllException.hpp>
#include <NameSpace.hpp>
#include <sys/socket.h>
#include <cstring>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <streambuf>

class Config;
class Exception;

class Webserv {
    private :
        int     serv_fd;
        Config  conf;
    public :
    Webserv();
    void    setupServ();
    void    setConfig(const Config newConf);
    void    launchServ();
    ~Webserv();
};