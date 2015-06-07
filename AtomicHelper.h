#pragma once

#include <atomic>
#include <mutex>
#include <type_traits>

/*
 * Proper class is defined at the bottom: AtomicIfPossible
 */

/******* Internal, not for use **********/

struct has_atomic_preincrement {
    template<class T>
    static auto test(int) -> decltype(std::declval<typename std::atomic<T>>().operator++(), std::true_type());

    template<class>
    static auto test(...) -> std::false_type;
};

template<typename T, typename U = void>
class AtomicInternal {
    std::mutex m_mutex;
    T m_value;

public:
    // No virtual functions because it should not be exposed
    explicit AtomicInternal(T &&value) : m_value(std::move(value)) { }

    explicit AtomicInternal(const T &value) : m_value(value) { }

    AtomicInternal(const AtomicInternal &) = delete;
    AtomicInternal(AtomicInternal &&) = default;
    AtomicInternal & operator =(const AtomicInternal &) = delete;
    AtomicInternal & operator =(AtomicInternal &&) = default;

    ~AtomicInternal() = default;

    auto operator++() -> decltype(++AtomicInternal::m_value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return ++m_value;
    }

    auto operator++(int) -> decltype(AtomicInternal::m_value++) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_value++;
    }

    auto operator ==(const T &other) -> decltype(AtomicInternal::m_value == other) { return m_value == other; }
    auto operator !=(const T &other) -> decltype(AtomicInternal::m_value != other) { return m_value != other; }

    operator T() { return m_value; }
};

template<typename T>
class AtomicInternal
        <T, typename std::enable_if<decltype(has_atomic_preincrement::test<T>(0))::value, void>::type > {
    std::atomic<T> m_atomic;

public:
    // No virtual functions because it should not be exposed
    explicit AtomicInternal(T &&value) : m_atomic(std::move(value)) { }
    explicit AtomicInternal(const T &value) : m_atomic(value) { }

    AtomicInternal(const AtomicInternal &) = delete;
    AtomicInternal(AtomicInternal &&) = default;
    AtomicInternal & operator =(const AtomicInternal &) = delete;
    AtomicInternal & operator =(AtomicInternal &&) = default;

    ~AtomicInternal() = default;

    auto operator ++() -> decltype(++AtomicInternal::m_atomic) { return ++m_atomic; }
    auto operator ++(int) -> decltype(AtomicInternal::m_atomic++) { return m_atomic++; }

    auto operator ==(const T &other) -> decltype(AtomicInternal::m_atomic == other) { return m_atomic == other; }
    auto operator !=(const T &other) -> decltype(AtomicInternal::m_atomic != other) { return m_atomic != other; }

    operator T() { return m_atomic; }
};


/*********** END of Internal *************/

template <typename T>
class AtomicIfPossible : private AtomicInternal<T, void> {
public:
    using AtomicInternal<T>::AtomicInternal;

    using AtomicInternal<T>::operator++;
    using AtomicInternal<T>::operator==;
    using AtomicInternal<T>::operator!=;
    using AtomicInternal<T>::operator T;
};
