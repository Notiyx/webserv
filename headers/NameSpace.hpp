/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NameSpace.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:19:18 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/02 08:51:24 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <InstanceInterface.hpp>
#include <map>

namespace utils
{
    bool        checkEndStr(const std::string str, const std::string suffix);
    std::string removeIsSpaceBetween(const char *str);
    bool        isOnlyDigit(const char *str);
} // namespace utils

namespace configUtils
{
    bool    checkIsPairChar(std::istream &infile); //va lire le fichier et renvoyer si y'a bien un nombre pair d'accolade
} // namespace config_utils

namespace parsingFunction
{
    IS_Listen   findListen(std::istream &infile);
    IS_Host   findHost(std::istream &infile);
    std::string findServerName(std::istream &infile);
    std::size_t findMaxClientRequest(std::istream &infile);
    std::map<std::string, IS_Location> findLocation(std::istream &infile);
    std::map<std::string, IS_ErrorPage> findErrorPage(std::istream &infile);
} // namespace name
