// Copyright (c) 2020 Robert Vaser

#include "thread_pool/thread_pool.hpp"

#include "gtest/gtest.h"

namespace thread_pool {
namespace test {

TEST(ThreadPoolThreadPoolTest, Submit) {
  std::function<std::size_t(std::size_t)> fibonacci =
      [&fibonacci] (std::size_t i) -> std::size_t {
    if (i == 1 || i == 2) {
      return 1;
    }
    return fibonacci(i - 1) + fibonacci(i - 2);
  };

  ThreadPool tp{};

  std::vector<std::future<std::size_t>> f;
  for (std::size_t i = 0; i < tp.num_threads(); ++i) {
    f.emplace_back(tp.Submit(fibonacci, 42));
  }
  for (auto& it : f) {
    EXPECT_EQ(267914296, it.get());
  }
}

TEST(ThreadPoolThreadPoolTest, ThreadIds) {
  ThreadPool tp{};
  EXPECT_EQ(tp.num_threads(), tp.thread_map().size());

  auto thread_id = [&] () -> std::size_t {
    return tp.thread_map().count(std::this_thread::get_id());
  };

  std::vector<std::future<size_t>> f;
  for (std::size_t i = 0; i < tp.num_threads() * 42; ++i) {
    f.emplace_back(tp.Submit(thread_id));
  }
  for (auto& it : f) {
    EXPECT_EQ(1, it.get());
  }
}

}  // namespace test
}  // namespace thread_pool
