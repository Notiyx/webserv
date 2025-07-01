/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Getter.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:34:35 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 08:55:05 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <InstanceInterface.hpp>

std::string    IS_Location::getRoot() {
    return (this->root)
}
std::string    IS_Location::getIndex() {
    return (this->index)
}
bool    IS_Location::getAutoIndex() {
    return (this->autoIndex);
}
bool    IS_Location::getuploadEnable() {
    return (this->uploadEnable);
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