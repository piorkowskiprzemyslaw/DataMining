#pragma once
#include <iostream>
#include <unistd.h>
#include <locale>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>
#include <fstream>

#include "Data/Data.h"

class DataLoader
{
private:
    class csvctype: public std::ctype<char> {
        mask csvTable[table_size];
    public:
        csvctype(size_t refs = 0): std::ctype<char>(&csvTable[0], false, refs) {
            std::copy_n(classic_table(), table_size, csvTable);
            csvTable[','] = (mask)space;
        }
    };

    std::string m_fileName;
    bool m_readHeaders;

    bool fileExist(const std::string & fileName);
public:
    DataLoader();
    bool setFileName(const std::string & fileName);
    void setReadHeaders(bool readHeaders);
    void loadData();
};

