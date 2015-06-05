#include "CHIReduction.h"

#include <cassert>

#include <numeric>
#include <thread>

#include "Logger.h"

namespace {

template <typename T>
std::ostream & operator<<(std::ostream& stream, std::vector<T> data)
{
    if (!data.empty()) {
        stream << "[ ";
        auto end = data.size() - 1;
        for (decltype(data.size()) i = 0u; i < end; ++i) {
            stream << data[i] << ", ";
        }
        stream << data[end] << " ]";
    } else {
        stream << "[]";
    }
    return stream;
}

}

std::vector<double> CHIReduction::reduce(ReductionMode mode)
{
    auto matrix = calculateCHIMatrix();

    std::vector<double> result;
    result.reserve(matrix.size());

    if (mode == ReductionMode::Maximum) {
        for (const auto & termValues: matrix) {
            result.emplace_back((*std::max_element(termValues.cbegin(), termValues.cend())) > m_threshold);
        }
    } else if (mode == ReductionMode::Average) {
        for (const auto & termValues: matrix) {
            result.emplace_back((std::accumulate(termValues.cbegin(), termValues.cend(), 0.0) / termValues.size()) > m_threshold);
        }
    } else {
        LOG(ERROR) << "No such reduction mode";
        assert(false);
    }
    LOG(DEBUG) << "Result: " << result;
    return result;
}

CHIReduction::Matrix CHIReduction::calculateCHIMatrix() const
{
    const auto attributesNumber = m_data->nAttributes();
    assert(attributesNumber > 0);

    assert(m_data->getNumberOfClasses() > 0);
    const unsigned classNumber = m_data->getNumberOfClasses();

    assert(m_data->getClassIdx() >= 0);
    const unsigned classIndex = m_data->getClassIdx();

    // Without class attribute
    Matrix resultMatrix(attributesNumber);
    // Quick test revealed that resize is faster (up to 2x) than providing default value
    // in a construtor. It's much faster for big vectors.
    for (decltype(resultMatrix.size()) i = 0u; i < resultMatrix.size(); ++i) {
        if (i != classIndex) {
            resultMatrix[i].resize(classNumber);
        } else {
            resultMatrix[i].emplace_back(1.0);
        }
    }


    const auto calculateValues = [this, classNumber, classIndex, &resultMatrix](const unsigned attribute) {
        // Holds values needed for calculating goodnessMeasure
        std::vector<std::tuple<unsigned, unsigned, unsigned, unsigned>> values(classNumber, {0, 0, 0, 0});

        for (const auto &row : *m_data) {
            assert(row.size() == resultMatrix.size());
            assert(row[classIndex] >= 0 && row[classIndex] == static_cast<unsigned>(row[classIndex]));

            // We count occurences of class and terms
            const unsigned klass = row[classIndex];
            const auto exists = row[attribute] > 0.0;
            for (decltype(values.size()) i = 0u; i < values.size(); ++i) {
                if (i != klass) {
                    if (exists) {
                        // Term exists but not with that class
                        ++std::get<1>(values[i]);
                    } else {
                        // Nor class nor term exists
                        ++std::get<3>(values[i]);
                    }
                } else {
                    if (exists) {
                        // Term and class exists
                        ++std::get<0>(values[i]);
                    } else {
                        // Class exist but term does not
                        ++std::get<2>(values[i]);
                    }
                }
            }
        }
        for (decltype(values.size()) i = 0u; i < values.size(); ++i) {
            resultMatrix[attribute][i] = goodnessMeasure(m_data->nRow(),
                                                         std::get<0>(values[i]),
                                                         std::get<1>(values[i]),
                                                         std::get<2>(values[i]),
                                                         std::get<3>(values[i]));
        }
    };

    /* Calculating this values is thread safe on following conditions:
     * 1. Vector does not change it size (that's why it was resized before)
     * 2. Each element is accessed only from the same thread - row is read concurrently
     *    but that's fine it's read only. Threads are split on column, this way threads
     *    write to only one element of result vector (one column).
     *
     */
    // TODO Use hardware_concurrency and do not spawn threads like crazy
    std::vector<std::thread> threads;
    threads.reserve(attributesNumber - 1);

    for (auto i = 0u; i < attributesNumber - 1; ++i) {
        if (i != classIndex) {
            threads.emplace_back(calculateValues, i);
        }
    }
    // Last attribute is calculated on the main thread
    // TODO If class is last attribute, main thread just waits.
    if ((attributesNumber - 1) != classIndex) {
        calculateValues(attributesNumber - 1);
    }

    for (auto &thread : threads) {
        thread.join();
    }

    return resultMatrix;
}