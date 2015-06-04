#pragma once

#include <istream>
#include "Data/Data.h"

class DFTReduction
{
private:
    const std::shared_ptr<const Data> m_train_data;
    double m_treshold;
public:
    explicit DFTReduction(const std::shared_ptr<const Data> train_data);
    void setTreshold(const double treshold);
    std::vector<double> reduceAttributes();
};

