#include "Data.h"

Data::Data(const std::string &classAttributeHeader)
    : m_classAttributeHeader{classAttributeHeader}
{ }

void Data::addColumnHeader(const std::string &columnName, const int columnNumber)
{
    m_header.emplace(columnName, columnNumber);
}

std::ostream & operator<<(std::ostream &stream, const Data &data)
{
    for(const auto &entry : data.m_header) {
        stream << entry.first << " : " << entry.second << std::endl;
    }

    for(const auto &row : data.m_dataMatrix) {
        for(const auto &attr : row) {
            stream << attr << " ";
        }
        stream << std::endl;
    }
    return stream;
}

void Data::printData() const
{
    std::cout << *this;
}

void Data::computeMinMaxValues()
{
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

void Data::setMinMaxValues(const std::vector<std::pair<double, double>> &minMaxValues)
{
    m_minMaxValues = minMaxValues;
}

const std::vector<std::pair<double, double>>& Data::getMinMaxValues() const
{
    return m_minMaxValues;
}

void Data::normalization()
{
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

const std::vector<double>& Data::getRow(size_t rowNumber) const
{
    return m_dataMatrix[rowNumber];
}

size_t Data::nRow() const
{
    return m_dataMatrix.size();
}

size_t Data::nAttributes() const
{
    return m_dataMatrix[0].size();
}

const std::string& Data::getClassAttributeHeader() const
{
    return m_classAttributeHeader;
}

int Data::getClassIdx() const {
    return m_header.at(m_classAttributeHeader);
}

const std::set<int> Data::getClassesValues() const {
    return m_classValues;
}

int Data::getNumberOfClasses() const {
    return m_numberOfClasses;
}

const std::vector<double> Data::getClassProbability() const {
    return m_classProbability;
}

void Data::computeParameters() {
    computeClassesValues();
    computeNumberOfClasses();
    computeClassesProbability();
}

void Data::computeClassesValues() {
    for(const auto &row : m_dataMatrix) {
        m_classValues.emplace(row[m_header[m_classAttributeHeader]]);
    }
}

void Data::computeNumberOfClasses() {
    m_numberOfClasses = m_classValues.size();
}

void Data::computeClassesProbability() {
    m_classProbability = std::vector<double>(getNumberOfClasses(), 0.0);
    int classAttributeIdx = getClassIdx();

    for(const auto &row : m_dataMatrix) {
        ++m_classProbability[ row[classAttributeIdx] ];
    }

    for(auto& classProb : m_classProbability) {
        classProb /= nRow();
    }
}
