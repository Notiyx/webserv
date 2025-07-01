/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_Host.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:44:15 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 05:50:18 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <Interface.hpp>

class IS_Host : public I_Host
{
    private:
    public:
        void    setHostAndPort(const std::string &str);
        void    setHostname(const std::string &str);
        void    setPort(const int &por);
        std::string getHostAndPort();
        std::string getHostname();
        int         getPort();
        IS_Host();
        ~IS_Host();
};