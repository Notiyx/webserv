/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdError.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 05:36:55 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/04 19:23:36 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fdError.hpp>
#include <iostream>


fdError::fdError(const std::string& msg) : msg(msg) {};

const char* fdError::what() const throw() {
    return msg.c_str();
}

fdError::~fdError() throw() {};

