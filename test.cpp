#include "ConcurrentHashMap.h"
#include <iostream>
#include <string>

int main() {
    ConcurrentHashMap<std::string, int> concurrentMap;

    concurrentMap.insert("one", 1);
    concurrentMap.insert("two", 2);
    concurrentMap.insert("three", 3);

    int value;

    if (concurrentMap.get("two", value)) {
        std::cout << "Value for key 'two': " << value << std::endl;
    } else {
        std::cout << "Key 'two' not found." << std::endl;
    }

    concurrentMap.remove("two");

    if (concurrentMap.get("two", value)) {
        std::cout << "Value for key 'two': " << value << std::endl;
    } else {
        std::cout << "Key 'two' not found." << std::endl;
    }

    concurrentMap.print();

    return 0;
}
