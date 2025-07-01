/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:46:38 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 08:19:46 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Config.hpp>
#include <NameSpace.hpp>

Config::Config() {
}

void    Config::parseConfig(const char *av) {
    std::string   strFile(av);
    std::string   extensionFile(".conf");
    std::string   valueRead;

    if (!utils::checkEndStr(strFile, extensionFile))
        throw (ConfigFileError("Error: Invalid extension in file name !"));
    std::ifstream infile(av);
    if (!infile.is_open())
        throw (ConfigFileError("Error: error during open file"));
    if (!configUtils::checkIsPairChar(infile))
    {
        infile.close();
        throw (ConfigFileError("Error: Missing brace in the configuration file"));
    }
    try {
        this->location = configUtils::findLocation(infile);
    } catch(const ConfigFileError& e) {
        throw (ConfigFileError(e));
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