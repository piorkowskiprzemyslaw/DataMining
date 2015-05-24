#include "Data.h"

Data::Data() {

}

void Data::addColumnHeader(const int columnNumber, const std::string &columnName) {
    m_header.emplace(columnNumber, columnName);
}

void Data::addDataMatrixRow(std::vector<double> &&row) {
    m_dataMatrix.emplace_back(row);
}

void Data::printData() {
    for(auto entry : m_header) {
        std::cout << entry.first << " : " << entry.second << std::endl;
    }

    for(auto row : m_dataMatrix) {
        for(auto attr : row) {
            std::cout << attr << " ";
        }
        std::cout << std::endl;
    }
}
