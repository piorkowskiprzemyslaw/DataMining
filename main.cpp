#include <iostream>
#include <string>
#include "Data/Data.h"
#include "Data/DataLoader.h"

std::shared_ptr<Data> loadData(std::shared_ptr<DataLoader>);
bool dataContainHeader();

int main()
{
    std::shared_ptr<DataLoader> dataLoader = std::make_shared<DataLoader>();
    std::shared_ptr<Data> data = loadData(dataLoader);

    return 0;
}

std::shared_ptr<Data> loadData(std::shared_ptr<DataLoader> dataLoader) {
    std::string fileName;
    int counter = 0;
    do {
        if(counter > 0 )
            std::cout << "File read failed, try again : " ;
        else
            std::cout << "Type file name : ";
        std::cin >> fileName;
        ++counter;
    } while( !dataLoader->setFileName(fileName) );
    dataLoader->setReadHeaders(dataContainHeader());
    dataLoader->loadData();

    return nullptr;
}

bool dataContainHeader() {
    int counter = 0;
    std::string response;
    do {
        if(!counter)
            std::cout << "Does data file contain headers [Y|N] : ";
        else
            std::cout << "Type Y or N : ";

        std::cin >> response;
        if (response=="Y" || response=="y") {
            return true;
        } else if (response=="N" || response=="n") {
            return false;
        }
        ++counter;
    } while(true);
}
