/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:06:59 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/05 18:04:33 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <dirent.h>
#include <sys/stat.h>
#include <InstanceInterface.hpp>
#include <ctime>

class DirectoryListing
{
private:
    std::map<std::string, IS_FolderList> listing;
public:
    DirectoryListing();
    std::map<std::string, IS_FolderList> getListing();
    void    setListing(const std::string &path);
    ~DirectoryListing();
};
