/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:59:02 by tlonghin          #+#    #+#             */
/*   Updated: 2025/06/30 08:10:59 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ConfigFileError.hpp>

ConfigFileError::ConfigFileError(const char *a) {
    std::string nStr(a);
    this->msg = nStr;
}

const char* ConfigFileError::what() const throw() {
    return msg.c_str();
}

ConfigFileError::~ConfigFileError() throw() {} 
