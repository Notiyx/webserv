/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Getter.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:34:35 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/09 20:12:19 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <InstanceInterface.hpp>

std::string    IS_Location::getRoot() {
    return (this->root);
}

std::string    IS_Location::getIndex() {
    return (this->index);
}

std::string     IS_Location::getUploadPath() {
    return (this->uploadPath);
}

int IS_Location::getCodeRedirect() {
    std::map<int, std::string>::iterator ite = this->redirectData.end();
    for (std::map<int, std::string>::iterator it = this->redirectData.begin(); it != ite; ++it) {
        if (it->first >= 0)
            return (it->first);
    }
    return (-1);
}

std::string IS_Location::getPathRedirect() {
    std::map<int, std::string>::iterator ite = this->redirectData.end();
    for (std::map<int, std::string>::iterator it = this->redirectData.begin(); it != ite; ++it) {
        if (it->second.size() > 0)
            return (it->second);
    }
    return ("");
}

bool    IS_Location::getDirectoryListing() {
    return (this->directoryListing);
}
bool    IS_Location::getUploadEnable() {
    return (this->uploadEnable);
}

bool    IS_Location::getLocationGetMethod() {
    return (this->methodsAllow.getGetMethod());
}
bool    IS_Location::getLocationPostMethod() {
    return (this->methodsAllow.getPostMethod());
}
bool    IS_Location::getLocationDeleteMethod() {
    return (this->methodsAllow.getDeleteMethod());
}


bool    IS_MethodsAllow::getGetMethod() {
    return (this->GET);
}
bool    IS_MethodsAllow::getPostMethod() {
    return (this->POST);
}
bool    IS_MethodsAllow::getDeleteMethod() {
    return (this->DELETE);
}

std::string IS_Listen::getListenHostAndPort() {
    return (this->hostAndPort);
}
std::string IS_Listen::getListenHostname() {
    return (this->hostName);
}
int         IS_Listen::getListenPort() {
    return (this->port);   
}

std::string IS_Host::getHostAndPort() {
    return (this->hostAndPort);
}
std::string IS_Host::getHostname() {
    return (this->hostName);
}
int         IS_Host::getPort() {
    return (this->port);   
}

std::string IS_ErrorPage::getErrorPath() {
    return (this->path);
}
int         IS_ErrorPage::getErrorCode() {
    return (this->errorCode);
}

std::string IS_FolderList::getFolderName() {
    return (this->folderMame);
}

double IS_FolderList::getFolderSize() {
    return (this->folderSize);
}

std::string IS_FolderList::getFolderLastEdit() {
    return (this->lastEdit);
}

std::string IS_FolderList::getFolderSuffix() {
    return (this->suffix);
}

std::string IS_Client::getBuffer() {
    return (this->buffer);
}

bool IS_Client::getHeader() {
    return (this->isHeader);
}

bool IS_Client::getChunk() {
    return (this->isChunked);
}

size_t IS_Client::getLength() {
    return (this->content_length);
}