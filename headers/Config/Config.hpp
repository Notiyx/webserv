/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:43:07 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 06:07:57 by tlonghin         ###   ########.fr       */
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
        IS_Host host;
        IS_Listen listen;
        std::size_t clientMaxRequest;
        std::map<std::string, IS_ErrorPage> errorPage;
        std::map<std::string, IS_Location> location;
    public:
        Config();
        Config  &operator=(const Config &conf);
        void    parseConfig(const char *av);
        ~Config();
};