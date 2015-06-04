#include <iostream>
#include <string>
#include "Data/Data.h"
#include "Data/DataLoader.h"
#include "KNNClassifier/KNNClassifier.h"
#include "DFT/DFTReduction.h"
#include "MI/MIReduction.h"

template<typename T>
std::shared_ptr<Data> loadData(T &&dataLoader, const std::string &fileName, bool readHeaders) {
    if( !dataLoader.setFileName(fileName) ) {
        std::cout << fileName << " does not exists!" << std::endl;
        exit(1);
    }
    dataLoader.setReadHeaders(readHeaders);
    return dataLoader.loadData();
}

void printNumberOfFaults(const std::vector<int>& classes, std::shared_ptr<Data> test_data) {
    auto faults = 0u;
    for(size_t i = 0u; i < classes.size(); ++i) {
        if(classes[i]!= test_data->getRow(i)[57]) {
            ++faults;
        }
    }
    std::cout << "Faults : " << faults << std::endl;
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
        } else if( strcmp(argv[i], "-DFT") == 0 ) {
            DFT_TRESHOLD = std::stod(argv[++i]);
        } else if( strcmp(argv[i], "-MI_MAX") == 0 ) {
            MI_REDUCTION_TYPE = MIReductionType::MAX;
        } else if( strcmp(argv[i], "-MI_AVG") == 0 ) {
            MI_REDUCTION_TYPE = MIReductionType::AVG;
        } else if( strcmp(argv[i], "-MI") == 0 ) {
            MI_TRESHOLD = std::stod(argv[++i]);
        } else if( strcmp(argv[i], "-l") == 0 ) {
            Logger::setLogLevel(std::stoi(argv[++i]));
        }
    }

    DataLoader dataLoader(CLASS_NAME);
    const auto train_data = loadData(dataLoader, TRAIN_FILE_NAME, READ_HEADERS);
    train_data->computeParameters();
    const auto test_data = loadData(dataLoader, TEST_FILE_NAME, READ_HEADERS);
    test_data->computeParameters();

    KNNClassifier classif(train_data, K);
    classif.setTestData(test_data);

    // without reduction
    const std::vector<double> w1(test_data->nAttributes(), 1.0);
    const auto classes = classif.classifiy(w1);
    printNumberOfFaults(classes,test_data);

    // dft reduction
    DFTReduction dft(train_data);
    dft.setTreshold(DFT_TRESHOLD);
    const std::vector<double> w2 = dft.reduceAttributes();
    const auto classes_dft = classif.classifiy(w2);
    printNumberOfFaults(classes_dft, test_data);

    // mi reduction
    MIReduction mi(train_data);
    mi.setReductionType(MI_REDUCTION_TYPE);
    mi.setTreshold(MI_TRESHOLD);
    const std::vector<double> w3 = mi.reduceAttributes();
    const auto classes_mi = classif.classifiy(w3);
    printNumberOfFaults(classes_mi, test_data);

    return 0;
}


