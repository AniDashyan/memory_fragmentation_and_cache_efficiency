#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <string>

// memory tracker to monitor allocations
struct MemoryTracker {
    size_t total_alloc = 0;
    size_t active_allocs = 0;
    bool is_malloc = false; // Track if allocation is from malloc (fragmented) or vector (optimized)

    void* alloc(size_t size, bool useMalloc = false) {
        total_alloc += size;
        active_allocs++;
        is_malloc = useMalloc;
        return useMalloc ? std::malloc(size) : nullptr;
    }

    void dealloc(void* ptr, size_t size) {
        if (ptr && is_malloc) {
            total_alloc -= size;
            active_allocs--;
            std::free(ptr);
        } else if (ptr) {
            total_alloc -= size;
            active_allocs--;
        }
    }

    void print_results(const std::string& str) {
        std::cout << str << " - Total allocd: " << total_alloc
                  << " bytes, Active allocs: " << active_allocs << "\n";
    }
};

std::random_device rd;
std::mt19937 gen(rd());

// Random-sized allocs with malloc/free
void fragmented(MemoryTracker& tracker, int iters) {
    std::uniform_int_distribution<> sizes(16, 256);
    std::vector<char*> allocs;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < iters; ++i) {
        size_t size = sizes(gen);
        char* ptr = static_cast<char*>(tracker.alloc(size));
        allocs.push_back(ptr);

        if (allocs.size() > 50) {
            int idx = gen() % allocs.size();
            tracker.dealloc(allocs[idx], sizes(gen));
            allocs.erase(allocs.begin() + idx);
        }

        if (i % 1000 == 0) {
            tracker.print_results("Fragmented [Iter " + std::to_string(i) + "]");
        }
    }

    for (char* ptr : allocs) {
        tracker.dealloc(ptr, sizes(gen));
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Fragmented Time: " << duration << " ms\n";
}


void optimized(MemoryTracker& tracker, int iters) {
    const size_t obj_size = 64;
    const size_t pool_size = iters * obj_size;
    std::vector<char> pool(pool_size);
    std::vector<char*> freeList;
    std::vector<char*> allocs;
    size_t next_free = 0;

    // Initialize free list
    for (size_t i = 0; i < pool_size / obj_size; ++i) {
        freeList.push_back(pool.data() + i * obj_size);
    }

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < iters; ++i) {
        if (next_free >= freeList.size()) {
            std::cout << "Pool exhausted!\n";
            break;
        }

        char* ptr = freeList[next_free++];
        tracker.alloc(obj_size); // Track allocation
        allocs.push_back(ptr);

        if (allocs.size() > 50) {
            int idx = gen() % allocs.size();
            freeList.push_back(allocs[idx]);
            tracker.dealloc(allocs[idx], obj_size);
            allocs.erase(allocs.begin() + idx);
            next_free--;
        }

        if (i % 1000 == 0) {
            tracker.print_results("Optimized [Iter " + std::to_string(i) + "]");
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Optimized Time: " << duration << " ms\n";
}

int main() {
    const int iters = 10000;
    MemoryTracker tracker;

    std::cout << "Running Fragmented Version...\n";
    fragmented(tracker, iters);

    std::cout << "\nRunning Optimized Version...\n";
    tracker = MemoryTracker();
    optimized(tracker, iters);

    return 0;
}