# Fixed Size Buffer
1. Fixed-Size buffer is a stack allocated container with maximum capacity N determinded at compile time
2. ```new``` calls the OS heap allocator, introducing latency jitter 
3. Heap nodes are scattered randonly in RAM causing frequent CPU cache miss due to pointer chasing.

***A ```FixedBuffer<T,N>``` stores all elements in a flat contiguous block of stack memory inside ```std::array<T,N>```. It has zero heap allocation and 100% CPu L1/L2 cache locality.***

***Known limitation: pop() and clear() decrement size_ but don’t reset the vacated slot(s). For trivial types this is invisible; for an owning type like std::unique_ptr<T>, the object stays alive (referenced by a dangling slot) until overwritten by a future push or the buffer itself is destroyed. Found by the test suite — not yet fixed.***

