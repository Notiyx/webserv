/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigListenHost.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 07:28:13 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/09 19:42:20 by tlonghin         ###   ########.fr       */
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
            throw (ConfigFileError("Error Listen: Multiple defintion for listen !"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (!isspace(valueRead[6]))
        {
            oss << "Error Listen: no found space after keywords at line " << line << " !";
            std::string error(oss.str()); 
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(7);
        if (valueRead.find(":") == std::string::npos) {
            oss << "Error Listen: caracter ':' missing in listen for separate hostname and port at line " << line << " !";
            std::string error(oss.str()); 
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error Listen : not found ';' in end of line at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error Listen : ';' pas bien placer at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        host = valueRead.substr(0, valueRead.find(":"));
        port = valueRead.substr(valueRead.find(":") + 1, valueRead.size());
        port = port.substr(0, port.size() - 1);
        if (!utils::isOnlyDigit(port.c_str()))
        {
            oss << "Error Listen : The port of listen must be include only numbers ! (line :" << line << ")";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (port.size() > 8) {
            oss << "Error Listen : The port have max 8 numbers ! (line :" << line << ")";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(0, valueRead.find(";"));
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
            throw (ConfigFileError("Error Host: Multiple defintion for Host !"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        valueRead = valueRead.substr(5);
        if (valueRead.find(":") == std::string::npos) {
            oss << "Error Host: caracter ':' missing in listen for separate hostname and port at line " << line << " !";
            std::string error(oss.str()); 
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error Host : not found ';' in end of line at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error Host : ';' pas bien placer at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        host = valueRead.substr(0, valueRead.find(":"));
        port = valueRead.substr(valueRead.find(":") + 1, valueRead.size());
        port = port.substr(0, port.size() - 1);
        if (!utils::isOnlyDigit(port.c_str()))
        {
            oss << "Error Host : The port of listen must be include only numbers ! (line :" << line << ")";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (port.size() > 8) {
            oss << "Error Host : The port have max 8 numbers ! (line :" << line << ")";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(0, valueRead.find(";"));
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
            throw (ConfigFileError("Error ServerName: Multiple defintion for server_name !"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error ServerName : no find ';' in end of line at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error ServerName : ';' pas bien placer at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(12);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
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
            throw (ConfigFileError("Error DefaultRoot: Multiple defintion for server_name !"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error DefaultRoot : no find ';' in end of line at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error DefaultRoot : ';' pas bien placer at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(14);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
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
            throw (ConfigFileError("Error DefaultIndex: Multiple defintion for server_name !"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error DefaultIndex : no find ';' in end of line at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error DefaultIndex : ';' pas bien placer at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(14);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
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
            throw (ConfigFileError("Error ClientMaxBodySize: Multiple defintion for client_max_body_size !"));
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (valueRead.find(";") == std::string::npos)
        {
            oss << "Error ClientMaxBodySize : no find ';' in end of line at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error ClientMaxBodySize : ';' pas bien placer at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(21);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
        multiplicator = valueRead.substr(valueRead.size() - 1);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
        if (valueRead.size() == 0) {
            oss << "Error ClientMaxBodySize : Il manque un suffix 'm' ou 'M at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (multiplicator == "M")
            multiplicatorN = 1000000;
        if (multiplicator == "m")
            multiplicatorN = 1000;
        if (!utils::isOnlyDigit(valueRead.c_str()))
        {
            oss << "Error ClientMaxBodySize : The max request size must be include only numbers ! (line :" << line << ")";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.size() + 8 > 15)
            throw (ConfigFileError("Error ClientMaxBodySize : Too longer request size plus only < 15 numbers !"));
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
            oss << "Error ErrorPage : no find ';' in end of line at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        if (valueRead.find(";") != valueRead.size() - 1)
        {
            oss << "Error ErrorPage : ';' pas bien placer at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(11);
        if (valueRead.find(" ") == std::string::npos) {
            oss << "Error ErrorPage : no found space for delimit error and path at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        errorCode = valueRead.substr(0, valueRead.find(" "));
        if (!utils::isOnlyDigit(errorCode.c_str()))
            throw (ConfigFileError("Error ErrorPage: the error accept only numbers !"));
        isError.setErrorCode(std::atoi(errorCode.c_str()));
        valueRead = valueRead.substr(errorCode.size() + 1);
        valueRead = valueRead.substr(0, valueRead.size() - 1);
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