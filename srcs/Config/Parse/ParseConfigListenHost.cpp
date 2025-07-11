/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigListenHost.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 07:28:13 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/11 21:03:55 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AllException.hpp>
#include <NameSpace.hpp>
#include <cstdlib>

IS_Listen   parsingFunction::findListen(std::istream &infile) {
    IS_Listen isl;
    int       line = 0;
    bool      alreadySet = false;
    std::string valueRead;
    std::string host;
    std::string port;
    std::ostringstream oss;

    while (std::getline(infile, valueRead)) {
        line++;
        if (valueRead.find("listen") == std::string::npos)
            continue ;
        if (alreadySet)
            throw (ConfigFileError("Error Listen: Multiple listen definition."));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (!isspace(valueRead[6]))
        {
            oss << "Error Listen: spaces not found after keywords at line " << line << " !";
            std::string error(oss.str()); 
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(7);
        if (valueRead.find(":") == std::string::npos) {
            oss << "Error Listen: characters ':' missing separators between hostname and port at line " << line << " !";
            std::string error(oss.str()); 
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error Listen : ';' not found at the end of line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error Listen : ';' missplaced at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        host = valueRead.substr(0, valueRead.find(":"));
        port = valueRead.substr(valueRead.find(":") + 1, valueRead.size());
        port = port.substr(0, port.size() - 1);
        if (!utils::isOnlyDigit(port.c_str()))
        {
            oss << "Error Listen : Listening port must include only numbers ! (line :" << line << ")";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (port.size() > 8) {
            oss << "Error Listen : Max port number is 8 ! (line :" << line << ")";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(0, valueRead.find(";"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        host = utils::removeIsSpaceBetween(host.c_str());
        if (std::atoi(port.c_str()) > 65535)
        {
            oss << "Error in port, the value up 65535 at line " << line;
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        isl.setListenHostAndPort(valueRead);
        isl.setListenHostname(host);
        isl.setListenPort(std::atoi(port.c_str()));
        alreadySet = true;
    }
    if (!alreadySet)
        throw (ConfigFileError("Error Listen: No listen set in config !"));
    infile.clear();
    infile.seekg(0, std::ios::beg);
    return (isl);
}

IS_Host   parsingFunction::findHost(std::istream &infile) {
    IS_Host ish;
    int       line = 0;
    bool      alreadySet = false;
    std::string valueRead;
    std::string host;
    std::string port;
    std::ostringstream oss;

    while (std::getline(infile, valueRead)) {
        line++;
        if (valueRead.find("host ") == std::string::npos)
            continue ;
        if (alreadySet)
            throw (ConfigFileError("Error Host: Multiple definition for Host !"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        valueRead = valueRead.substr(5);
        if (valueRead.find(":") == std::string::npos) {
            oss << "Error Host: caracter ':' missing separators between hostname and port at line " << line << " !";
            std::string error(oss.str()); 
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error Host : spaces not found after keywords at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error Host : ';' missplaced at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        host = valueRead.substr(0, valueRead.find(":"));
        port = valueRead.substr(valueRead.find(":") + 1, valueRead.size());
        port = port.substr(0, port.size() - 1);
        if (!utils::isOnlyDigit(port.c_str()))
        {
            oss << "Error Host : Listening port must include only numbers ! (line :" << line << ")";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (port.size() > 8) {
            oss << "Error Host : Max port number is 8 ! (line :" << line << ")";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(0, valueRead.find(";"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        host = utils::removeIsSpaceBetween(host.c_str());
        if (std::atoi(port.c_str()) > 65535)
        {
            oss << "Error in port, the value up 65535 at line " << line;
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        ish.setHostAndPort(valueRead);
        ish.setHostname(host);
        ish.setPort(std::atoi(port.c_str()));
        alreadySet = true;
    }
    if (!alreadySet)
        throw (ConfigFileError("Error Host: No Host set in config !"));
    infile.clear();
    infile.seekg(0, std::ios::beg);
    return (ish);
}

std::string parsingFunction::findServerName(std::istream &infile) {
    int       line = 0;
    bool      alreadySet = false;
    std::string valueRead;
    std::string serverName;
    std::ostringstream oss;

    while (std::getline(infile, valueRead)) {
        if (valueRead.find("server_name ") == std::string::npos)
            continue;
        if (alreadySet)
            throw (ConfigFileError("Error ServerName: Multiple definition for server_name !"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error ServerName : no find ';' missing in the end of line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error ServerName : ';' missplaced at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(12);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        serverName = valueRead;
        alreadySet = true;
    }
    if (!alreadySet)
        throw (ConfigFileError("Error ServerName: No ServerName set in config !"));
    infile.clear();
    infile.seekg(0, std::ios::beg);
    return (serverName);
}

std::string parsingFunction::findDefaultRoot(std::istream &infile) {
    int       line = 0;
    bool      alreadySet = false;
    std::string valueRead;
    std::string defaultRoot;
    std::ostringstream oss;

    while (std::getline(infile, valueRead)) {
        if (valueRead.find("default_root ") == std::string::npos)
            continue;
        if (alreadySet)
            throw (ConfigFileError("Error DefaultRoot: Multiple definition for server_name !"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error DefaultRoot : no find ';' missing in the end of line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error DefaultRoot : ';' missplaced at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(14);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        defaultRoot = valueRead;
        alreadySet = true;
    }
    if (!alreadySet)
        throw (ConfigFileError("Error DefaultRoot: No DefaultRoot set in config !"));
    infile.clear();
    infile.seekg(0, std::ios::beg);
    return (defaultRoot);
}

std::string parsingFunction::findDefaultIndex(std::istream &infile) {
    int       line = 0;
    bool      alreadySet = false;
    std::string valueRead;
    std::string defaultIndex;
    std::ostringstream oss;

    while (std::getline(infile, valueRead)) {
        if (valueRead.find("default_index ") == std::string::npos)
            continue;
        if (alreadySet)
            throw (ConfigFileError("Error DefaultIndex: Multiple definition for server_name !"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error DefaultIndex : ';' is missing at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error DefaultIndex : ';' missplaced at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(14);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        defaultIndex = valueRead;
        alreadySet = true;
    }
    if (!alreadySet)
        throw (ConfigFileError("Error DefaultIndex: No DefaultIndex set in config !"));
    infile.clear();
    infile.seekg(0, std::ios::beg);
    return (defaultIndex);
}

std::size_t parsingFunction::findMaxClientRequest(std::istream &infile) {
    std::size_t requestSize = 0;
    std::string multiplicator;
    std::size_t multiplicatorN = 1;
    std::string valueRead;
    bool    alreadySet = false;
    std::ostringstream oss;
    int line = 0;
    
    while(std::getline(infile, valueRead)) {
        line++;
        if (valueRead.find("client_max_body_size") == std::string::npos)
            continue ;
        if (alreadySet)
            throw (ConfigFileError("Error ClientMaxBodySize: Multiple definition for client_max_body_size !"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error ClientMaxBodySize : ';' not found at the end of line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error ClientMaxBodySize : ';' missplaced at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(21);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
        multiplicator = valueRead.substr(valueRead.size() - 1);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
        if (valueRead.size() == 0) {
            oss << "Error ClientMaxBodySize : Missing suffix at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        multiplicator = utils::removeIsSpaceBetween(multiplicator.c_str());
        if (multiplicator == "M")
            multiplicatorN = 1000000;
        if (multiplicator == "m")
            multiplicatorN = 1000;
        if (!utils::isOnlyDigit(valueRead.c_str()))
        {
            oss << "Error ClientMaxBodySize : Max Request size must only include numbers ! (line :" << line << ")";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (valueRead.size() + 8 > 15)
            throw (ConfigFileError("Error ClientMaxBodySize : Too long request size don't be rude ! 15 numbers max !"));
        requestSize = std::atol(valueRead.c_str());
        requestSize *= multiplicatorN;
        alreadySet = true;
    }
    infile.clear();
    infile.seekg(0, std::ios::beg);
    return (requestSize);
}

std::map<std::string, IS_ErrorPage> parsingFunction::findErrorPage(std::istream &infile) {
    std::map<std::string, IS_ErrorPage> errorPage;
    IS_ErrorPage    isError;
    std::string valueRead;
    std::string errorCode;
    std::ostringstream oss;
    int         line = 0;
    
    while (std::getline(infile, valueRead)) {
        line++;
        if (valueRead.find("error_page ") == std::string::npos)
            continue ;
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error ErrorPage : ';' not found at the end of line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error ErrorPage : ';' missplaced at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(11);
        if (valueRead.find(" ") == std::string::npos) {
            oss << "Error ErrorPage : Space is missing at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        errorCode = valueRead.substr(0, valueRead.find(" "));
        if (!utils::isOnlyDigit(errorCode.c_str()))
            throw (ConfigFileError("Error ErrorPage: i only accept numbers !"));
        isError.setErrorCode(std::atoi(errorCode.c_str()));
        valueRead = valueRead.substr(errorCode.size() + 1);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        isError.setErrorPath(valueRead);
        if (errorPage.find(errorCode) != errorPage.end()) {
            oss << "Error ErrorPage : Multiple definition for the same error : " << errorCode << " at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        errorPage[errorCode] = isError;
    }
    infile.clear();
    infile.seekg(0, std::ios::beg);
    return (errorPage);
}