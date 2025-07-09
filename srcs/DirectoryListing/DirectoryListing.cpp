/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:34:23 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/09 21:36:42 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <DirectoryListing.hpp>
#include <AllException.hpp>

DirectoryListing::DirectoryListing() {}

void    DirectoryListing::setListing(const std::string &path) {
    std::string suffix;
    DIR *dir = opendir(path.c_str());

    if (dir == NULL) {
        throw (DirectoryListError("Error Directory Listing : opendir failed or not find path !"));
    }
    
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL) {
        std::string dName(entry->d_name);
        if (dName == ".")
            continue ;
        std::string filePath = path + entry->d_name;
        struct stat st;
        if (stat(filePath.c_str(), &st) == 0) {
            this->listing[filePath].setFolderName(entry->d_name);
            if (st.st_size < 1000) {
                this->listing[filePath].setFolderSize(st.st_size);
                this->listing[filePath].setFolderSuffix("bytes");
            } else if (st.st_size < 1000000) {
                double sizeKB = st.st_size / 1000.0;
                this->listing[filePath].setFolderSize(sizeKB);
                this->listing[filePath].setFolderSuffix("KB");
            } else if (st.st_size < 1000000000) {
                double sizeMB = st.st_size / 1000000.0;
                this->listing[filePath].setFolderSize(sizeMB);
                this->listing[filePath].setFolderSuffix("MB");
            } else {
                double sizeGB = st.st_size / 1000000000.0;
                this->listing[filePath].setFolderSize(sizeGB);
                this->listing[filePath].setFolderSuffix("GB");
            }
            char buff[20];
            std::tm* local_time = std::localtime(&st.st_mtime);
            std::strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", local_time);
            std::string formatTime(buff);
            this->listing[filePath].setLastEdit(formatTime);
        }
    }
    // for (std::map<std::string, IS_FolderList>::iterator it = this->listing.begin(); it != this->listing.end(); ++it) {
    //     std::cout << "[" << it->second.getFolderName() << "]" << " : " << it->second.getFolderSize() << " " << it->second.getFolderSuffix() << " " << it->second.getFolderLastEdit() << std::endl; 
    // }
}

std::map<std::string, IS_FolderList> DirectoryListing::getListing() {
    return (this->listing);
}

DirectoryListing::~DirectoryListing() {}