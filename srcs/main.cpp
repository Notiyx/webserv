/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:36:56 by tlonghin          #+#    #+#             */
/*   Updated: 2025/06/30 07:39:42 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

int main(int ac, char **av) {
    if (ac != 2)
    {
        std::cerr << "Error: synthax : ./webserv <*.conf>" << std::endl;
        return (1);
    }
    (void) av;
}