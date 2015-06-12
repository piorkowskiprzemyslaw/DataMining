#include "DataAdapter.h"

void DataAdapter::calculateAdaptationVector(const std::function<bool(const Data::RowType &)> &adapter)
{
    size_t index = 0u;
    for (const auto &row : *m_data) {
        if (adapter(row)) {
            m_indices.emplace_back(index);
        }
        ++index;
    }
}

std::ostream & operator<<(std::ostream &stream, const DataAdapter &data)
{
    for(const auto &row : data) {
        for(const auto &attr : row) {
            stream << attr << " ";
        }
        stream << std::endl;
    }
    return stream;
}

void DataAdapter::printData() const
{
    std::cout << *this;
}

const std::vector<std::pair<double, double>>& DataAdapter::getMinMaxValues() const
{
    if (m_minMaxValues.size() != m_indices.size()) {
        computeMinMaxValues();
    }
    return m_minMaxValues;
}

void DataAdapter::computeMinMaxValues() const
{
    double actualVal;
    // vector of (min;max) values; one per attributes
    m_minMaxValues = std::vector<std::pair<double,double>>(Data::CONTINUOUS_ATTRIBUTES,
            std::make_pair(std::numeric_limits<double>::max(), std::numeric_limits<double>::min()));
    for(size_t i = 0; i < nRow(); ++i) {
        for(size_t j = 0; j < Data::CONTINUOUS_ATTRIBUTES; ++j) {
            actualVal = getRow(i)[j+Data::PERCENTAGE_ATTRIBUTES];
            if(actualVal < m_minMaxValues[j].first) {
                m_minMaxValues[j].first = actualVal;
            }
            if(actualVal > m_minMaxValues[j].second) {
                m_minMaxValues[j].second = actualVal;
            }
        }
    }
}

const std::vector<double> &DataAdapter::getClassProbability() const
{
    if (m_classProbabilities.empty()) {
        computeClassProbabilities();
    }
    return m_classProbabilities;
}

void DataAdapter::computeClassProbabilities() const
{
    m_classProbabilities = std::vector<double>(getNumberOfClasses(), 0.0);
    const int classAttributeIdx = getClassIdx();

    for(const auto &row : *this) {
        ++m_classProbabilities[ row[classAttributeIdx] ];
    }

    for(auto& classProb : m_classProbabilities) {
        classProb /= nRow();
    }
}
