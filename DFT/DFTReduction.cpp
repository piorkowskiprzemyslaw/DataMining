#include "DFTReduction.h"

DFTReduction::DFTReduction(const std::shared_ptr<const Data> train_data)
    : m_train_data(std::move(train_data))
{ }

void DFTReduction::setTreshold(const double treshold)
{
    m_treshold = treshold;
}

std::vector<double> DFTReduction::reduceAttributes()
{
    std::vector<double> occurences(m_train_data->nAttributes(), 0.0);

    for(size_t i = 0; i < m_train_data->nRow(); ++i) {
        const auto &row = m_train_data->getRow(i);
        for(long int j = 0; j < m_train_data->nAttributes(); ++j) {
            if(row[j]>0 && j != m_train_data->getClassIdx()) {
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
