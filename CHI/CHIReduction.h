#pragma once

#include <cmath>

#include "Data/Data.h"

class CHIReduction
{
public:

    enum class ReductionMode {
        Average,
        Maximum
    };

    explicit CHIReduction(std::shared_ptr<const Data> &&data)
        : CHIReduction(data)
    { }

    explicit CHIReduction(const std::shared_ptr<const Data> &data)
        : m_data(data)
        , m_threshold(0.0)
    { }

    ~CHIReduction() = default;

    std::vector<double> reduce(ReductionMode mode);

    inline void setThreshold(const double threshold) { m_threshold = threshold; }
    inline double threshold() const { return m_threshold; }

private:
    using Matrix = std::vector<std::vector<double>>;

    static constexpr inline double goodnessMeasure(unsigned total,
                                                   unsigned tWithC,
                                                   unsigned tWithoutC,
                                                   unsigned cWithoutT,
                                                   unsigned norTnorC)
    {
        return (total * (tWithC * norTnorC + tWithoutC * cWithoutT) * (tWithC * norTnorC + tWithoutC * cWithoutT))
             / ((tWithC + cWithoutT) * (tWithoutC + norTnorC) * (tWithC + tWithoutC) * (cWithoutT + norTnorC));
    }

    Matrix calculateCHIMatrix() const;

    const std::shared_ptr<const Data> m_data;
    double m_threshold;
};
