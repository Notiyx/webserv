/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:43:07 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 05:47:05 by tlonghin         ###   ########.fr       */
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
        std::string listen;
        std::string host;
        std::size_t clientMaxRequest;
        std::map<std::string, IS_ErrorPage> errorPage;
        std::map<std::string, IS_Location> location;
    public:
        Config();
        Config  &operator=(const Config &conf);
        void    parseConfig(const char *av);
        ~Config();
};