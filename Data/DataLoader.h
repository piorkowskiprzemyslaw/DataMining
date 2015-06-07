#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>
#include <vector>

#include "Data/Data.h"

class DataLoader
{
private:
    class csvctype: public std::ctype<char>
    {
        mask csvTable[table_size];
    public:
        csvctype(size_t refs = 0)
            : std::ctype<char>(&csvTable[0], false, refs)
        {
            std::copy_n(classic_table(), table_size, csvTable);
            csvTable[static_cast<int8_t>(',')] = (mask)space;
        }
    };

    std::string m_fileName;
    std::string m_classHeader;
    bool m_readHeaders;

    bool fileExist(const std::string &fileName);
public:
    explicit DataLoader(const std::string &classHeader);
    bool setFileName(const std::string &fileName);
    void setReadHeaders(bool readHeaders);
    std::shared_ptr<Data> loadData();
};

