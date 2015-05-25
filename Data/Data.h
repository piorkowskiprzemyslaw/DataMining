#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <limits>
#include <utility>

class Data
{
private:
    std::string m_classAttributeHeader;
    // Mapping column name -> column number
    std::map<std::string, int> m_header;
    std::vector<std::vector<double>> m_dataMatrix;
    std::vector<std::pair<double, double>> m_minMaxValues;

public:
    Data(const std::string& classAttributeHeader);
    void addColumnHeader(const std::string& columnName, const int columnNumber);
    void addDataMatrixRow(std::vector<double>&& row);
    void printData();
    void computeMinMaxValues();
    void setMinMaxValues(const std::vector<std::pair<double, double>>& minMaxValues);
    const std::vector<std::pair<double, double>>& getMinMaxValues();
    void minMaxNormalization();
    const std::vector<double>& getRow(size_t rowNumber) const;
    const std::string& getClassAttributeHeader() const;
    int getClassIdx();
    size_t nRow() const;
    size_t nAttributes() const;
};

