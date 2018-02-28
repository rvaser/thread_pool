# Thread pool

[![Latest GitHub release](https://img.shields.io/github/release/rvaser/thread_pool.svg)](https://github.com/rvaser/thread_pool/releases/latest)
![image](https://travis-ci.org/rvaser/thread_pool.svg?branch=master)

A c++ thread pool implementation inspired by https://github.com/progschj/ThreadPool.

## Dependencies

### Linux

Application uses following software:

1. gcc 4.8+ or clang 3.4+
2. cmake 3.2+

## Instalation

CmakeLists is provided in the project root folder. By running the following commands:

```bash
git clone https://github.com/rvaser/thread_pool thread_pool
cd thread_pool
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

a library named libthread_pool.a will appear in `build/lib` directory. To link the library with your code, add `-Iinclude/ -Lbuild/lib -lthread_pool -lpthread --std=c++11` while compiling and include `thread_pool/thread_pool.hpp` in your desired source files. Optionally, you can run `sudo make install` to install thread_pool library to your machine which lets you exclude `-Iinclude/ -Lbuild/lib` while compiling.

Alternatively, add the project to your CMakeLists.txt file with `add_subdirectory(vendor/thread_pool EXCLUDE_FROM_ALL)` and `target_link_libraries(your_exe thread_pool pthread)` commands.

## Usage

For details on how to use the thread pool, please look at the example bellow:

```cpp
// define some functions you want to execute in parallel
int function1(std::vector<int>& data, int index, ...) {
    ...
}
int function2(float a, float b) {
    ...
}
void function3(void) {
    ...
}

// create thread pool
std::shared_ptr<thread_pool::ThreadPool> thread_pool =
    thread_pool::createThreadPool(); // or pass number of threads you desire
// or std::unique_ptr<thread_pool::ThreadPool> ...

// create storage for return values of function1 and function2
std::vector<std::future<int>> thread_futures;
for (int i = 0; i < num_tasks; ++i) {
    // be sure to use std::ref() when passing references!
    thread_futures.emplace_back(thread_pool->submit_task(function1,
        std::ref(data), index, ...));
    thread_futures.emplace_back(thread_pool->submit_task(function2, a, b));
}

// wait for threads to finish
for (auto& it: thread_futures) {
    it.wait();
    // get return value with it.get();
}

// new set of tasks running function3
std::vector<std::future<void>> thread_futures2;
for (int i = 0; i < num_tasks2; ++i) {
    thread_futures2.emplace_back(thread_pool->submit_task(function3));
}
for (auto& it2: thread_futures2) {
    it.wait();
}
```
