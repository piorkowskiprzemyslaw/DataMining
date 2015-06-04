#include "KNNClassifier.h"

KNNClassifier::KNNClassifier(std::shared_ptr<Data> trainData, const int k)
    : m_trainData(std::move(trainData))
    , m_k(k)
{
    m_trainData->computeMinMaxValues();
    m_trainData->normalization();
    m_classIdx = m_trainData->getClassIdx();
}

void KNNClassifier::setTestData(std::shared_ptr<Data> testData)
{
    m_testData = std::move(testData);
    m_testData->setMinMaxValues(m_trainData->getMinMaxValues());
    m_testData->normalization();
}

std::vector<int> KNNClassifier::classifiy(const std::vector<double>& weights) const
{
    std::vector<int> classes(m_testData->nRow());
    if( weights.size() != m_trainData->nAttributes() ) {
        std::cerr << " Incompatible size of weights "
                     "vector and number of train data attributes" << std::endl;
        return classes;
    }

    for(size_t i = 0; i < m_testData->nRow(); ++i) {
        const std::vector<double>& testRow = m_testData->getRow(i);
        std::priority_queue<std::pair<int,double>, std::vector<std::pair<int,double>>, PairCompare> pq;
        for(size_t j = 0; j < m_trainData->nRow(); ++j) {
            const std::vector<double>& trainRow = m_trainData->getRow(j);
            pq.push(std::make_pair(j,distance(testRow,trainRow,weights)));
        }
        classes[i] = getResultClass(pq);
    }

    return classes;
}

int KNNClassifier::getResultClass(std::priority_queue<std::pair<int, double>,
                                  std::vector<std::pair<int, double> >, PairCompare>& pq) const
{
    int cls = 0;
    std::pair<int, double> actualPair;
    std::vector<double> clsVal(m_trainData->getNumberOfClasses(), 0.0);

    for(int i = 0; i < m_k; ++i) {
        actualPair = pq.top();
        const std::vector<double>& row = m_trainData->getRow(actualPair.first);
        clsVal[ row[m_classIdx] ] += (1 / actualPair.second);
        pq.pop();
    }

    for(size_t i = 0; i < clsVal.size(); ++i) {
        if(clsVal[cls] < clsVal[i]) {
            cls = i;
        }
    }

    return cls;
}

double KNNClassifier::distance(const std::vector<double> &v1,
                               const std::vector<double> &v2,
                               const std::vector<double> &weights) const
{
    double distance = 0;
    double partialSum = 0;
    for(decltype(v1.size()) i = 0 ; i < v1.size() ; ++i) {
        if(i != m_classIdx) {
            partialSum = v1[i] - v2[i];
            distance += ((partialSum * partialSum) * weights[i] );
        }
    }
    return std::sqrt(distance);
}
