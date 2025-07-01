/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 06:40:37 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 06:49:18 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Utils.hpp>



namespace utils {
    bool    checkEndStr(const std::string str, const std::string suffix) {
        if (str.length() < suffix.length())
            return (false);
        if (str.compare(str.length() - suffix.length(), suffix.length(), suffix) != 0)
            return (false);
        return (true);
    }
}