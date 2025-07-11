/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_Client.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:44:15 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/11 14:04:46 by nmetais          ###   ########.fr       */
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
		bool getComplete();
		std::string getBuffer();
		std::string getDir();
		void setDir(std::string lastDir);
		void setBuffer(std::string buffer);
		void setLength(size_t content_length);
		void setHeader(bool isHeader);
		void setComplete(bool complete);
		void setChunk(bool isChunk);
		void appendBuffer(const char *data, size_t size);
        IS_Client();
        ~IS_Client();
};