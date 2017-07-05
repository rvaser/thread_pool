/*!
 * @file thread_pool_test.cpp
 *
 * @brief Thread_pool unit test source file
 */

#include <unordered_map>
#include <unordered_set>

#include "thread_pool/thread_pool.hpp"
#include "gtest/gtest.h"

class ThreadPoolTest: public ::testing::Test {
public:
    void SetUp() {
        thread_pool = thread_pool::createThreadPool();
    }

    void TearDown() {}

    std::unique_ptr<thread_pool::ThreadPool> thread_pool;
};

TEST(ThreadPoolTest_, CreateThreadPoolError) {
    EXPECT_DEATH((thread_pool::createThreadPool(0)),
        "thread_pool::createThreadPool error: invalid number of threads!");
}

TEST_F(ThreadPoolTest, ParallelCalculation) {

    std::vector<std::vector<uint32_t>> data(10);
    for (auto& it: data) {
        it.reserve(100000);
        for (uint32_t i = 0; i < 100000; ++i) {
            it.push_back(i);
        }
    }

    auto do_some_calculation = [](std::vector<uint32_t>& src) -> void {
        for (uint32_t i = 0; i < src.size() - 1; ++i) {
            src[i] = (src[i] * src[i + 1]) / (src[i] - src[i + 1] * 3);
        }
    };

    std::vector<std::future<void>> thread_futures;
    for (uint32_t i = 0; i < data.size(); ++i) {
        thread_futures.emplace_back(thread_pool->submit_task(do_some_calculation,
            std::ref(data[i])));
    }

    for (const auto& it: thread_futures) {
        it.wait();
    }
}

TEST_F(ThreadPoolTest, ThreadIdentifiers) {

    const auto& identifiers = thread_pool->thread_identifiers();
    std::unordered_map<std::thread::id, uint32_t> thread_map;
    uint32_t thread_id = 0;
    for (const auto& it: identifiers) {
        thread_map[it] = thread_id++;
    }

    EXPECT_EQ(thread_id, thread_map.size());
}
