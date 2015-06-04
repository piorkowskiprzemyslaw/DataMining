#include "MIReduction.h"

MIReduction::MIReduction(const std::shared_ptr<const Data> train_data)
    : m_train_data(std::move(train_data))
{
    m_classIdx = train_data->getClassIdx();
}

void MIReduction::setReductionType(const MIReductionType type)
{
    m_miReductionType = type;
}

void MIReduction::setTreshold(double treshold)
{
    m_treshold = treshold;
}

const std::vector<double> MIReduction::reduceAttributes()
{
    computeParametersVector();
    computeMIMatrix();
    if(m_miReductionType == MIReductionType::AVG) {
        return avgReduction();
    } else /* if(m_ireductiontype == MIReductionType::MAX) */ {
        return maxReduction();
    }
}

void MIReduction::computeParametersVector()
{
    m_parameters = std::vector<std::vector<std::vector<unsigned int>>>(m_train_data->nAttributes(),
                                                                       std::vector<std::vector<unsigned int>>( m_train_data->getNumberOfClasses(),
                                                                                                               std::vector<unsigned int>(3,0) ) );

    for(size_t i = 0 ; i < m_train_data->nRow(); ++i ) {
        const std::vector<double>& row = m_train_data->getRow(i);
        const auto actualClass = row[m_classIdx];
        for(size_t attNumber = 0 ; attNumber < m_train_data->nAttributes(); ++attNumber) {
            if(row[attNumber] > 0) {
                m_parameters[attNumber][actualClass][Param::A]++;
                for(auto cls = 0 ; cls < m_train_data->getNumberOfClasses(); ++cls) {
                    if(cls != actualClass) {
                        m_parameters[attNumber][cls][Param::B]++;
                    }
                }
            } else /* if(row[attNumber] == 0) */ {
                m_parameters[attNumber][actualClass][Param::C]++;
            }
        }
    }
}

void MIReduction::computeMIMatrix()
{
    m_miMatrix = std::vector<std::vector<double>>(m_train_data->nAttributes(),
                                                  std::vector<double>(m_train_data->getNumberOfClasses(), 0.0));
    auto N = m_train_data->nRow();
    for(size_t attNumber = 0; attNumber < m_train_data->nAttributes(); ++attNumber) {
        for(auto actualCls = 0; actualCls < m_train_data->getNumberOfClasses(); ++actualCls) {
            double A = m_parameters[attNumber][actualCls][Param::A];
            double B = m_parameters[attNumber][actualCls][Param::B];
            double C = m_parameters[attNumber][actualCls][Param::C];
            m_miMatrix[attNumber][actualCls] = log((A*N)/((A+C)*(A+B)));
            m_miMatrix[attNumber][actualCls] = std::max(std::numeric_limits<double>::min(), m_miMatrix[attNumber][actualCls]);
        }
    }
}

const std::vector<double> MIReduction::avgReduction()
{
    std::vector<double> weights(m_train_data->nAttributes(),0.0);
    const auto &classProbability = m_train_data->getClassProbability();

    for(size_t attNumber = 0; attNumber < m_train_data->nAttributes(); ++attNumber) {
        for(auto actualCls = 0; actualCls < m_train_data->getNumberOfClasses(); ++actualCls) {
            std::cout << classProbability[actualCls] << " " << m_miMatrix[attNumber][actualCls] << std::endl;
            weights[attNumber] += classProbability[actualCls] * m_miMatrix[attNumber][actualCls];
        }
    }
    printWeights(weights);
    filterWeights(weights);
    return weights;
}

const std::vector<double> MIReduction::maxReduction()
{
    std::vector<double> weights(m_train_data->nAttributes(), std::numeric_limits<double>::min());

    for(size_t attNumber = 0; attNumber < m_train_data->nAttributes(); ++attNumber) {
        std::stringstream logStr;
        for(auto actualCls = 0; actualCls < m_train_data->getNumberOfClasses(); ++actualCls) {
            logStr << m_miMatrix[attNumber][actualCls] << " ";
            if(weights[attNumber] < m_miMatrix[attNumber][actualCls]) {
                weights[attNumber] = m_miMatrix[attNumber][actualCls];
            }
        }
        LOG(INFO) << logStr.rdbuf();
    }
    printWeights(weights);
    filterWeights(weights);
    return weights;
}

void MIReduction::filterWeights(std::vector<double> &weights)
{
    for(auto &weight: weights) {
        weight = weight > m_treshold ? 1.0 : 0.0;
    }
}
