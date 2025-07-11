/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Setter.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:34:35 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/11 14:04:05 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <InstanceInterface.hpp>

void    IS_Location::setRoot(const std::string &root) {
    this->root = root;
}
void    IS_Location::setIndex(const std::string &index) {
    this->index = index;
}

void    IS_Location::setUploadPath(const std::string &path) {
    this->uploadPath = path;
}

void    IS_Location::setUploadEnable(const bool &value) {
    this->uploadEnable = value;
}

void    IS_Location::setDirectoryListing(const bool &value) {
    this->directoryListing = value;
}

void    IS_Location::setMethodAllow(const char *types, const bool &value) {
    std::string compare(types);
    if (compare == "GET")
        this->methodsAllow.setGetMethod(value);
    if (compare == "POST")
        this->methodsAllow.setPostMethod(value);
    if (compare == "DELETE")
        this->methodsAllow.setDeleteMethod(value);
}

void    IS_Location::setRedirectData(const int code, const std::string &path) {
    this->redirectData[code] = path;
}

void    IS_MethodsAllow::setGetMethod(const bool &value) {
    this->GET = value;
}
void    IS_MethodsAllow::setPostMethod(const bool &value) {
    this->POST = value;
}
void    IS_MethodsAllow::setDeleteMethod(const bool &value) {
    this->DELETE = value;
}

void    IS_Listen::setListenHostAndPort(const std::string &str) {
    this->hostAndPort = str;
}

void    IS_Listen::setListenHostname(const std::string &str) {
    this->hostName = str;
}
void    IS_Listen::setListenPort(const int &por) {
    this->port = por;
}

void    IS_Host::setHostAndPort(const std::string &str) {
    this->hostAndPort = str;
}

void    IS_Host::setHostname(const std::string &str) {
    this->hostName = str;
}
void    IS_Host::setPort(const int &por) {
    this->port = por;
}

void    IS_ErrorPage::setErrorPath(const std::string &path) {
    this->path = path;
}
void    IS_ErrorPage::setErrorCode(const int &err) {
    this->errorCode = err;
}

void    IS_FolderList::setFolderName(const std::string &str) {
    this->folderMame = str;
}

void     IS_FolderList::setFolderSize(const double &size) {
    this->folderSize = size;
}

void    IS_FolderList::setLastEdit(const std::string &time) {
    this->lastEdit = time;
}

void    IS_FolderList::setFolderSuffix(const std::string &suffix) {
    this->suffix = suffix;
}

void    IS_Client::setLength(size_t content_length) {
    this->content_length = content_length;
}

void    IS_Client::setHeader(bool isHeader) {
    this->isHeader = isHeader;
}

void    IS_Client::setChunk(bool isChunk) {
    this->isChunked = isChunk;
}

void IS_Client::setBuffer(std::string buffer) {
    this->buffer = buffer;
}

void IS_Client::appendBuffer(const char *data, size_t size) {
    this->buffer.append(data, size);
};

void IS_Client::setComplete(bool complete) {
    this->complete = complete;
};

void IS_Client::setDir(std::string lastDir) {
    this->lastDirectoryList = lastDir;
};