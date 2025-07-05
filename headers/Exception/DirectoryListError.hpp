/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListError.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 08:00:29 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/02 06:01:32 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class DirectoryListError : public std::exception {
    private :
        std::string msg;
    public :
        DirectoryListError(const char *a);
        const char* what() const throw();
        ~DirectoryListError() throw();
};