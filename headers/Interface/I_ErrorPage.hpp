/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   I_ErrorPage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:44:15 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 05:52:52 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class I_ErrorPage
{
    protected:
        int    errorCode;
        std::string path;
    public:
        virtual ~I_ErrorPage() = 0;
};