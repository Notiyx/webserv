/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 23:41:36 by nmetais           #+#    #+#             */
/*   Updated: 2025/07/05 05:17:58 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>

class HTTPResponse {
	private:
		std::string msg;
		int			res;
	public:
		HTTPResponse();
		HTTPResponse(int res, std::string msg);
		~HTTPResponse();
		std::string buildResponse();
		std::string buildGet(std::string filename);
		void send(int client_fd);
		void send(std::string filename, int client_fd);

};