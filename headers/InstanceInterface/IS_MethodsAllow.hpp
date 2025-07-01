/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_MethodsAllow.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 04:58:41 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 05:33:00 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "I_MethodsAllow.hpp"


class IS_MethodsAllow : public I_MethodsAllow
{
    private:
    
    public:
        void    setGetMethod(const bool &value);
        void    setPostMethod(const bool &value);
        void    setDeleteMethod(const bool &value);
        bool    getGetMethod();
        bool    getPostMethod();
        bool    getDeleteMethod();
        IS_MethodsAllow();
        ~IS_MethodsAllow();
};