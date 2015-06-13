#include "DFTReduction.h"

#include <cassert>

DFTReduction::DFTReduction(const std::shared_ptr<const DataAdapter> train_data)
    : m_train_data(std::move(train_data))
{ }

void DFTReduction::setTreshold(const double treshold)
{
    m_treshold = treshold;
}

std::vector<double> DFTReduction::reduceAttributes()
{
    assert(m_train_data->nAttributes() > 0);
    const auto attributesNumber = m_train_data->nAttributes();

    assert(m_train_data->getClassIdx() >= 0);
    const unsigned classIndex = m_train_data->getClassIdx();

    std::vector<double> occurences(attributesNumber);

    for(decltype(m_train_data->nRow()) i = 0u; i < m_train_data->nRow(); ++i) {
        const auto &row = m_train_data->getRow(i);
        // Sorry about the type
        for(std::remove_const<decltype(attributesNumber)>::type j = 0u; j < attributesNumber; ++j) {
            if(row[j] > 0 && j != classIndex) {
                ++occurences[j];
            }
        }
    }

    for(auto &el : occurences) {
        el /= m_train_data->nRow();
        el = el > m_treshold ? 1.0 : 0.0;
    }

    return occurences;
}
