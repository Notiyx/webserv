/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilsConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlonghin <tlonghin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 06:35:59 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/02 06:41:26 by tlonghin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <NameSpace.hpp>
#include <AllException.hpp>

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

static  std::string getPathLoc(std::string valueRead, const char *keywords, const int sizeKeywords, const int line) {
    std::ostringstream oss;
    valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
    if (!isspace(valueRead[sizeKeywords]))
    {
        oss << "Error Location: " << keywords << " attributs in location error no found space after keywords at line " << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    valueRead = valueRead.substr(sizeKeywords + 1);
    if (valueRead == ";" || valueRead.size() == 0)
    {
        oss << "Error Location: " << keywords << " attributs error data not set at line" << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    if (valueRead.find(";") == std::string::npos) {
        oss << "Error Location: " << keywords << " attributs no find ';' in end of line at line " << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    if (valueRead.find(";") != valueRead.size() - 1) {
            oss << "Error Location: " << keywords << " attributs ; pas bien placer a la ligne " << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    valueRead = valueRead.substr(0, valueRead.size() - 1);
    return (valueRead);
}

static  bool    getBoolLoc(std::string valueRead, const char *keywords, const int sizeKeywords, const int line) {
    std::ostringstream oss;
    valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
    if (!isspace(valueRead[sizeKeywords]))
    {
        oss << "Error Location: " << keywords << " attribut error no found space after keywords at line " << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    valueRead = valueRead.substr(sizeKeywords + 1);
    if (valueRead == ";" || valueRead.size() == 0)
    {
        oss << "Error Location: " << keywords << " attributs error data not set at line " << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    if (valueRead.find(";") == std::string::npos) {
        oss << "Error Location: " << keywords << " attributs no find ';' in end of line at line " << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    if (valueRead.find(";") != valueRead.size() - 1)
    {
        oss << "Error Location: " << keywords << " attributs ; pas bien placer at line " << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    valueRead = valueRead.substr(0, valueRead.size() - 1);
    oss << "Error Location: " << keywords << " Enabled attributes error: the keyword is neither 'on' or 'off' but is '" << valueRead << "' at line " << line << " !";
    if (valueRead != "off" && valueRead != "on")
    {
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    bool values;
    if (valueRead == "off")
        values = false;
    if (valueRead == "on")
        values = true;
    return (values);
}

static  IS_Location setAllowedMethods(std::string valueRead, IS_Location &is, const int line) {
    std::ostringstream oss;
    valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
    if (!isspace(valueRead[13]))
    {
        oss << "Error Location: Allow Methods attributs no found space after keywords ! at line " << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    valueRead = valueRead.substr(14);
    if (valueRead == ";" || valueRead.size() == 0) {
        oss << "Error Location: Allow Methods attributs data not set at line " << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    if (valueRead.find(";") == std::string::npos) {
        oss << "Error Location: Allow Methods attribut no find ';' in end of line at line " << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    if (valueRead.find(";") != valueRead.size() - 1) {
        oss << "Error Location: Allow Methods attribut ';' pas bien placer at line " << line << " !";
        std::string error(oss.str());
        throw (ConfigFileError(error.c_str()));
    }
    valueRead = valueRead.substr(0, valueRead.size() - 1);
    std::istringstream iss(valueRead);
    std::string        words;
    while (iss >> words) {
        if (words != "GET" && words != "POST" && words != "DELETE")
        {
            oss << "Error Location: Allow Methods attribut '" << words << "' is not valid arguments for allow_methods(GET,POST,DELETE) at line " << line << " !";
            std::string error(oss.str());
            throw (ConfigFileError(error.c_str()));
        }
        is.setMethodAllow(words.c_str(), true);
    }
    return (is);
}

/// SI pas de root definis on va considerer qu'on prend le root du server donc qu'elle est heriter
/// pareil pour l'auto index mais si y'a pas de auto index alors on va dire qu'elle est sur off par defaut
/// si il n'y as pas de methods autoriser definis on considere qu'on les acceptes toutes donc qu'elles sont toutes passes a true
/// si pas de upload_enable alors il est en false
/// si pas d'index alors on dit qu'il herite de l'index server ou de un par defaut (peu probable)
static IS_Location  findDataLoc(std::istream &infile, int line) {
    IS_Location isloc;
    std::string valueRead;
    std::ostringstream ost;
    bool               Data[6] = {false, false, false, false, false, false};
    isloc.setUploadEnable(false);
    isloc.setDirectoryListing(false);
    isloc.setMethodAllow("GET", false);
    isloc.setMethodAllow("POST", false);
    isloc.setMethodAllow("DELETE", false);
    while (std::getline(infile,valueRead)) {
        line++;
        std::cout << valueRead << std::endl;
        if (valueRead.find("root") != std::string::npos) {
            try {
                if (Data[0])
                    throw (ConfigFileError("Error Location: multiple definition of root !"));
                Data[0] = true;
                valueRead = getPathLoc(valueRead, "Root", 4, line);
                isloc.setRoot(valueRead);
            } catch(const ConfigFileError& e) { throw (ConfigFileError(e));}
        } else if (valueRead.find("index") != std::string::npos) {
            try {
                if (Data[1])
                    throw (ConfigFileError("Error Location: multiple definition of index !"));
                Data[1] = true;
                valueRead = getPathLoc(valueRead, "Index", 5, line);
                isloc.setIndex(valueRead);
            } catch(const ConfigFileError& e) { throw (ConfigFileError(e));}
        } else if (valueRead.find("directoryListing") != std::string::npos) {
            try {
                if (Data[2])
                    throw (ConfigFileError("Error Location: multiple definition of directoryListing !"));
                Data[2] = true;
                bool values = getBoolLoc(valueRead, "Directory Listing", 16, line);
                isloc.setDirectoryListing(values);
            } catch(const ConfigFileError& e) { throw (ConfigFileError(e));}
        } else if (valueRead.find("upload_path") != std::string::npos) {
            try {
                if (Data[3])
                    throw (ConfigFileError("Error Location: multiple definition of upload_path !"));
                Data[3] = true;
                valueRead = getPathLoc(valueRead, "Upload Path", 11, line);
                isloc.setUploadPath(valueRead);
            } catch(const ConfigFileError& e) { throw (ConfigFileError(e));}
        } else if (valueRead.find("upload_enabled") != std::string::npos) {
            try {
                if (Data[4])
                    throw (ConfigFileError("Error Location: multiple definition of upload_enabled !"));
                Data[4] = true;
                bool values = getBoolLoc(valueRead, "Upload Enabled", 14, line);
                isloc.setUploadEnable(values);
            } catch(const ConfigFileError& e) { throw (ConfigFileError(e));}
        } else if (valueRead.find("allow_methods") != std::string::npos) {
             try {
                if (Data[5])
                    throw (ConfigFileError("Error Location: multiple definition of allow_methods !"));
                Data[5] = true;
                isloc = setAllowedMethods(valueRead, isloc, line);
            } catch(const ConfigFileError& e) { throw (ConfigFileError(e));}
        } else if (valueRead.find("}") != std::string::npos) {
            valueRead = utils::removeIsSpaceBetween(valueRead.c_str());
            if (valueRead != "}")
                throw (ConfigFileError("Error Location: The brace must be set in newline !"));
            break ;
        } else {
            throw (ConfigFileError("Error Location : keywords not founds"));
        }
    }
    if (!Data[5]) {
        isloc.setMethodAllow("GET", true);
        isloc.setMethodAllow("POST", true);
        isloc.setMethodAllow("DELETE", true);
    }
    return (isloc);
}

std::map<std::string, IS_Location> parsingFunction::findLocation(std::istream &infile) {
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
        try {
            defaultLoc = findDataLoc(infile, lines);   
        } catch (const ConfigFileError &e) {
            throw (ConfigFileError(e));
        }
        loc[path] = defaultLoc;
    }
    if (loc.size() == 0)
        throw (ConfigFileError("Error: No location found !"));
    return (loc);
}