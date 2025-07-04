/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SetupServError.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 05:33:15 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/04 05:54:16 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class SetupServError : public std::exception {
    private :
        std::string msg;
    public :
		SetupServError(const std::string& msg);
        const char* what() const throw();
        ~SetupServError() throw();
};