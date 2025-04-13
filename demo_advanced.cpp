#include "ConcurrentHashMap.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

std::mutex coutMutex;

void stressTest(ConcurrentHashMap<std::string, int> &map, int threadId)
{
    // Concurrent insertions/updates
    for(int i = 0; i < 10; ++i)
    {
        std::string key = "key-" + std::to_string(threadId) + "-" + std::to_string(i);
        map.insert(key, i);
        // Update shared key to test write contention
        map.insert("shared-key", threadId * 100 + i);
    }

    // Concurrent reads
    for(int i = 0; i < 5; ++i)
    {
        std::string key = "key-" + std::to_string(threadId) + "-" + std::to_string(i);

        if(auto val = map.get(key))
        {
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "Thread " << threadId << " found " << key << " = " << *val << "\n";
        }
    }
}

int main()
{
    ConcurrentHashMap<std::string, int> concurrentMap;
    std::atomic<bool> startFlag{false};
    std::mutex coutMutex;
    // === Basic Functionality Test ===
    std::cout << "=== Basic Operations ===\n";
    concurrentMap.insert("apple", 10);
    concurrentMap.insert("banana", 20);
    // Test duplicate insertion
    concurrentMap.insert("apple", 15);

    if(auto val = concurrentMap.get("apple"))
    {
        std::cout << "Apple: " << *val << " (should be 15)\n";
    }

    // Test non-existent key
    if(!concurrentMap.get("mango"))
    {
        std::cout << "Mango correctly not found\n";
    }

    concurrentMap.print();
    // === Concurrent Stress Test ===
    std::cout << "\n=== Starting Concurrent Stress Test ===\n";
    constexpr int NUM_THREADS = 8;
    std::vector<std::thread> threads;

    // Create worker threads
    for(int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back([ &, i]()
        {
            // Wait for all threads to be ready
            while(!startFlag)
            {
                std::this_thread::yield();
            }

            stressTest(concurrentMap, i);
        });
    }

    // Start all threads simultaneously
    startFlag = true;
    // Let threads work for 500ms
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // Concurrent modifications while threads are running
    concurrentMap.remove("apple");
    concurrentMap.insert("shared-key", -1);  // Last writer should win

    // Wait for all threads to finish
    for(auto &t : threads)
    {
        t.join();
    }

    // === Post-Test Verification ===
    std::cout << "\n=== Final Verification ===\n";

    // Verify shared key (last writer should be main thread)
    if(auto val = concurrentMap.get("shared-key"))
    {
        std::cout << "Shared key value: " << *val << " (should be -1)\n";
    }

    // Verify removed key
    if(!concurrentMap.get("apple"))
    {
        std::cout << "Apple correctly removed\n";
    }

    // Verify thread-specific keys
    bool allFound = true;

    for(int t = 0; t < NUM_THREADS; ++t)
    {
        for(int i = 0; i < 10; ++i)
        {
            std::string key = "key-" + std::to_string(t) + "-" + std::to_string(i);

            if(!concurrentMap.get(key))
            {
                allFound = false;
                std::cout << "Missing: " << key << "\n";
            }
        }
    }

    if(allFound)
    {
        std::cout << "All thread-specific keys preserved\n";
    }

    // Final print
    std::cout << "\n=== Final Map State ===\n";
    concurrentMap.print();
    return 0;
}