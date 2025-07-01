/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_ErrorPage.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:44:15 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 08:49:03 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <Interface.hpp>

class IS_ErrorPage : public I_ErrorPage
{
    private:
    public:
        void    setErrorPath(const std::string &path);
        void    setErrorCode(const int &err);
        std::string getErrorPath();
        int         getErrorCode();
        IS_ErrorPage();
        ~IS_ErrorPage();
};