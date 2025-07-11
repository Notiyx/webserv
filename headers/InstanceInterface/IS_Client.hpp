/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_Client.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:44:15 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/11 18:53:51 by nmetais          ###   ########.fr       */
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
		std::string	getOldDir();
		std::string getCurrentDirectoryName();
		void setDir(std::string lastDir);
		void setOldDir(std::string oldDir);
		void setBuffer(std::string buffer);
		void setLength(size_t content_length);
		void setHeader(bool isHeader);
		void setComplete(bool complete);
		void setChunk(bool isChunk);
		void setCurrentDirectoryName(const std::string &name);
		void appendBuffer(const char *data, size_t size);
		void setEraseData();
        IS_Client();
        ~IS_Client();
};