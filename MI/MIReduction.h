#pragma once

#include <iostream>
#include <memory>
#include <cmath>
#include <climits>

#include "Data/Data.h"

enum class MIReductionType {
    AVG,
    MAX
};

class MIReduction
{
private:
    /*
     * A = attribute occurs, class occurs
     * B = attribute occurs without class
     * C = class occurs without attribute
     */
    enum Param : int { A = 0, B = 1, C = 2 };

    const std::shared_ptr<const Data> m_train_data;
    std::vector<std::vector<std::vector<unsigned int>>> m_parameters;
    std::vector<std::vector<double>> m_miMatrix;
    unsigned int m_classIdx;
    MIReductionType m_miReductionType;
    double m_treshold;

    void computeParametersVector();
    void computeMIMatrix();
    const std::vector<double> avgReduction();
    const std::vector<double> maxReduction();
    void filterWeights(std::vector<double>& weights);

    void printWeights(const std::vector<double>& weights)
    {
        for(const auto &weight : weights) {
            LOG(INFO) << weight;
        }
    }

public:
    explicit MIReduction(const std::shared_ptr<const Data> train_data);
    void setReductionType(const MIReductionType type);
    void setTreshold(double treshold);
    const std::vector<double> reduceAttributes();
};

