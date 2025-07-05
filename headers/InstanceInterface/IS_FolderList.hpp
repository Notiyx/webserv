/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_FolderList.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:44:15 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 05:50:18 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <ctime>
#include <Interface.hpp>

class IS_FolderList : public I_FolderList
{
    private:
    public:
        void    setFolderName(const std::string &str);
        void    setFolderSize(const std::size_t &size);
        void    setLastEdit(const std::string &time);
        void    setFolderSuffix(const std::string &suffix);
        std::string getFolderName();
        std::size_t getFolderSize();
        std::string getFolderLastEdit();
        std::string getFolderSuffix();
        IS_FolderList();
        ~IS_FolderList();
};