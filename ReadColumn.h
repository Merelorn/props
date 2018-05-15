#ifndef READCOLUMN_H
#define READCOLUMN_H

#include <string>

std::string ReadColumn (const std::string & line, int column, std::string delim = " ");
float ReadColumnP (const std::string & line, int column, std::string delim = " ");

#endif
