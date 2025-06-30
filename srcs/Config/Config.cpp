/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:46:38 by tlonghin          #+#    #+#             */
/*   Updated: 2025/06/30 08:58:36 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Config.hpp>

Config::Config() {}

void    Config::parseConfig(const char *av) {
    std::ifstream infile(av);
    std::string   valueRead;
    if (!infile.is_open())
        throw ConfigFileError("Error: error during open file");
    while (std::getline(infile, valueRead)) {
        std::cout << valueRead << std::endl;
    } 
}

Config  &Config::operator=(const Config &conf) {
    if (this != &conf)
        this->data.insert(conf.data.begin(), conf.data.end());
    return (*this);
}

Config::~Config() {}