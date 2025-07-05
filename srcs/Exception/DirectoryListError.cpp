/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListError.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:59:02 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/05 16:51:36 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <DirectoryListError.hpp>

DirectoryListError::DirectoryListError(const char *a) : msg(a) {}

const char* DirectoryListError::what() const throw() {
    return msg.c_str();
}

DirectoryListError::~DirectoryListError() throw() {} 
