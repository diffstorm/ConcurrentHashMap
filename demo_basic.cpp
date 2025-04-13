#include "ConcurrentHashMap.h"
#include <iostream>
#include <string>

int main()
{
    ConcurrentHashMap<std::string, int> concurrentMap;
    concurrentMap.insert("one", 1);
    concurrentMap.insert("two", 2);
    concurrentMap.insert("three", 3);

    if(auto value = concurrentMap.get("two"))
    {
        std::cout << "Value for key 'two': " << *value << "\n";
    }
    else
    {
        std::cout << "Key 'two' not found.\n";
    }

    concurrentMap.remove("two");

    if(auto value = concurrentMap.get("two"))
    {
        std::cout << "Value for key 'two': " << *value << "\n";
    }
    else
    {
        std::cout << "Key 'two' not found.\n";
    }

    concurrentMap.print();
    return 0;
}