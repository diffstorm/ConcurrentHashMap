/*
    ConcurrentHashMap C++ Library
    A thread-safe hash map implementation in C++ with support for concurrent read and write operations.
    This C++ library provides a ConcurrentHashMap class that allows multiple threads to perform read and write operations on a hash map concurrently. It uses std::shared_timed_mutex to provide efficient and safe concurrent access.
    Author               : Eray Ozturk | erayozturk1@gmail.com
    URL                  : github.com/diffstorm
    Date                 : 10/06/2020
*/

#ifndef CONCURRENT_HASH_MAP_H
#define CONCURRENT_HASH_MAP_H

#include <iostream>
#include <vector>
#include <mutex>
#include <functional>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class ConcurrentHashMap {
private:
    // Define the structure of each node in the hash map
    struct Node {
        Key key;
        Value value;
        Node* next;
        Node(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
    };

    // Define the hash map buckets and associated mutexes
    std::vector<std::vector<Node*>> buckets;
    std::vector<std::mutex> mutexes;
    Hash hashFunction;

    // Get the mutex for a given key
    std::mutex& getMutex(const Key& key) {
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

        Node* current = buckets[index].empty() ? nullptr : buckets[index][0]; // Cast to Node*
        while (current != nullptr) {
            if (current->key == key) {
               current->value = value; // Update the value
               return; // Found the key
            }
            current = current->next; // No need to cast here
        }
        
        Node* newNode = new Node(key, value);
        newNode->next = buckets[index].empty() ? nullptr : buckets[index][0]; // Cast to Node*
        buckets[index] = {newNode};
    }

    // Retrieve the value associated with a key from the hash map
    bool get(const Key& key, Value& value) {
        std::unique_lock lock(getMutex(key));
        std::size_t hashValue = hashFunction(key);
        std::size_t index = hashValue % buckets.size();

        Node* current = buckets[index].empty() ? nullptr : buckets[index][0];  // Cast to Node*
        while (current != nullptr) {
            if (current->key == key) {
                value = current->value;
                return true; // Found the key
            }
            current = current->next;  // No need to cast here
        }

        return false; // Key not found
    }

    // Remove a key-value pair from the hash map
    void remove(const Key& key) {
        std::unique_lock lock(getMutex(key));
        std::size_t hashValue = hashFunction(key);
        std::size_t index = hashValue % buckets.size();

        Node* current = buckets[index].empty() ? nullptr : buckets[index][0];  // Cast to Node*
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    buckets[index][0] = current->next;  // No need to cast here
                } else {
                    prev->next = current->next;  // No need to cast here
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;  // No need to cast here
        }
    }

    // Print the contents of the hash map
    void print() {
        for (std::size_t i = 0; i < buckets.size(); ++i) {
            std::lock_guard lock(mutexes[i]);
            std::cout << "Bucket " << i << ": ";
            Node* current = buckets[i].empty() ? nullptr : buckets[i][0];  // Cast to Node*
            while (current != nullptr) {
                std::cout << "(" << current->key << ", " << current->value << ") ";
                current = current->next;  // No need to cast here
            }
            std::cout << std::endl;
        }
    }

    // Destructor to clean up memory
    ~ConcurrentHashMap() {
        for (auto& bucket : buckets) {
            Node* current = bucket.empty() ? nullptr : bucket[0];  // Cast to Node*
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;  // No need to cast here
                delete temp;
            }
        }
    }
};

#endif // CONCURRENT_HASH_MAP_H
