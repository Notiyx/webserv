/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 06:40:37 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/01 09:38:48 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <NameSpace.hpp>
#include <AllException.hpp>

std::string utils::removeIsSpaceBetween(const char *str) {
    std::string nString;
    int i = 0;
    while (str[i] && isspace(str[i]))
        i++;
    while (str[i]) 
        nString += str[i++];
    return (nString);
}

bool    utils::checkEndStr(const std::string str, const std::string suffix) {
    if (str.length() < suffix.length())
        return (false);
    if (str.compare(str.length() - suffix.length(), suffix.length(), suffix) != 0)
        return (false);
    return (true);
}

bool    configUtils::checkIsPairChar(std::istream &infile) {
    std::string valueRead;
    int         count = 0;

    while (std::getline(infile, valueRead)) {
        for (std::string::size_type i = 0; i < valueRead.size(); ++i) {
            if (valueRead[i] == '{' || valueRead[i] == '}') {
                ++count;
            }
        }
    }
    infile.clear();
    infile.seekg(0, std::ios::beg);
    if (count % 2 != 0)
        return (false);
    return (true);
}

static std::string  keepToNextIsSpace(const char *str) {
    std::size_t i = 0;
    std::string nstr;
    
    while (str[i] && !isspace(str[i]))
        nstr += str[i++];
    return (nstr);
}

/// SI pas de root definis on va considerer qu'on prend le root du server donc qu'elle est heriter
/// pareil pour l'auto index mais si y'a pas de auto index alors on va dire qu'elle est sur off par defaut
/// si il n'y as pas de methods autoriser definis on considere qu'on les acceptes toutes donc qu'elles sont toutes passes a true
/// si pas de upload_enable alors il est en false
/// si pas d'index alors on dit qu'il herite de l'index server ou de un par defaut (peu probable)
static IS_Location  findDataLoc(std::istream &infile) {
    IS_Location isloc;
    std::string valueRead;
    bool    Data[5] = {false, false, false, false, false};
    (void) infile;
    (void) Data;
    return (isloc);
}

std::map<std::string, IS_Location> configUtils::findLocation(std::istream &infile) {
    std::string valueRead;
    std::string path;
    std::ostringstream conv;
    std::size_t lines = 0;
    std::map<std::string, IS_Location> loc;
    IS_Location defaultLoc;

    while (std::getline(infile, valueRead)) {
        lines++;
        if (valueRead.find("location") == std::string::npos)
            continue ;
        valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
        valueRead = valueRead.substr(8);
        if (!isspace(valueRead[0]))
        {
            conv << "Error: The location at line " << lines << " is not valid!";
            std::string error(conv.str());
            throw (ConfigFileError(error.c_str()));
        }
        valueRead = valueRead.substr(1);
        path = keepToNextIsSpace(valueRead.c_str());
        std::cout << path << std::endl;
        defaultLoc = findDataLoc(infile);
    }
    if (loc.size() == 0)
        throw (ConfigFileError("Error: No location found !"));
    return (loc);
}