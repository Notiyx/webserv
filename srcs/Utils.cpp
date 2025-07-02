/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 06:40:37 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/02 06:35:57 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <NameSpace.hpp>
#include <AllException.hpp>

std::string utils::removeIsSpaceBetween(const char *str) {
    std::string nString;
    int i = 0;
    while (str[i] && isspace(str[i]))
        i++;
    while (str[i]) 
        nString += str[i++];
    return (nString);
}

bool    utils::checkEndStr(const std::string str, const std::string suffix) {
    if (str.length() < suffix.length())
        return (false);
    if (str.compare(str.length() - suffix.length(), suffix.length(), suffix) != 0)
        return (false);
    return (true);
}