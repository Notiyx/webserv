/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:46:38 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 06:46:57 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Config.hpp>
#include <Utils.hpp>

Config::Config() {
}

void    Config::parseConfig(const char *av) {
    std::string   strFile(av);
    std::string   extensionFile(".conf");

    if (!utils::checkEndStr(strFile, extensionFile))
        throw ConfigFileError("Error: Invalid extension in file name !");

    std::ifstream infile(av);
    std::string   valueRead;
    if (!infile.is_open())
        throw (ConfigFileError("Error: error during open file"));
    if (infile)
    while (std::getline(infile, valueRead)) {
        std::cout << valueRead << std::endl;
    } 
}
Config  &Config::operator=(const Config &conf) {
    if (this != &conf)
    {
        this->serverName = conf.serverName;
        this->clientMaxRequest = conf.clientMaxRequest;
        this->errorPage.insert(conf.errorPage.begin(), conf.errorPage.end());
        this->host = conf.host;
        this->listen = conf.listen;
        this->location.insert(conf.location.begin(), conf.location.end());
    }
    return (*this);
}

Config::~Config() {}