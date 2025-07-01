/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   I_Host.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 06:03:55 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 06:04:48 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class I_Host
{
    protected:
        std::string hostAndPort;
        std::string hostName;
        int port;
    public:
        virtual ~I_Host() = 0;
};