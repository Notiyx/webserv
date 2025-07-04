/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 06:40:37 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/04 20:20:13 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <NameSpace.hpp>
#include <AllException.hpp>
#include <cstring>
#include <errno.h>

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

bool    utils::isOnlyDigit(const char *str) {
    for (int i = 0; str[i]; ++i) {
        if (!std::isdigit(str[i]))
            return (false);
    }
    return (true);
}

bool    utils::isOnlySpace(const char *str) {
    for (int i = 0; str[i]; ++i) {
        if (!std::isspace(str[i]))
            return (false);
    }
    return (true);
}

bool    configUtils::checkIsPairChar(std::istream &infile) {
    std::string valueRead;
    int         count = 0;

    while (std::getline(infile, valueRead)) {
        for (std::string::size_type i = 0; i < valueRead.size(); ++i) {
            if (valueRead[i] == '{' || valueRead[i] == '}') {
                ++count;
            }
        }
    }
    infile.clear();
    infile.seekg(0, std::ios::beg);
    if (count % 2 != 0)
        return (false);
    return (true);
}

void    configUtils::checkValidString(std::istream &infile) {
    std::string         valueRead;
    std::ostringstream  oss;
    int         line = 0;
    while (std::getline(infile, valueRead)) {
        line++;
        if (line == 1 && valueRead.find("server {") != std::string::npos)
            continue;
        if (utils::isOnlySpace(valueRead.c_str()))
            continue;
        if (valueRead.find("location") != std::string::npos)
            while (std::getline(infile, valueRead) && valueRead != "}") { line++; }
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        if (valueRead.find("location") == std::string::npos && valueRead.find("listen") == std::string::npos &&
            valueRead.find("host") == std::string::npos && valueRead.find("server_name") == std::string::npos &&
            valueRead.find("client_max_body_size") == std::string::npos && valueRead.find("error_page") == std::string::npos
            && valueRead != "}")
        {
            oss << "Error in default config : unknow keywords '" << valueRead << "' at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
    }
    infile.clear();
    infile.seekg(0, std::ios::beg);
}

//Fonction qui protege et envois la bonne erreure si un fd fail;
void    utils::check_syscall(int ret, const char* syscall_name) {
    if (ret < 0)
        throw fdError(std::string(syscall_name) + " failed: " + strerror(errno));
};