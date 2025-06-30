/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:43:07 by tlonghin          #+#    #+#             */
/*   Updated: 2025/06/30 08:23:56 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <AllException.hpp>

class Config {
    private:
        std::map<std::string, std::string> data;
    public:
        Config();
        Config  &operator=(const Config &conf);
        void    parseConfig(const char *av);
        ~Config();
};