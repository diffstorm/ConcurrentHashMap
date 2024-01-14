/*
    ConcurrentHashMap C++ Library
    A thread-safe hash map implementation in C++ with support for concurrent read and write operations.
    This C++ library provides a ConcurrentHashMap class that allows multiple threads to perform read and write operations on a hash map concurrently. It uses std::shared_timed_mutex to provide efficient and safe concurrent access, also std::list within each bucket to handle collisions.
    Author               : Eray Ozturk | erayozturk1@gmail.com
    URL                  : github.com/diffstorm
    Date                 : 12/09/2020
*/

#ifndef CONCURRENT_HASH_MAP_H
#define CONCURRENT_HASH_MAP_H

#include <iostream>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <list>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class ConcurrentHashMap {
private:
    // Define the structure of each node in the hash map
    struct Node {
        Key key;
        Value value;
    };

    // Define the hash map buckets and associated mutexes
    std::vector<std::list<Node>> buckets;
    std::vector<std::shared_mutex> mutexes;
    Hash hashFunction;

    // Get the mutex for a given key
    std::shared_mutex& getMutex(const Key& key) {
        std::size_t hashValue = hashFunction(key);
        return mutexes[hashValue % mutexes.size()];
    }

public:
    explicit ConcurrentHashMap(std::size_t num_buckets = 16) : buckets(num_buckets), mutexes(num_buckets) {}

    // Insert a key-value pair into the hash map
    void insert(const Key& key, const Value& value) {
        std::unique_lock lock(getMutex(key));
        std::size_t hashValue = hashFunction(key);
        std::size_t index = hashValue % buckets.size();

        auto& bucket = buckets[index];
        auto it = std::find_if(bucket.begin(), bucket.end(), [&](const Node& node) { return node.key == key; });

        if (it != bucket.end()) {
            // Update existing key
            it->value = value;
        } else {
            // Insert new key-value pair
            bucket.push_back({key, value});
        }
    }

    // Retrieve the value associated with a key from the hash map
    bool get(const Key& key, Value& value) {
        std::shared_lock lock(getMutex(key));
        std::size_t hashValue = hashFunction(key);
        std::size_t index = hashValue % buckets.size();

        const auto& bucket = buckets[index];
        auto it = std::find_if(bucket.begin(), bucket.end(), [&](const Node& node) { return node.key == key; });

        if (it != bucket.end()) {
            value = it->value;
            return true; // Found the key
        }

        return false; // Key not found
    }

    // Remove a key-value pair from the hash map
    void remove(const Key& key) {
        std::unique_lock lock(getMutex(key));
        std::size_t hashValue = hashFunction(key);
        std::size_t index = hashValue % buckets.size();

        auto& bucket = buckets[index];
        bucket.remove_if([&](const Node& node) { return node.key == key; });
    }

    // Print the contents of the hash map
    void print() {
        for (std::size_t i = 0; i < buckets.size(); ++i) {
            std::shared_lock lock(mutexes[i]);
            std::cout << "Bucket " << i << ": ";
            const auto& bucket = buckets[i];
            for (const auto& node : bucket) {
                std::cout << "(" << node.key << ", " << node.value << ") ";
            }
            std::cout << std::endl;
        }
    }

    // Destructor to clean up memory
    ~ConcurrentHashMap() = default; // The default destructor is sufficient for cleanup.
};

#endif // CONCURRENT_HASH_MAP_H
