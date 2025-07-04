/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:46:38 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/04 20:25:00 by nmetais          ###   ########.fr       */
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
    try
    {
        configUtils::checkValidString(infile);
    }
    catch(const ConfigFileError& e)
    {
        throw (ConfigFileError(e));
    }
    
    try {
        this->listen = parsingFunction::findListen(infile);
        this->host = parsingFunction::findHost(infile);
        this->serverName = parsingFunction::findServerName(infile);
        this->clientMaxRequest = parsingFunction::findMaxClientRequest(infile);
        this->errorPage = parsingFunction::findErrorPage(infile);
        this->location = parsingFunction::findLocation(infile);
       // this->printConfig(strFile);
    } catch(const ConfigFileError& e) {
        throw (ConfigFileError(e));
    }
}

void    Config::printConfig(const std::string strFile) {
    std::cout << "=======================" << strFile << "=======================" << std::endl;
        std::cout << this->serverName << " --> serverName" << std::endl;
        std::cout << this->clientMaxRequest << " --> Max client request size" << std::endl;
        std::cout << "-----------------------LISTEN DATA-----------------------" << std::endl;
        std::cout << this->listen.getListenHostAndPort() << " --> HostNameAndPort" << std::endl;
        std::cout << this->listen.getListenPort() << " --> Port" << std::endl;
        std::cout << this->listen.getListenHostname() << " --> HostName" << std::endl;
        std::cout << "-----------------------HOST DATA-----------------------" << std::endl;
        std::cout << this->host.getHostAndPort() << " --> HostNameAndPort" << std::endl;
        std::cout << this->host.getPort() << " --> Port" << std::endl;
        std::cout << this->host.getHostname() << " --> HostName" << std::endl;
        for(std::map<std::string, IS_Location>::iterator it = this->location.begin(); it != this->location.end(); ++it) {
            std::cout << "------------ Location : " << it->first << "------------" << std::endl;
            std::cout << it->second.getRoot() << " --> Root " << std::endl;
            std::cout << it->second.getIndex() << " --> Index " << std::endl;
            std::cout << it->second.getDirectoryListing() << " --> Directory listing active " << std::endl;
            std::cout << it->second.getUploadPath() << " --> Upload Path " << std::endl;
            std::cout << it->second.getUploadEnable() << " --> Upload Enable " << std::endl;
            std::cout << it->second.getLocationGetMethod() << " --> GET Method " << std::endl;
            std::cout << it->second.getLocationPostMethod() << " --> POST Method " << std::endl;
            std::cout << it->second.getLocationDeleteMethod() << " --> DELETE Method " << std::endl;
        }
        std::cout << "-----------------------ERROR PAGE-----------------------" << std::endl;
        for (std::map<std::string, IS_ErrorPage>::iterator it = this->errorPage.begin(); it != this->errorPage.end(); ++it) {
            std::cout << it->first << " --> Error code(String)" << std::endl;
            std::cout << it->second.getErrorCode() << " --> Error code (int)" << std::endl;
            std::cout << it->second.getErrorPath() << " --> Error path" << std::endl;
        }
        std::cout << "=====================================================================" << std::endl;
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

int     Config::getPort(){
    return (host.getPort());
};

std::string     Config::getServName(){
    return (serverName);
};

std::size_t    Config::getMaxClients(){
    return (clientMaxRequest);
};


Config::~Config() {}