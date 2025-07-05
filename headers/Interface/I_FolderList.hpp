/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   I_FolderList.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 06:03:55 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 08:41:50 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <ctime>

class I_FolderList
{
    protected:
        std::string folderMame;
        std::string suffix;
        std::size_t folderSize;
        std::string lastEdit;
    public:
        virtual ~I_FolderList() = 0;
};