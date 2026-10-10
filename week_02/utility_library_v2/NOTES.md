# MinHeap

Minheap is a heap data structue where the every parent node is either less than or equal to the value of it's child nodes``data[parent]<=data[child]``.
This invariant gaurantees that the smallest number in the heap will be at index 0.

### *Q.* Why do we use MinHeap in HFT?
*A.* Every individual order has it's own expiration timeout,finding the next order that will expire requres finding the smallest timestamp. A MinHeap let's us inspect entire dataset in O(1) time without sorting entire data set.

## Architecture and array-indexing

A binary heap is represented as a **complete binary tree** mapped flattened into a contiguous array:

```
Tree View:
          [ 5 ] (Index 0 - Root / Min Element)
         /     \
    [ 12 ]     [ 8 ]
    /    \
 [ 15 ]  [ 20 ]

 Array Representation:
Index:   | 0 | 1  | 2 | 3  | 4  | 
Element: | 5 | 12 | 8 | 15 | 20 |
```
For any element at index `i`:

* **parent(i)** = `(i - 1) / 2`
* **left_child(i)** = `2 * i + 1`
* **right_child(i)** = `2 * i + 2`

## Scope and No-Scope

1. In Scope :
    * Array backend complete binary tree storage(`std::vector<T>` or underlying buffer)
    * Index navigation helper functions(`parent`,`left_child`,`right_child`).
    * Core operations: `push()`,`pop()`,`top()`,`empty()`, `size()`.
2. Non Scope : 
    * Pointer-based tree nodes (pointer-chasing creates CPU cache misses).




