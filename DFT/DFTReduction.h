#pragma once

#include <istream>
#include "Data/Data.h"

class DFTReduction
{
private:
    std::shared_ptr<Data> m_train_data;
    double m_treshold;
public:
    DFTReduction(std::shared_ptr<Data> train_data);
    void setTreshold(const double treshold);
    const std::vector<double> reduceAttributes();
};

