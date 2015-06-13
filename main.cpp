#include <cassert>

#include <iostream>
#include <random>
#include <string>
#include <cstring>

#include "CHI/CHIReduction.h"
#include "DFT/DFTReduction.h"
#include "Data/Data.h"
#include "Data/DataAdapter.h"
#include "Data/DataLoader.h"
#include "KNNClassifier/KNNClassifier.h"
#include "MI/MIReduction.h"

enum RUNMODE {
    RM_DEF = 1,
    RM_DFT = 2,
    RM_MI = 4,
    RM_CHI = 8,
    RM_K = 16
};

using namespace std;

template<typename T>
std::shared_ptr<Data> loadData(T &&dataLoader, const std::string &fileName, bool readHeaders) {
    if( !dataLoader.setFileName(fileName) ) {
        std::cout << fileName << " does not exists!" << std::endl;
        exit(1);
    }
    dataLoader.setReadHeaders(readHeaders);
    return dataLoader.loadData();
}

void printNumberOfFaults(const std::vector<int>& classes, std::shared_ptr<const DataAdapter> test_data) {
    assert(test_data->getClassIdx() >= 0);
    const unsigned classIndex = test_data->getClassIdx();

    auto faults = 0u;
    for(size_t i = 0u; i < classes.size(); ++i) {
        if(classes[i]!= test_data->getRow(i)[classIndex]) {
            ++faults;
        }
    }
    std::cout << "Faults : " << faults << std::endl;
}

void printQuality(const std::vector<int>& classes, std::shared_ptr<const DataAdapter> test_data) {
    auto faults = 0u;
    auto true_positive = 0u;
    auto true_negative = 0u;
    auto false_positive = 0u;
    auto false_negative = 0u;
    float precision = 0.0;
    float recall = 0.0;

    for(size_t i = 0u; i < classes.size(); ++i) {
        if(classes[i]!= test_data->getRow(i)[57]) { //incorrectly...
            if (classes[i] == 1) ++false_positive; //identified
            else ++false_negative; //rejected
            ++faults;
        }
        else { //Correctly...
            if (classes[i] == 1) ++true_positive; //identified (as spam)
            else ++true_negative; //rejected
        }
    }
    precision = (float)true_positive / (true_positive + false_positive);
    recall = (float)true_positive / (true_positive + false_negative);

    std::cout << "Faults: " << faults << "; ";
    std::cout << "TP: " << true_positive << "; ";
    std::cout << "TN: " << true_negative << "; ";
    std::cout << "FP: " << false_positive << "; ";
    std::cout << "FN: " << false_negative << "; ";
    std::cout << "Precision: " << precision << "; Recall: " << recall << std::endl;
}

