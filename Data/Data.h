#pragma once

#include <iostream>
#include <limits>
#include <map>
#include <ostream>
#include <set>
#include <utility>
#include <vector>
#include <memory>

#include "Logger.h"

class Data
{
public:
    using RowType = std::vector<double>;

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
            LOG(WARNING) << "Incompatible number of attributes";
        }

        m_dataMatrix.emplace_back(std::forward<T>(row));
    }

    friend std::ostream & operator<<(std::ostream &, const Data &);
    void printData() const;

    void computeMinMaxValues();
    void setMinMaxValues(const std::vector<std::pair<double, double>>& minMaxValues);
    const std::vector<std::pair<double, double>>& getMinMaxValues() const;

    void normalization();

    const RowType& getRow(size_t rowNumber) const;
    // Iterate over rows
    auto begin() const -> decltype(m_dataMatrix.begin()) { return m_dataMatrix.begin(); }
    auto end() const -> decltype(m_dataMatrix.end()) { return m_dataMatrix.end(); }
    auto begin() -> decltype(m_dataMatrix.begin()) { return m_dataMatrix.begin(); }
    auto end() -> decltype(m_dataMatrix.end()) { return m_dataMatrix.end(); }
    auto cbegin() const -> decltype(m_dataMatrix.cbegin()) { return m_dataMatrix.cbegin(); }
    auto cend() const -> decltype(m_dataMatrix.cend()) { return m_dataMatrix.cend(); }

    const std::string& getClassAttributeHeader() const;
    int getClassIdx() const;
    size_t nRow() const;
    size_t nAttributes() const;

    const std::set<int> getClassesValues() const;
    int getNumberOfClasses() const;
    const std::vector<double> getClassProbability() const;
    void computeParameters();
};

