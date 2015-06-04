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
    int m_numberOfClasses = -1;
    std::map<std::string, int> m_header;
    std::vector<std::vector<double>> m_dataMatrix;
    std::vector<std::pair<double, double>> m_minMaxValues;
    std::set<int> m_classValues;
    std::vector<double> m_classProbability;

    void computeClassesValues();
    void computeNumberOfClasses();
    void computeClassesProbability();

public:
    static constexpr const auto PERCENTAGE_ATTRIBUTES = 54u;
    static constexpr const auto CONTINUOUS_ATTRIBUTES =  3u;

    explicit Data(const std::string& classAttributeHeader);
    void addColumnHeader(const std::string& columnName, const int columnNumber);

    template <typename T>
    void addDataMatrixRow(T &&row)
    {
        if(row.size() != m_header.size()) {
            std::cerr << "Incompatible number of attributes" << std::endl;
        }

        m_dataMatrix.emplace_back(std::forward<T>(row));
    }

    void printData() const;
    void computeMinMaxValues();
    void setMinMaxValues(const std::vector<std::pair<double, double>>& minMaxValues);
    const std::vector<std::pair<double, double>>& getMinMaxValues();
    void normalization();
    const std::vector<double>& getRow(size_t rowNumber) const;
    const std::string& getClassAttributeHeader() const;
    int getClassIdx() const;
    size_t nRow() const;
    size_t nAttributes() const;
    const std::set<int> getClassesValues() const;
    int getNumberOfClasses() const;
    const std::vector<double> getClassProbability() const;
    void computeParameters();
};

