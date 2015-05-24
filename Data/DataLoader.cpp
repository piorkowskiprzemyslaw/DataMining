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

std::shared_ptr<Data> DataLoader::loadData() {
    std::shared_ptr<Data> data = std::make_shared<Data>();
    std::fstream file(m_fileName);
    std::locale csv(std::locale::classic(), new csvctype);
    std::string line;

    while(getline(file, line)) {
        std::istringstream is(line);
        is.imbue(csv);

        if(m_readHeaders) {
            std::string fieldName;
            for(int i = 0 ; is >> fieldName ; ++i ) {
                data->addColumnHeader(i, fieldName);
            }
            m_readHeaders = false;
        } else {
            double attrVal;
            std::vector<double> row;
            for(int i = 0 ; is >> attrVal ; ++i ) {
                row.push_back(attrVal);
            }
            data->addDataMatrixRow(std::move(row));
        }
    }
    return data;
}
