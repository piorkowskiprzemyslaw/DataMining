#pragma once
#include <iostream>
#include <map>
#include <vector>

class Data
{
private:
    // Mapping column number -> column name
    std::map<int, std::string> m_header;
    std::vector<std::vector<double>> m_dataMatrix;
public:
    Data();
    void addColumnHeader( const int columnNumber, const std::string & columnName);
    void addDataMatrixRow(std::vector<double> && row);
    void printData();
};

