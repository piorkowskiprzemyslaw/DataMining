#include "DFTReduction.h"

DFTReduction::DFTReduction(std::shared_ptr<Data> train_data) : m_train_data(train_data) { }

void DFTReduction::setTreshold(const double treshold) {
    m_treshold = treshold;
}

const std::vector<double> DFTReduction::reduceAttributes() {
    std::vector<double> occurences(m_train_data->nAttributes(),0.0);

    for(size_t i = 0; i < m_train_data->nRow(); ++i) {
        const std::vector<double> row = m_train_data->getRow(i);
        for(size_t j = 0; j < m_train_data->nAttributes(); ++j) {
            if(row[j]>0 && j!=m_train_data->getClassIdx())
                occurences[j]++;
        }
    }

    for(auto& el : occurences){
        el /= m_train_data->nRow();
        el = el > m_treshold ? 1.0 : 0.0;
    }

    return occurences;
}
