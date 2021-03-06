#pragma once

#include <cmath>
#include <memory>
#include <algorithm>

#include "Data/DataAdapter.h"

class CHIReduction
{
public:

    enum class ReductionMode {
        Average,
        Maximum
    };

    explicit CHIReduction(std::shared_ptr<const DataAdapter> &&data) noexcept
        : CHIReduction(data)
    { }

    explicit CHIReduction(const std::shared_ptr<const DataAdapter> &data) noexcept
        : m_data(data)
        , m_threshold(0.0)
    { }

    ~CHIReduction() = default;

    std::vector<double> reduce(ReductionMode mode);

    inline void setThreshold(const double threshold) noexcept { m_threshold = threshold; }
    inline double threshold() const noexcept { return m_threshold; }

private:
    using Matrix = std::vector<std::vector<double>>;

    static constexpr inline double goodnessMeasure(double total,
                                                   double tWithC,
                                                   double tWithoutC,
                                                   double cWithoutT,
                                                   double norTnorC) noexcept
    {
        return (total * (tWithC * norTnorC + tWithoutC * cWithoutT) * (tWithC * norTnorC + tWithoutC * cWithoutT))
             / ((tWithC + cWithoutT) * (tWithoutC + norTnorC) * (tWithC + tWithoutC) * (cWithoutT + norTnorC));
    }

    Matrix calculateCHIMatrix() const;

    const std::shared_ptr<const DataAdapter> m_data;
    double m_threshold;
};
