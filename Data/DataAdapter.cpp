#include "DataAdapter.h"

#include <cassert>

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

//const std::vector<std::pair<double, double>>& DataAdapter::getMinMaxValues() const
//{
//    if (m_minMaxValues.size() != m_indices.size()) {
//        computeMinMaxValues();
//    }
//    return m_minMaxValues;
//}

//void DataAdapter::computeMinMaxValues() const
//{
//    double actualVal;
//    // vector of (min;max) values; one per attributes
//    m_minMaxValues = std::vector<std::pair<double,double>>(Data::CONTINUOUS_ATTRIBUTES,
//            std::make_pair(std::numeric_limits<double>::max(), std::numeric_limits<double>::min()));
//    for(size_t i = 0; i < nRow(); ++i) {
//        for(size_t j = 0; j < Data::CONTINUOUS_ATTRIBUTES; ++j) {
//            actualVal = getRow(i)[j+Data::PERCENTAGE_ATTRIBUTES];
//            if(actualVal < m_minMaxValues[j].first) {
//                m_minMaxValues[j].first = actualVal;
//            }
//            if(actualVal > m_minMaxValues[j].second) {
//                m_minMaxValues[j].second = actualVal;
//            }
//        }
//    }
//}

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

void DataAdapter::join(const DataAdapter &other)
{
    assert(other.m_data == m_data);

    m_indices.reserve(m_indices.size() + other.m_indices.size());
    m_indices.insert(m_indices.cend(), other.m_indices.cbegin(), other.m_indices.cend());
    std::sort(m_indices.begin(), m_indices.end());
    m_indices.erase(std::unique(m_indices.begin(), m_indices.end()), m_indices.end());
    m_indices.shrink_to_fit();

    m_classProbabilities.clear();
    m_classProbabilities.shrink_to_fit();

    // TODO merge minMax from other and this
//    m_minMaxValues.clear();
//    m_minMaxValues.shrink_to_fit();
}

void DataAdapter::remove(const DataAdapter &other)
{
    assert(other.m_data == m_data);

    // TODO It could be done in one go
    m_indices.erase(std::remove_if(m_indices.begin(), m_indices.end(),
                                   [&](const decltype(m_indices)::value_type &value) {
        return other.m_indices.cend() != std::find(other.m_indices.cbegin(), other.m_indices.cend(), value);
    }), m_indices.end());
    m_indices.shrink_to_fit();

    m_classProbabilities.clear();
    m_classProbabilities.shrink_to_fit();

//    m_minMaxValues.clear();
//    m_minMaxValues.shrink_to_fit();
}

void DataAdapter::invert()
{
    decltype(m_indices) newIndices;
    const auto rowNumber = m_data->nRow();
    newIndices.resize(rowNumber - m_indices.size());
    auto currentIndex = 0u;
    auto indexIt = m_indices.cbegin();
    std::generate(newIndices.begin(), newIndices.end(), [&](){
        if (indexIt != m_indices.cend() && currentIndex == *indexIt) {
            ++currentIndex;
            ++indexIt;
        }
        return currentIndex++;
    });
    m_indices = std::move(newIndices);
}
