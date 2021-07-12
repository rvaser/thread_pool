# Thread pool

[![Latest GitHub release](https://img.shields.io/github/release/rvaser/thread_pool.svg)](https://github.com/rvaser/thread_pool/releases/latest)
![Build status for gcc/clang](https://github.com/rvaser/thread_pool/actions/workflows/thread_pool.yml/badge.svg)

ThreadPool is a c++ header only library combining https://github.com/progschj/ThreadPool and task stealing by Sean Parent.

## Usage

To build thread_pool run the following commands:
```bash
git clone https://github.com/rvaser/thread_pool && cd thread_pool && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && make
```
which will create install targets and unit tests. Running `make install` will create a package on your system that can be searched and linked with:
```cmake
find_package(thread_pool)
target_link_libraries(<target> thread_pool:thread_pool)
```
On the other hand, you can include thread_pool as a submodule and add it to your project with the following:
```cmake
if (NOT TARGET thread_pool)
  add_subdirectory(<path_to_submodules>/thread_pool EXCLUDE_FROM_ALL)
endif ()
target_link_libraries(<target> thread_pool::thread_pool)
```

If you are not using CMake, include the appropriate header file directly to your project and link with pthread.

#### Build options

- `thread_pool_install`: generate install target
- `thread_pool_build_tests`: build unit tests

#### Dependencies

- gcc 4.8+ | clang 3.5+
- pthread
- (optional) cmake 3.11+

###### Hidden

- (thread_pool_test) google/googletest 1.10.0

## Examples

```cpp
#include "thread_pool/thread_pool.hpp"

int function1(const T& t, ...) {
  ...
}
int function2(...) {
  ...
}
...
auto lambda1 = [...] (...) -> void {
  ...
};

thread_pool::ThreadPool thread_pool{};

std::vector<std::future<int>> futures;
for (...) {
  // be sure to used std::ref() or std::cref() for references
  futures.emplace_back(thread_pool.Submit(function1, std::cref(t), ...));
  futures.emplace_back(thread_pool.Submit(function2, ...));
}
for (auto& it : futures) {
  ... = it.get();
}

std::vector<std::future<void>> void_futures;
for (...) {
  void_futures.emplace_back(thread_pool.Submit(lambda1, ...));
}
for (const auto& it : void_futures) {
  it.wait();
}
```

## Acknowledgement

This work has been supported in part by the Croatian Science Foundation under the project Single genome and metagenome assembly (IP-2018-01-5886).
