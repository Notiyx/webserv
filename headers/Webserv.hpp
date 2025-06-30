/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:35:47 by tlonghin          #+#    #+#             */
/*   Updated: 2025/06/30 08:06:59 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <Config.hpp>
#include <AllException.hpp>

class Config;
class Exception;

class Webserv {
    private :
        Config conf;
    public :
    Webserv();
    void    setConfig(const Config newConf);
    ~Webserv();
};