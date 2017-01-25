#ifdef THREAD_POOL_MAIN_

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <vector>

#include "thread_pool.hpp"

using namespace THREAD_POOL;

void doSomeCalculations(std::vector<uint32_t>& data) {
    for (uint32_t i = 0; i < data.size() - 1; ++i) {
        data[i] = (data[i] * data[i + 1]) / (data[i] - data[i + 1]);
    }
}

void fillData(std::vector<std::vector<uint32_t>>& data, uint32_t data_length) {
    for (auto& it: data) {
        it.reserve(data_length);
        for (uint32_t i = 0; i < data_length; ++i) {
            it.push_back(i);
        }
    }
}

int main(int argc, char** argv) {

    std::vector<std::vector<uint32_t>> data(100);
    fillData(data, 1000000);

    uint32_t num_threads = argc > 1 ? atoi(argv[1]) : std::thread::hardware_concurrency() / 2;
    std::shared_ptr<ThreadPool> thread_pool = createThreadPool(num_threads);

    timeval start, stop;
    gettimeofday(&start, nullptr);

    std::vector<std::future<void>> thread_futures;
    for (uint32_t i = 0; i < data.size(); ++i) {
        thread_futures.emplace_back(thread_pool->submit_task(doSomeCalculations, std::ref(data[i])));
    }
    for (auto& it: thread_futures) {
        it.wait();
    }

    gettimeofday(&stop, nullptr);
    fprintf(stderr, "%u threads: %.5lf\n", thread_pool->num_threads(), (((stop.tv_sec - start.tv_sec) * 1000000L + stop.tv_usec)
        - start.tv_usec) / (double) 1000000);

    return 0;
}

#endif
