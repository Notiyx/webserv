/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:48:10 by tlonghin          #+#    #+#             */
/*   Updated: 2025/06/30 07:49:32 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

Webserv::Webserv() {}

void    Webserv::setConfig(const Config newConf) {
    this->conf = newConf;
}

Webserv::~Webserv() {}