# ConcurrentHashMap C++ Library [![Awesome](https://awesome.re/badge.svg)](https://github.com/diffstorm/ConcurrentHashMap)

[![CI](https://github.com/diffstorm/ConcurrentHashMap/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/diffstorm/ConcurrentHashMap/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Code Coverage](https://codecov.io/gh/diffstorm/ConcurrentHashMap/branch/main/graph/badge.svg)](https://codecov.io/gh/diffstorm/ConcurrentHashMap)
![GitHub Stars](https://img.shields.io/github/stars/diffstorm/ConcurrentHashMap?style=social)
![Platforms](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey)

A thread-safe hash map implementation in C++ with support for concurrent read and write operations.


## Overview


This is an attempt to create an equivalent of Java's ConcurrentHashMap in C++. It is not a direct equivalent because the underlying concurrency models in Java and C++ are different.


This C++ library provides a ConcurrentHashMap class that allows multiple threads to perform read and write operations on a hash map concurrently.


## Features

- **Thread-safe** read/write operations
- **Fine-grained locking** with `std::shared_mutex`
- **Modern C++17** API with `std::optional` returns
- **Efficient collision handling** using chaining
- **Comprehensive unit tests** (Google Test)
- **CMake** build system support


## Usage

1. **Include the Library:**
```cpp
#include "ConcurrentHashMap.h"
```
2. **Create an Instance:**
```cpp
ConcurrentHashMap<std::string, int> concurrentMap;
```
3. **Perform Operations:**

```cpp
// Insert a key-value pair
concurrentMap.insert("one", 1);

// Retrieve the value for a key
if(auto value = concurrentMap.get("one"))
    std::cout << "Value for key 'one': " << value << std::endl;
} else {
    std::cout << "Key 'one' not found." << std::endl;
}

// Remove a key-value pair
concurrentMap.remove("one");
```


## Advanced Features

Print entire map structure using `print()`.

Custom hash function support:
```
struct CustomHash { /* ... */ };
ConcurrentHashMap<KeyType, ValueType, CustomHash> customMap;
```


## Build
```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

## Running tests

### After CMake build
`cd build && ctest --verbose`

### Or directly
`./ConcurrentHashMapTest --gtest_color=yes`


## :snowman: Author
Eray Öztürk ([@diffstorm](https://github.com/diffstorm))


## License
This project is licensed under the MIT License - see the LICENSE file for details.

Feel free to contribute, report issues, or suggest improvements!


## Contributing
Contributions welcome! Please:
1. Fork the repository
2. Create your feature branch
3. Submit a Pull Request
4. Ensure all tests pass
