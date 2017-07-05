/*!
 * @file thread_pool_test.cpp
 *
 * @brief Thread_pool unit test source file
 */

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

TEST(ThreadPoolTest, CreateThreadPoolError) {
    EXPECT_DEATH((thread_pool::createThreadPool(0)),
        "thread_pool::createThreadPool error: invalid number of threads!");
}
