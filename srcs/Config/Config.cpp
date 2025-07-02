/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:46:38 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/02 06:18:42 by tlonghin         ###   ########.fr       */
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
        std::cout << "---------------------------" << std::endl;
        for(std::map<std::string, IS_Location>::iterator it = this->location.begin(); it != this->location.end(); ++it) {
            std::cout << it->second.getRoot() << std::endl;
            std::cout << it->second.getIndex() << std::endl;
            std::cout << it->second.getDirectoryListing() << std::endl;
            std::cout << it->second.getUploadPath() << std::endl;
            std::cout << it->second.getUploadEnable() << std::endl;
            std::cout << it->second.getLocationGetMethod() << std::endl;
            std::cout << it->second.getLocationPostMethod() << std::endl;
            std::cout << it->second.getLocationDeleteMethod() << std::endl;
            std::cout << "-----------------------------" << std::endl;
        }
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