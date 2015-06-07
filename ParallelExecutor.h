#pragma once

#include <functional>
#include <thread>
#include <vector>

#include "AtomicHelper.h"

/*
 * You can make your loop go multithreaded with:
 *
 * ParallelExecutor(beginIt, endIt) << yourFunction;
 *
 * Where yourFunction takes your iterator by value ( void(T) ). Iterator can be
 * an integral type, pointer or regular iterator. It will be copied and
 * incremented for each task. If possible (integrals, pointers) atomic will be
 * used for iterator, otherwise mutex will guard its incrementation. You must
 * assure that there will be no race conditions in yourFunction by yourself.
 *
 */

template <class T>
class ParallelExecutor
{
public:
    static constexpr const auto kReasonableDefaultNumberOfThreads = 4u;

    ParallelExecutor(const T &begin, const T &end)
        : m_it(begin)
        , m_end(end)
    {
        m_threads.reserve(kThreadsNumber);
    }


    ParallelExecutor(const ParallelExecutor &) = delete;
    ParallelExecutor(ParallelExecutor &&) = delete;
    ParallelExecutor & operator =(const ParallelExecutor &) = delete;
    ParallelExecutor & operator =(ParallelExecutor &&) = delete;

    ~ParallelExecutor() = default;

    inline void operator <<(const std::function<void(T)> &func) {
        if (m_it != m_end) {
            assert(kThreadsNumber > 0);
            LOG(DEBUG) << "Starting job on " << kThreadsNumber << " threads.";
            for (auto i = 0u; i < kThreadsNumber - 1; ++i) {
                m_threads.emplace_back(std::bind(&ParallelExecutor::threadLoop, this, std::cref(func)));
            }
            threadLoop(func);
            for (auto &thread: m_threads) {
                thread.join();
            }
        }
    }

private:
    inline void threadLoop(const std::function<void(T)> &func) {
        while (m_it != m_end) {
            func(m_it++);
        }
    }

    // You can set it to 1, to go single threaded.
    const unsigned kThreadsNumber = std::thread::hardware_concurrency() > 0u
                                 ? std::thread::hardware_concurrency()
                                 : kReasonableDefaultNumberOfThreads;
    AtomicIfPossible<T> m_it;
    const T &m_end;
    std::vector<std::thread> m_threads;
};
