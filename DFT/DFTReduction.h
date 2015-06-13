#pragma once

#include <istream>
#include "Data/DataAdapter.h"

class DFTReduction
{
private:
    const std::shared_ptr<const DataAdapter> m_train_data;
    double m_treshold;
public:
    explicit DFTReduction(const std::shared_ptr<const DataAdapter> train_data);
    void setTreshold(const double treshold);
    std::vector<double> reduceAttributes();
};

