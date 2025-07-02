/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   I_Location.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 04:53:14 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/02 03:09:19 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
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
    public:
        virtual ~I_Location() = 0;
};
