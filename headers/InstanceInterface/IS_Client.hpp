/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_Client.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:44:15 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/09 02:23:07 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <Interface.hpp>

class IS_Client : public I_Client
{
    private:
    public:
		bool getHeader();
		bool getChunk();
		size_t getLength();
		std::string getBuffer();
		std::string setBuffer(std::string buffer);
		size_t setLength(size_t content_length);
		bool setHeader(bool isHeader);
		bool setChunk(bool isChunk);
		std::string appendBuffer(const char *data, size_t size);
        IS_Client();
        ~IS_Client();
};