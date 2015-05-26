#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <limits>
#include <utility>

class Data
{
private:
    std::string m_classAttributeHeader;
    int m_numberOfClasses;
    std::map<std::string, int> m_header;
    std::vector<std::vector<double>> m_dataMatrix;
    std::vector<std::pair<double, double>> m_minMaxValues;

public:
    const int PERCENTAGE_ATTRIBUTES = 54;
    const int CONTINUOUS_ATTRIBUTES = 3;

    Data(const std::string& classAttributeHeader);
    void addColumnHeader(const std::string& columnName, const int columnNumber);
    void addDataMatrixRow(std::vector<double>&& row);
    void printData();
    void computeMinMaxValues();
    void setMinMaxValues(const std::vector<std::pair<double, double>>& minMaxValues);
    const std::vector<std::pair<double, double>>& getMinMaxValues();
    void normalization();
    const std::vector<double>& getRow(size_t rowNumber) const;
    const std::string& getClassAttributeHeader() const;
    int getClassIdx();
    size_t nRow() const;
    size_t nAttributes() const;
    int getNumberOfClasses();
};

