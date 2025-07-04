/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SetupServError.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 05:36:55 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/04 05:54:20 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SetupServError.hpp>

SetupServError::SetupServError(const std::string& msg) : msg(msg) {};

const char* SetupServError::what() const throw() {
    return msg.c_str();
}

SetupServError::~SetupServError() throw() {} 
