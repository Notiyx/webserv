/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:36:56 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/02 06:00:04 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

int main(int ac, char **av) {
    if (ac != 2)
    {
        std::cerr << "Error: synthax : ./webserv <*.conf>" << std::endl;
        return (1);
    }
    Webserv webserv;
    Config  conf;
    try
    {
        conf.parseConfig(av[1]);
    }
    catch(const ConfigFileError &e)
    {
        std::cerr << e.what() << '\n';
    }
    webserv.setConfig(conf);
}