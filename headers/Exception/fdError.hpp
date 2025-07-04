/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdError.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 05:33:15 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/04 19:20:38 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class fdError : public std::exception {
    private :
        std::string msg;
    public :
		fdError(const std::string& msg);
        const char* what() const throw();
        ~fdError() throw();
};