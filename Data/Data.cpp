#include "Data.h"

Data::Data(const std::string &classAttributeHeader) {
    m_classAttributeHeader = classAttributeHeader;
    m_numberOfClasses = -1;
}

void Data::addColumnHeader(const std::string &columnName, const int columnNumber) {
    m_header.emplace(columnName, columnNumber);
}

void Data::addDataMatrixRow(std::vector<double> &&row) {
    if(row.size() != m_header.size())
        std::cerr << "incompatible number of attributes" << std::endl;
    m_dataMatrix.emplace_back(row);
}

void Data::printData() {
    for(auto entry : m_header) {
        std::cout << entry.first << " : " << entry.second << std::endl;
    }

    for(auto row : m_dataMatrix) {
        for(auto attr : row) {
            std::cout << attr << " ";
        }
        std::cout << std::endl;
    }
}

void Data::computeMinMaxValues() {
    double actualVal;
    // vector of (min;max) values; one per attributes
    m_minMaxValues = std::vector<std::pair<double,double>>(CONTINUOUS_ATTRIBUTES,
            std::make_pair(std::numeric_limits<double>::max(), std::numeric_limits<double>::min()));
    for(size_t i = 0; i < m_dataMatrix.size(); ++i) {
        for(size_t j = 0; j < CONTINUOUS_ATTRIBUTES; ++j) {
            actualVal = m_dataMatrix[i][j+PERCENTAGE_ATTRIBUTES];
            if(actualVal < m_minMaxValues[j].first) {
                m_minMaxValues[j].first = actualVal;
            }
            if(actualVal > m_minMaxValues[j].second) {
                m_minMaxValues[j].second = actualVal;
            }
        }
    }
}

void Data::setMinMaxValues(const std::vector<std::pair<double, double>> &minMaxValues) {
    m_minMaxValues = minMaxValues;
}

const std::vector<std::pair<double, double>>& Data::getMinMaxValues() {
    return m_minMaxValues;
}

void Data::normalization() {
    for(size_t i = 0; i < m_dataMatrix.size(); ++i) {
        for(size_t j = 0; j < m_dataMatrix[i].size(); ++j) {
            if(j < PERCENTAGE_ATTRIBUTES) {
                m_dataMatrix[i][j] /= 100;
            } else if(j < PERCENTAGE_ATTRIBUTES+CONTINUOUS_ATTRIBUTES) {
                m_dataMatrix[i][j] = (m_dataMatrix[i][j] - m_minMaxValues[j-PERCENTAGE_ATTRIBUTES].first) /
                        (m_minMaxValues[j-PERCENTAGE_ATTRIBUTES].second - m_minMaxValues[j-PERCENTAGE_ATTRIBUTES].first);
            }
        }
    }
}

const std::vector<double>& Data::getRow(size_t rowNumber) const {
    return m_dataMatrix[rowNumber];
}

size_t Data::nRow() const {
    return m_dataMatrix.size();
}

size_t Data::nAttributes() const {
    return m_dataMatrix[0].size();
}

const std::string& Data::getClassAttributeHeader() const {
    return m_classAttributeHeader;
}

int Data::getClassIdx() {
    return m_header[m_classAttributeHeader];
}

int Data::getNumberOfClasses() {
    if(m_numberOfClasses != -1)
        return m_numberOfClasses;
    std::set<int> classes;

    for(auto row : m_dataMatrix) {
        classes.insert(row[m_header[m_classAttributeHeader]]);
    }
    return classes.size();
}
