/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NameSpace.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:19:18 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 21:22:16 by tlonghin         ###   ########.fr       */
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
} // namespace utils

namespace configUtils
{
    bool    checkIsPairChar(std::istream &infile); //va lire le fichier et renvoyer si y'a bien un nombre pair d'accolade
    std::map<std::string, IS_Location> findLocation(std::istream &infile);
} // namespace config_utils
