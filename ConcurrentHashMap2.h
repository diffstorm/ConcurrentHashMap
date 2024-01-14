/*
    ConcurrentHashMap C++ Library
    A thread-safe hash map implementation in C++ with support for concurrent read and write operations.
    This C++ library provides a ConcurrentHashMap class that allows multiple threads to perform read and write operations on a hash map concurrently. It uses std::shared_timed_mutex to provide efficient and safe concurrent access, also std::unordered_map to simplify the code.
    Author               : Eray Ozturk | erayozturk1@gmail.com
    URL                  : github.com/diffstorm
    Date                 : 01/08/2020
*/
#ifndef CONCURRENT_HASH_MAP_H
#define CONCURRENT_HASH_MAP_H

#include <unordered_map>
#include <shared_mutex>
#include <string>
#include <iostream>

template <typename Key, typename Value>
class ConcurrentHashMap {
private:
    std::unordered_map<Key, Value> map_;
    mutable std::shared_timed_mutex mutex_;

public:
    void insert(const Key& key, const Value& value) {
        std::unique_lock lock(mutex_);
        map_[key] = value;
    }

    bool get(const Key& key, Value& value) const {
        std::shared_lock lock(mutex_);
        auto it = map_.find(key);
        if (it != map_.end()) {
            value = it->second;
            return true;
        }
        return false;
    }

    void remove(const Key& key) {
        std::unique_lock lock(mutex_);
        map_.erase(key);
    }

    void print() const {
        std::shared_lock lock(mutex_);
        for (const auto& entry : map_) {
            std::cout << "Key: " << entry.first << ", Value: " << entry.second << std::endl;
        }
    }
	
	// Destructor to clean up memory
    ~ConcurrentHashMap() {
        // No explicit memory cleanup needed for std::unordered_map
    }
};

#endif // CONCURRENT_HASH_MAP_H
