#pragma once
#include <iostream>
#include <map>

class Data
{
private:
    // Mapping column name -> column number
    std::map<std::string, int> header;

public:
    Data();
};

