/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   I_MethodsAllow.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 04:58:41 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 05:52:52 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>


class I_MethodsAllow
{
    protected:
        bool    GET;
        bool    POST;
        bool    DELETE;
    public:
        virtual ~I_MethodsAllow() = 0;
};