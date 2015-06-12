#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Data.h"

class DataAdapter
{
private:
    void calculateAdaptationVector(const std::function<bool(const Data::RowType &)> &adapter);

    void computeMinMaxValues() const;
    void computeClassProbabilities() const;

    const std::shared_ptr<const Data> m_data;
    std::vector<size_t> m_indices;
    mutable std::vector<std::pair<double, double>> m_minMaxValues;
    mutable std::vector<double> m_classProbabilities;

public:
    friend class const_iterator;

    class const_iterator {
        using IndexIterator = decltype(DataAdapter::m_indices)::const_iterator;

        const DataAdapter *m_adapter;
        IndexIterator m_indexIt;

        inline const_iterator(const DataAdapter *adapter, const IndexIterator &indexIt)
            : m_adapter(adapter)
            , m_indexIt(indexIt)
        {
        }

    public:
        friend class DataAdapter;

        const_iterator() = default;

        const_iterator(const const_iterator &) = default;

        ~const_iterator() = default;

        inline const_iterator & operator++()
        {
            ++m_indexIt;
            return *this;
        }

        inline const_iterator operator++(int)
        {
            const_iterator old(*this);
            ++*this;
            return old;
        }

        inline bool operator==(const const_iterator &other) const noexcept(noexcept(m_indexIt == other.m_indexIt))
        {
            return m_adapter == other.m_adapter && m_indexIt == other.m_indexIt;
        }

        inline bool operator!=(const const_iterator &other) const noexcept(noexcept(*this == other))
        {
            return !(*this == other);
        }

        inline auto operator*() const -> decltype(m_adapter->m_data->getRow(*m_indexIt))
        {
            return m_adapter->m_data->getRow(*m_indexIt);
        }

    };

    DataAdapter(const std::shared_ptr<const Data> &data, const std::function<bool(const Data::RowType &)> &adapter)
        : m_data(data)
    {
        calculateAdaptationVector(adapter);
    }

    DataAdapter(std::shared_ptr<const Data> &&data, const std::function<bool(const Data::RowType &)> &adapter)
        : m_data(std::move(data))
    {
        calculateAdaptationVector(adapter);
    }

    DataAdapter(const DataAdapter &) = default;
    DataAdapter(DataAdapter &&) = default;
    DataAdapter & operator=(const DataAdapter &) = default;
    DataAdapter & operator=(DataAdapter &&) = default;


    ~DataAdapter() = default;

    friend std::ostream & operator<<(std::ostream &, const DataAdapter &);
    void printData() const;

    inline const Data::RowType& getRow(size_t rowNumber) const { return m_data->getRow(m_indices[rowNumber]); }
    // Iterate over rows
    const_iterator begin() const { return cbegin(); }
    const_iterator end() const { return cend(); }
    const_iterator cbegin() const { return const_iterator(this, m_indices.cbegin()); }
    const_iterator cend() const { return const_iterator(this, m_indices.cend()); }

    const std::string& getClassAttributeHeader() const { return m_data->getClassAttributeHeader(); }
    int getClassIdx() const { return m_data->getClassIdx(); }
    inline size_t nRow() const noexcept(noexcept(m_indices.size())) { return m_indices.size(); }
    size_t nAttributes() const { return m_data->nRow(); }

    const std::set<int> getClassesValues() const { return m_data->getClassesValues(); }
    int getNumberOfClasses() const { return m_data->getNumberOfClasses(); }

    const std::vector<double> & getClassProbability() const;
    const std::vector<std::pair<double, double>> & getMinMaxValues() const;
};
