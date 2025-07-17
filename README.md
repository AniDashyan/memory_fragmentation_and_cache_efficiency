# Memory Fragmentation and Cache Efficiency

## Overview
This project compares memory fragmentation and cache efficiency in an allocation-intensive workload. The fragmented version uses random-sized `std::malloc` allocations, causing heap fragmentation and poor cache performance. The optimized version uses fixed-size allocations from a contiguous `std::vector` buffer for better cache efficiency. Memory usage is tracked with a `MemoryTracker` struct, and execution times are measured to show performance differences.

## Build & Run
```bash
# Clone the Repository
git clone https://github.com/AniDashyan/memory_fragmentation_and_cache_efficiency
cd memory_fragmentation_and_cache_efficiency

# Build the Project
cmake -S . -B build
cmake --build build

# Run the Executable
./build/memory_frag
```

## Example Output
```bash
Running Fragmented Version...
Fragmented [Iter 0] - Total allocd: 29 bytes, Active allocs: 1
Fragmented [Iter 1000] - Total allocd: 131755 bytes, Active allocs: 1001
Fragmented [Iter 2000] - Total allocd: 266612 bytes, Active allocs: 2001
Fragmented [Iter 3000] - Total allocd: 404772 bytes, Active allocs: 3001
Fragmented [Iter 4000] - Total allocd: 541075 bytes, Active allocs: 4001
Fragmented [Iter 5000] - Total allocd: 677151 bytes, Active allocs: 5001
Fragmented [Iter 6000] - Total allocd: 813160 bytes, Active allocs: 6001
Fragmented [Iter 7000] - Total allocd: 948279 bytes, Active allocs: 7001
Fragmented [Iter 8000] - Total allocd: 1082843 bytes, Active allocs: 8001
Fragmented [Iter 9000] - Total allocd: 1216272 bytes, Active allocs: 9001
Fragmented Time: 13 ms

Running Optimized Version...
Optimized [Iter 0] - Total allocd: 64 bytes, Active allocs: 1
Optimized [Iter 1000] - Total allocd: 3200 bytes, Active allocs: 50
Optimized [Iter 2000] - Total allocd: 3200 bytes, Active allocs: 50
Optimized [Iter 3000] - Total allocd: 3200 bytes, Active allocs: 50
Optimized [Iter 4000] - Total allocd: 3200 bytes, Active allocs: 50
Optimized [Iter 5000] - Total allocd: 3200 bytes, Active allocs: 50
Optimized [Iter 6000] - Total allocd: 3200 bytes, Active allocs: 50
Optimized [Iter 7000] - Total allocd: 3200 bytes, Active allocs: 50
Optimized [Iter 8000] - Total allocd: 3200 bytes, Active allocs: 50
Optimized [Iter 9000] - Total allocd: 3200 bytes, Active allocs: 50
Optimized Time: 6 ms
```

## Explanation
The program demonstrates memory fragmentation and cache efficiency through two versions. The fragmented version allocates random-sized blocks (16–256 bytes) with `std::malloc`, deallocating every iteration when over 50 blocks are active, causing high fragmentation and cache misses, leading to slower performance. The optimized version allocates fixed-size (64-byte) blocks from a `std::vector<char>` buffer, deallocating similarly to reuse slots, minimizing fragmentation and improving cache efficiency for faster execution. The `MemoryTracker` struct tracks total allocated bytes and active allocations, using std::malloc/std::free for the fragmented version and counter updates for the optimized version, ensuring cross-platform compatibility. Execution times, measured with `std::chrono`, show the optimized version’s speed advantage due to contiguous memory and reduced allocator overhead.
