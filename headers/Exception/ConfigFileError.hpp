/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileError.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 08:00:29 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 00:48:18 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class ConfigFileError : public std::exception {
    private :
        std::string msg;
    public :
        ConfigFileError(const char *a);
        const char* what() const throw();
        ~ConfigFileError() throw();
};