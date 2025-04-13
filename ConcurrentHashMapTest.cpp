#include "ConcurrentHashMap.h"
#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <atomic>
#include <string>

class ConcurrentHashMapTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        map_ = std::make_unique<ConcurrentHashMap<std::string, int>>();
    }

    std::unique_ptr<ConcurrentHashMap<std::string, int>> map_;
};

TEST_F(ConcurrentHashMapTest, BasicInsertAndRetrieve)
{
    map_->insert("apple", 10);
    auto result = map_->get("apple");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(10, *result);
}

TEST_F(ConcurrentHashMapTest, UpdateExistingKey)
{
    map_->insert("apple", 10);
    map_->insert("apple", 20);
    auto result = map_->get("apple");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(20, *result);
}

TEST_F(ConcurrentHashMapTest, RemoveKey)
{
    map_->insert("banana", 30);
    map_->remove("banana");
    auto result = map_->get("banana");
    EXPECT_FALSE(result.has_value());
}

TEST_F(ConcurrentHashMapTest, NonExistentKey)
{
    auto result = map_->get("mango");
    EXPECT_FALSE(result.has_value());
}

TEST_F(ConcurrentHashMapTest, ConcurrentInserts)
{
    constexpr int NUM_THREADS = 8;
    constexpr int ITEMS_PER_THREAD = 100;
    std::vector<std::thread> threads;

    for(int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back([this, i]
        {
            for(int j = 0; j < ITEMS_PER_THREAD; ++j)
            {
                std::string key = "thread" + std::to_string(i) + "-" + std::to_string(j);
                map_->insert(key, j);
            }
        });
    }

    for(auto &t : threads)
    {
        t.join();
    }

    // Verify all items were inserted
    for(int i = 0; i < NUM_THREADS; ++i)
    {
        for(int j = 0; j < ITEMS_PER_THREAD; ++j)
        {
            std::string key = "thread" + std::to_string(i) + "-" + std::to_string(j);
            auto result = map_->get(key);
            ASSERT_TRUE(result.has_value()) << "Missing key: " << key;
            EXPECT_EQ(j, *result);
        }
    }
}

TEST_F(ConcurrentHashMapTest, ConcurrentUpdates)
{
    constexpr int NUM_THREADS = 8;
    std::vector<std::thread> threads;
    std::atomic<bool> start{false};
    // Initial value
    map_->insert("contended", 0);

    for(int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back([this, &start, i]
        {
            while(!start) { /* spin */ }  // Wait for start signal

            for(int j = 0; j < 100; ++j)
            {
                map_->insert("contended", i * 100 + j);
            }
        });
    }

    start = true;

    for(auto &t : threads)
    {
        t.join();
    }

    // Verify the final value is from the last writer
    auto result = map_->get("contended");
    ASSERT_TRUE(result.has_value());
    // The exact value depends on thread scheduling, but it should be
    // from one of the threads (between 0*100+99 and 7*100+99)
    EXPECT_GE(*result, 99);
    EXPECT_LE(*result, 799);
}

TEST_F(ConcurrentHashMapTest, ConcurrentReadWrite)
{
    constexpr int NUM_WRITERS = 4;
    constexpr int NUM_READERS = 4;
    std::atomic<bool> running{true};
    std::vector<std::thread> writers;
    std::vector<std::thread> readers;

    // Writers constantly update values
    for(int i = 0; i < NUM_WRITERS; ++i)
    {
        writers.emplace_back([this, i, &running]
        {
            while(running)
            {
                map_->insert("key" + std::to_string(i), i);
            }
        });
    }

    // Readers constantly read values
    for(int i = 0; i < NUM_READERS; ++i)
    {
        readers.emplace_back([this, &running]
        {
            while(running)
            {
                for(int j = 0; j < NUM_WRITERS; ++j)
                {
                    auto result = map_->get("key" + std::to_string(j));
                }
            }
        });
    }

    // Let them run for 500ms
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    running = false;

    for(auto &t : writers)
    {
        t.join();
    }

    for(auto &t : readers)
    {
        t.join();
    }

    // Verify final values are from writers
    for(int i = 0; i < NUM_WRITERS; ++i)
    {
        auto result = map_->get("key" + std::to_string(i));
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(i, *result);
    }
}

TEST_F(ConcurrentHashMapTest, HighContentionSingleBucket)
{
    // Create map with only 1 bucket to maximize contention
    ConcurrentHashMap<int, int> singleBucketMap(1);
    constexpr int NUM_OPERATIONS = 1000;
    std::vector<std::thread> threads;
    std::atomic<int> counter{0};

    for(int i = 0; i < 8; ++i)
    {
        threads.emplace_back([&]
        {
            for(int j = 0; j < NUM_OPERATIONS; ++j)
            {
                int key = j % 10;  // Only 10 different keys
                singleBucketMap.insert(key, ++counter);
                singleBucketMap.get(key);

                if(j % 10 == 0)
                {
                    singleBucketMap.remove(key);
                }
            }
        });
    }

    for(auto &t : threads)
    {
        t.join();
    }

    // Verify final state
    for(int i = 0; i < 10; ++i)
    {
        auto result = singleBucketMap.get(i);

        if(result)
        {
            EXPECT_GT(*result, 0);
            EXPECT_LE(*result, counter.load());
        }
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}