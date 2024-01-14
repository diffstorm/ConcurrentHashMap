# ConcurrentHashMap C++ Library


A thread-safe hash map implementation in C++ with support for concurrent read and write operations.


## Overview


This is an attempt to create an equivalent of Java's ConcurrentHashMap in C++. It is not a direct equivalent because the underlying concurrency models in Java and C++ are different.


This C++ library and its different versions provides a ConcurrentHashMap class that allows multiple threads to perform read and write operations on a hash map concurrently. It uses std::shared_timed_mutex to provide efficient and safe concurrent access.


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
int value;
if (concurrentMap.get("one", value)) {
    std::cout << "Value for key 'one': " << value << std::endl;
} else {
    std::cout << "Key 'one' not found." << std::endl;
}

// Remove a key-value pair
concurrentMap.remove("one");
```


## Additional Methods:

`print()`: Print the contents of the hash map.


## Build and Run Example
To compile the example test.cpp, you can use:
```
g++ -std=c++17 test.cpp -o test
```
And run it:
```
./test
```


## License
This project is licensed under the MIT License - see the LICENSE file for details.

Feel free to contribute, report issues, or suggest improvements!