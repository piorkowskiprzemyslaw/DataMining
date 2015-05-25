#include <iostream>
#include <string>
#include "Data/Data.h"
#include "Data/DataLoader.h"

// default data file name
std::string FILE_NAME = "data.csv";
// read headers
bool READ_HEADERS = false;


std::shared_ptr<Data> loadData(std::shared_ptr<DataLoader>);
bool dataContainHeader();

int main(int argc, char* argv[])
{
    for( int i = 1 ; i < argc ; ++i){
        if( strcmp(argv[i], "-f") == 0 ){
            FILE_NAME = argv[++i];
            continue;
        }
        if( strcmp(argv[i], "-h") == 0 ) {
            READ_HEADERS = true;
            continue;
        }
        break;
    }

    std::shared_ptr<DataLoader> dataLoader = std::make_shared<DataLoader>();
    std::shared_ptr<Data> data = loadData(dataLoader);
    std::cout << "Data loaded!" << std::endl;

    return 0;
}

std::shared_ptr<Data> loadData(std::shared_ptr<DataLoader> dataLoader) {
    if( !dataLoader->setFileName(FILE_NAME) ) {
        std::cout << FILE_NAME << " read failed!" << std::endl;
        exit(1);
    }
    dataLoader->setReadHeaders(READ_HEADERS);
    std::shared_ptr<Data> data = dataLoader->loadData();

    return data;
}
