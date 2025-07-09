/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   I_Location.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 04:53:14 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/09 19:50:28 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <IS_MethodsAllow.hpp>

class IS_MethodsAllow;

class I_Location
{
    protected:
        std::string root;
        std::string index;
        std::string uploadPath;
        bool        directoryListing;
        bool        uploadEnable;
        IS_MethodsAllow methodsAllow;
        std::map<int, std::string> redirectData;
    public:
        virtual ~I_Location() = 0;
};
