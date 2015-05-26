#include <iostream>
#include <string>
#include "Data/Data.h"
#include "Data/DataLoader.h"
#include "KNNClassifier/KNNClassifier.h"

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


std::shared_ptr<Data> loadData(std::shared_ptr<DataLoader> dataLoader, const std::string& fileName) {
    if( !dataLoader->setFileName(fileName) ) {
        std::cout << fileName << " read failed!" << std::endl;
        exit(1);
    }
    dataLoader->setReadHeaders(READ_HEADERS);
    return dataLoader->loadData();
}

void printNumberOfFaults(const std::vector<int>& classes, std::shared_ptr<Data> test_data) {
    int faults = 0;
    for(int i = 0; i < classes.size(); ++i) {
        if(classes[i]!= test_data->getRow(i)[57]) {
            faults++;
        }
    }
    std::cout << "Faults : " << faults << std::endl;
}

int main(int argc, char* argv[])
{
    for( int i = 1 ; i < argc ; ++i){
        if( strcmp(argv[i], "-ftrain") == 0 ) {
            TRAIN_FILE_NAME = argv[++i];
            continue;
        }
        if( strcmp(argv[i], "-ftest") == 0 ) {
            TEST_FILE_NAME = argv[++i];
            continue;
        }
        if( strcmp(argv[i], "-h") == 0 ) {
            READ_HEADERS = true;
            continue;
        }
        if( strcmp(argv[i], "-cname") == 0 ) {
            CLASS_NAME = argv[++i];
            continue;
        }
        if( strcmp(argv[i], "-k") == 0 ) {
            K = std::stoi(argv[++i]);
            continue;
        }
        break;
    }

    std::shared_ptr<DataLoader> dataLoader = std::make_shared<DataLoader>(CLASS_NAME);
    std::shared_ptr<Data> train_data = loadData(dataLoader, TRAIN_FILE_NAME);
    std::shared_ptr<Data> test_data = loadData(dataLoader, TEST_FILE_NAME);

    std::shared_ptr<KNNClassifier> classif = std::make_shared<KNNClassifier>(train_data, K);
    classif->setTestData(test_data);

    std::vector<double> weights(train_data->nAttributes(), 1);
    std::vector<int> classes = classif->classifiy(weights);
    printNumberOfFaults(classes, test_data);

    return 0;
}


