/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   I_Client.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 21:46:50 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/11 13:57:24 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>

class I_Client {
	protected:
		std::string buffer;
		std::string lastDirectoryList;
		bool 		isHeader;
		bool 		isChunked;
		bool 		complete;
		size_t 		content_length;
    public:
        virtual ~I_Client() = 0;
};