int main(int argc, char* argv[])
{
    // default train data file name
    std::string TRAIN_FILE_NAME = "data_train.csv";
    // default test data file name
    std::string TEST_FILE_NAME = "data_test.csv";
    // read headers
    bool READ_HEADERS = false;
    // class attribute column name
    std::string CLASS_NAME = "spam";
    // parameter K for KNN algorithm
    int K = 5;
    // dft algo treshold
    double DFT_TRESHOLD = 0.2;
    // mi algo reduction type
    MIReductionType MI_REDUCTION_TYPE = MIReductionType::MAX;
    // mi algo reduction treshold
    double MI_TRESHOLD = 0.2;
    // mi algo reduction type
    CHIReduction::ReductionMode CHI_REDUCTION_TYPE = CHIReduction::ReductionMode::Maximum;
    // mi algo reduction treshold
    double CHI_THRESHOLD = 0.2;
    //program mode
    unsigned ALGORITHMS = RM_DEF;
    // cross-validation
    unsigned CV = 3u;


    for( int i = 1 ; i < argc ; ++i){
        if( strcmp(argv[i], "-ftrain") == 0 ) {
            TRAIN_FILE_NAME = argv[++i];
        } else if( strcmp(argv[i], "-ftest") == 0 ) {
            TEST_FILE_NAME = argv[++i];
        } else if( strcmp(argv[i], "-h") == 0 ) {
            READ_HEADERS = true;
        } else if( strcmp(argv[i], "-cname") == 0 ) {
            CLASS_NAME = argv[++i];
        } else if( strcmp(argv[i], "-k") == 0 ) {
            K = std::stoi(argv[++i]);
            ALGORITHMS |= RM_K;
        } else if( strcmp(argv[i], "-DFT") == 0 ) {
            DFT_TRESHOLD = std::stod(argv[++i]);
            ALGORITHMS |= RM_DFT;
        } else if( strcmp(argv[i], "-MI_MAX") == 0 ) {
            MI_REDUCTION_TYPE = MIReductionType::MAX;
            ALGORITHMS |= RM_MI;
        } else if( strcmp(argv[i], "-MI_AVG") == 0 ) {
            MI_REDUCTION_TYPE = MIReductionType::AVG;
            ALGORITHMS |= RM_MI;
        } else if( strcmp(argv[i], "-MI") == 0 ) {
            MI_TRESHOLD = std::stod(argv[++i]);
            ALGORITHMS |= RM_MI;
        } else if( strcmp(argv[i], "-l") == 0 ) {
            Logger::setLogLevel(std::stoi(argv[++i]));
        } else if( strcmp(argv[i], "-CHI_MAX") == 0 ) {
            CHI_REDUCTION_TYPE = CHIReduction::ReductionMode::Maximum;
            ALGORITHMS |= RM_CHI;
        } else if( strcmp(argv[i], "-CHI_AVG") == 0 ) {
            CHI_REDUCTION_TYPE = CHIReduction::ReductionMode::Average;
            ALGORITHMS |= RM_CHI;
        } else if( strcmp(argv[i], "-CHI") == 0 ) {
            CHI_THRESHOLD = std::stod(argv[++i]);
            ALGORITHMS |= RM_CHI;
        } else if( strcmp(argv[i], "-CV") == 0 ) {
            CV = std::stoi(argv[++i]);
        }
    }

    DataLoader dataLoader(CLASS_NAME);
    const auto data = loadData(dataLoader, TRAIN_FILE_NAME, READ_HEADERS);
    data->computeParameters();
    data->computeMinMaxValues();
    data->normalization();

    // to go full random use random_device
    //std::random_device rd;
    std::mt19937 gen/*(rd())*/;
    std::uniform_real_distribution<> dis(0, 1);

    // generate splits
    std::vector<std::shared_ptr<DataAdapter>> adapters;
    {
        adapters.reserve(CV);
        auto leftPart = std::make_shared<DataAdapter>(data, [](const Data::RowType &){return true;});
        for (auto split = CV; split > 1; --split) {
            adapters.emplace_back(std::make_shared<DataAdapter>(*leftPart, [&](const Data::RowType &) {
                // 33% chance that the item will be picked should result in random 1/3 split (?)
                return dis(gen) < (1.0/split);
            }));
            leftPart->remove(*adapters.back());
        }
        adapters.emplace_back(std::move(leftPart));
    }

    LOG(DEBUG) << "From " << data->nRow() << " picked:" << std::endl;
    for (const auto &adapter : adapters) {
        LOG(DEBUG) << adapter->nRow() << ": " << adapter->indices();
    }

    for (auto part = 0u; part < CV; ++part) {

        std::cout << "== Split " << 1+part << "/" << CV << " ==" << std::endl;
        const auto & test_data = adapters[part];
        const auto train_data = std::make_shared<DataAdapter>(*test_data);
        train_data->invert();

        KNNClassifier classif(train_data, K);
        classif.setTestData(test_data);

        if (ALGORITHMS & RM_K){
            // without reduction
            std::cout << "* kNN" << std::endl;
            const std::vector<double> w1(test_data->nAttributes(), 1.0);
            const auto classes = classif.classifiy(w1);
            printQuality(classes,test_data);
        }

        if (ALGORITHMS & RM_DFT){
            // dft reduction
            std::cout << "* DFT" << std::endl;
            DFTReduction dft(train_data);
            dft.setTreshold(DFT_TRESHOLD);
            const std::vector<double> w2 = dft.reduceAttributes();
            std::cout << "Picked " << std::accumulate(w2.cbegin(), w2.cend(), 0u) << " attributes" << std::endl;
            const auto classes_dft = classif.classifiy(w2);
            printQuality(classes_dft, test_data);
        }
        if (ALGORITHMS & RM_MI){
            // mi reduction
            std::cout << "* MI" << std::endl;
            MIReduction mi(train_data);
            mi.setReductionType(MI_REDUCTION_TYPE);
            mi.setTreshold(MI_TRESHOLD);
            const std::vector<double> w3 = mi.reduceAttributes();
            std::cout << "Picked " << std::accumulate(w3.cbegin(), w3.cend(), 0u) << " attributes" << std::endl;
            const auto classes_mi = classif.classifiy(w3);
            printQuality(classes_mi, test_data);
        }
        if (ALGORITHMS & RM_CHI){
            // chi reduction
            std::cout << "* CHI" << std::endl;
            CHIReduction chi(train_data);
            chi.setThreshold(CHI_THRESHOLD);
            const auto w4 = chi.reduce(CHI_REDUCTION_TYPE);
            std::cout << "Picked " << std::accumulate(w4.cbegin(), w4.cend(), 0u) << " attributes" << std::endl;
            const auto classes_chi = classif.classifiy(w4);
            printQuality(classes_chi, test_data);
        }
    }

    return 0;
}


