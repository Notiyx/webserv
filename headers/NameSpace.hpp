/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NameSpace.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:19:18 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/06 19:14:28 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <InstanceInterface.hpp>
#include <map>
#include <sys/stat.h>
#include <sys/socket.h>
#include <ctime>


namespace utils
{
    bool        checkEndStr(const std::string str, const std::string suffix);
    std::string removeIsSpaceBetween(const char *str);
    bool        isOnlyDigit(const char *str);
    void        check_syscall(int ret, const char* syscall_name);
    bool        isOnlySpace(const char *str);
    void        sender(int client_fd, std::string res);
    std::string trim(const std::string& str);
    std::string getDateCurrent();
    bool        fileExist(std::string path);
    bool        isFile(const std::string& path);
    bool        isDirectory(const std::string& path);
} // namespace utils

namespace configUtils
{
    bool    checkIsPairChar(std::istream &infile); //va lire le fichier et renvoyer si y'a bien un nombre pair d'accolade
    void    checkValidString(std::istream &infile);
} // namespace config_utils

namespace parsingFunction
{
    IS_Listen   findListen(std::istream &infile);
    IS_Host   findHost(std::istream &infile);
    std::string findServerName(std::istream &infile);
    std::size_t findMaxClientRequest(std::istream &infile);
    std::map<std::string, IS_Location> findLocation(std::istream &infile);
    std::map<std::string, IS_ErrorPage> findErrorPage(std::istream &infile);
} // namespace parsingFunction
