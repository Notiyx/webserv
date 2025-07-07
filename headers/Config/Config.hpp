/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:43:07 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/06 20:07:22 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <AllException.hpp>
#include <Interface.hpp>
#include <InstanceInterface.hpp>
class Config {
    private:
        std::string serverName;
        std::size_t clientMaxRequest;
        IS_Host host;
        IS_Listen listen;
        std::map<std::string, IS_ErrorPage> errorPage;
        std::map<std::string, IS_Location> location;
    public:
        Config();
        Config          &operator=(const Config &conf);
        void            parseConfig(const char *av);
        void            printConfig(const std::string strFile);
        int             getPort();
        std::string     getHostAndPort();
        std::string     getHostName();
        std::size_t     getMaxClients();
        std::string     getServName();
        bool            isLocation(std::string key);
        std::map<std::string, IS_Location>::iterator  locationEnd();
		std::map<std::string, IS_Location>::iterator  getBestLocation(const std::string &path);
        std::map<std::string, IS_Location>::iterator     getLocation(std::string key);
        ~Config();
};