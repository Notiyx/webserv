/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Setter.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:34:35 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 08:49:23 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <InstanceInterface.hpp>

void    IS_Location::setRoot(const std::string &root) {
    this->root = root;
}
void    IS_Location::setIndex(const std::string &index) {
    this->index = index;
}
void    IS_Location::setAutoIndex(const bool &value) {
    this->autoIndex = value;
}
void    IS_Location::setuploadEnable(const bool &value) {
    this->uploadEnable = value;
}

void    IS_Location::setMethodAllow(const char *types, const bool &value) {
    std::string compare(types);
    if (types == "GET")
        this->methodsAllow.setGetMethod(value);
    if (types == "POST")
        this->methodsAllow.setPostMethod(value);
    if (types == "DELETE")
        this->methodsAllow.setDeleteMethod(value);
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