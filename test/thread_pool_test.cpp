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

  struct Semaphore {
   public:
    void Wait() {
      std::unique_lock<std::mutex> lock(mutex);
      condition.wait(lock, [&] () -> std::size_t { return count; });
      --count;
    }

    void Signal() {
      std::unique_lock<std::mutex> lock(mutex);
      ++count;
      condition.notify_one();
    }

    std::mutex mutex;
    std::condition_variable condition;
    std::size_t count = 0;
  };

  std::vector<Semaphore> s(tp.num_threads());
  std::vector<Semaphore> b(tp.num_threads());

  auto check = [&] () -> void {
    EXPECT_EQ(1, tp.thread_map().count(std::this_thread::get_id()));
    auto i = tp.thread_map().at(std::this_thread::get_id());
    s[i].Signal();
    b[i].Wait();
  };

  std::vector<std::future<void>> f;
  for (std::size_t i = 0; i < tp.num_threads(); ++i) {
    f.emplace_back(tp.Submit(check));
  }
  for (auto& it : s) {
    it.Wait();
  }
  for (auto& it : b) {
    it.Signal();
  }
  for (const auto& it : f) {
    it.wait();
  }
}

}  // namespace test
}  // namespace thread_pool
