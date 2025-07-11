/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:46:38 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/11 21:31:43 by nmetais          ###   ########.fr       */
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
        this->default_root = parsingFunction::findDefaultRoot(infile);
        this->default_index = parsingFunction::findDefaultIndex(infile);
        this->errorPage = parsingFunction::findErrorPage(infile);
        this->location = parsingFunction::findLocation(infile);
        //this->printConfig(strFile);
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
            std::cout << it->second.getCodeRedirect() << " --> Code REDIRECT" << std::endl;
            std::cout << it->second.getPathRedirect() << " --> Path REDIRECT" << std::endl;
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
        this->default_index = conf.default_index;
        this->default_root  = conf.default_root;
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
    return (this->serverName);
};

std::string     Config::getDefaultIndex(){
    return (this->default_index);
};

std::string     Config::getDefaultRoot(){
    return (this->default_root);
};

std::size_t    Config::getMaxBodySize(){
    return (this->clientMaxRequest);
};

bool    Config::isLocation(const std::string key) {
    return this->location.find(key) != this->location.end();
}

std::string Config::getHostName() {
    return (listen.getListenHostname());
}

std::string Config::getHostAndPort() {
    return (host.getHostAndPort());
}

std::map<std::string, IS_Location>::iterator     Config::getLocation(std::string key) {
    return (this->location.find(key));
}

std::map<std::string, IS_Location>::iterator Config::getLocationDelete(std::string& path) {
    std::map<std::string, IS_Location>::iterator comp = location.end();
    size_t len = 0;
    for(std::map<std::string, IS_Location>::iterator  it = location.begin(); it != location.end(); ++it)
    {
        const std::string locpath = it->first;
        if (path.compare(0, locpath.size(), locpath) == 0 && (path.size() == locpath.size() || path[locpath.size()] == '/' || locpath == "/")) {
            if (locpath.size() > len) {
                comp = it;
                len = locpath.size();
            }
        }
    }
    if (comp != location.end()) {
        path = path.substr(comp->first.size());
        if (path.empty())
            path = "/";
    }
    return (comp);
};


std::map<std::string, IS_Location>::iterator Config::getDirectoryLocation(std::string& path) {
    std::map<std::string, IS_Location>::iterator comp = location.end();
    size_t len = 0;
    for(std::map<std::string, IS_Location>::iterator  it = location.begin(); it != location.end(); ++it)
    {
        const std::string locpath = it->first;
        if (path.compare(0, locpath.size(), locpath) == 0 && (path.size() == locpath.size() || path[locpath.size()] == '/' || locpath == "/") && it->second.getDirectoryListing()) {
            if (locpath.size() > len) {
                comp = it;
                len = locpath.size();
            }
        }
    }
    if (comp != location.end()) {
        path = path.substr(comp->first.size());
        if (path.empty())
            path = "/";
    }
    return (comp);
};

std::map<std::string, IS_Location>::iterator Config::getBestLocation(std::string &path) {
    std::map<std::string, IS_Location>::iterator comp = location.end();
    size_t maxLen = 0;
    if (path[path.size() - 1] == '?' && path.find("/CGI/") != std::string::npos)
        path = path.substr(0, path.size() - 1);
    for (std::map<std::string, IS_Location>::iterator it = location.begin(); it != location.end(); ++it) {
        const std::string& locationPath = it->first;

        if (path.compare(0, locationPath.size(), locationPath) == 0) {
            if (path.size() == locationPath.size() || path[locationPath.size()] == '/') {
                if (locationPath.size() > maxLen) {
                    comp = it;
                    maxLen = locationPath.size();
                }
            }
        }
    }
    return (comp);
}

std::map<std::string, IS_Location>::iterator  Config::locationEnd() {
	return location.end();
}


std::map<std::string, IS_ErrorPage> Config::getErrorPage() {
    return (this->errorPage);
}

std::string Config::getSpecificErrorPage(int code) {
    std::map<std::string, IS_ErrorPage>::iterator ite = this->errorPage.end();
    for (std::map<std::string, IS_ErrorPage>::iterator it = this->errorPage.begin(); it != ite; ++it) {
        if (it->second.getErrorCode() == code)
            return (it->second.getErrorPath());
    }
    return ("");
}

Config::~Config() {}