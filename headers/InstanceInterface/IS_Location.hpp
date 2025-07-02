/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_Location.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 04:53:14 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 05:31:36 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <Interface.hpp>

class I_Location;

class IS_Location : public I_Location
{
    private:
    public:
        void    setRoot(const std::string &root);
        void    setIndex(const std::string &index);
        void    setDirectoryListing(const bool &value);
        void    setUploadPath(const std::string &path);
        void    setUploadEnable(const bool &value);
        void    setMethodAllow(const char *types, const bool &value);
        std::string getRoot();
        std::string getIndex();
        std::string getUploadPath();
        bool        getDirectoryListing();
        bool        getUploadEnable();
        bool    getLocationGetMethod();
        bool    getLocationPostMethod();
        bool    getLocationDeleteMethod();
        IS_Location();
        ~IS_Location();
};
