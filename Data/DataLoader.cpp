#include "DataLoader.h"

DataLoader::DataLoader() { }

bool DataLoader::setFileName(const std::string &fileName) {
    if(fileExist(fileName)) {
        m_fileName.assign(fileName);
        return true;
    }
    return false;
}

bool DataLoader::fileExist(const std::string &fileName) {
    return ( access( fileName.c_str(), F_OK ) != -1 );
}

void DataLoader::setReadHeaders(bool readHeaders) {
    m_readHeaders = readHeaders;
}

void DataLoader::loadData() {
    std::fstream file(m_fileName);
    std::string line;

    while(getline(file, line)) {
        std::istringstream is(line);
        std::string fieldName;
        std::locale csv(std::locale::classic(), new csvctype);
        is.imbue(csv);

        for(int i = 0 ; is >> fieldName ; ++i ) {

        }
        break;
    }
}
