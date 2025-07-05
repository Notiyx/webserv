/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:36:56 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/05 17:59:48 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>
#include <DirectoryListing.hpp>

int main(int ac, char **av) {
    DirectoryListing dir;

    dir.setListing("./");
    if (ac != 2)
    {
        std::cerr << "Error: synthax : ./webserv <*.conf>" << std::endl;
        return (1);
    }
    Webserv webserv;
    Config  conf;
    try {
        conf.parseConfig(av[1]);
    } catch(const ConfigFileError &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    webserv.setConfig(conf);
    try {
        std::cout << utils::getDateCurrent() << std::endl;
        webserv.setupServ();
        webserv.launchServ();
    } catch (const fdError &e) {
        std::cerr <<e.what() << std::endl;
        return (1);
    }
}