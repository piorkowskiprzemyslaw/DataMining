#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include "Data/Data.h"

struct PairCompare
{
    bool operator()(const std::pair<int,double> & lhs,
                    const std::pair<int,double> & rhs){
        return lhs.second > rhs.second;
    }
};

class KNNClassifier
{
private:
    std::string m_classHeader;
    std::shared_ptr<Data> m_trainData;
    std::shared_ptr<Data> m_testData;
    int m_k;
    size_t m_classIdx;

    double distance(const std::vector<double>& v1,
                    const std::vector<double>& v2,
                    const std::vector<double>& weights) const;
    int getResultClass(std::priority_queue<std::pair<int, double>,
                       std::vector<std::pair<int, double> >, PairCompare> &pq) const;
public:
    KNNClassifier(std::shared_ptr<Data> trainData, const int k);
    void setTestData(std::shared_ptr<Data> testData);
    std::vector<int> classifiy(const std::vector<double>& weights) const;
};

