# Lab 8
## Exercise 1
### scenario 1
Notice that in this case the program will only goes to label `wordZero` and execute `sw` operation (only this operation connects to memory).  \
The *Byte Offset* accounts for 3 bits and *Index* accounts for 2 bits. \
In each iteration, the address of the array is from $0x10008000$ to $0x10008080$ (array is stored in `HEAP` ). Every step jumps for a 32 Bytes, which increases the address by $0x20$ \
From this perspective, only the first cache line will be used. And it will miss all the time.

 1. Hit rate is 0
 2. Always 0
 3. Set step to 1, so that the Hit rate can be $64/128 = 0.5$

### scenario 2
In each step, the address increases by $0x8$
There are two instructions need to be considered: `lw` and `sw` and in one step the addresses are the same.

 1. The only reason for the miss of Hit is that the cache has not yet been written. There are 16 blocks, thus 16 misses. Therefore the result would be $(64-16)/64=0.75$
 2. It will increase, for the access increases, but the miss is still the same
 3. Use the method the <font color=red> cache blocking </font>

### scenario 3
1. The hit-range is about [0.1875, 0.5]
2. To set *Associativity* to 1 or 256, then we will obtain the constant Hit rate 0.5

## Exercise 2
When the original image is stored in memory, it is stored in row-major order, meaning that consecutive elements in the same row are stored together in memory. When the `gb_v()` function accesses the image data in column-major order, meaning consecutive elements in the same column are accessed, the access pattern is not contiguous in memory, which can lead to cache misses.

By transposing the image, the rows and columns are swapped, so the image data is now stored in column-major order. This means that the `gb_v()` function can access consecutive elements in the same column, which improves data locality and reduces cache misses.

## Explanation for *Cache Blocking*
First version:
```cpp
for (body1 = 0; body1 < NBODIES; body1 ++) {
   for (body2=0; body2 < NBODIES; body2++) {
     OUT[body1] += compute(body1, body2);
   }
}
```
Second Version:
```cpp
for (body2 = 0; body2 < NBODIES; body2 += BLOCK) {
   for (body1=0; body1 < NBODIES; body1 ++) {
      for (body22=0; body22 < BLOCK; body22 ++) {
         OUT[body1] += compute(body1, body2 + body22);
      }
   }
}
```
In the first code snippet, for each iteration of the outer loop, the inner loop iterates over all the bodies again, leading to multiple cache misses as the same data is fetched from memory repeatedly. This can be especially expensive when the number of bodies is large and the compute function is computationally expensive.

In the second code snippet, the outer loop now iterates over blocks of bodies, rather than individual bodies. This means that each iteration of the outer loop will load a block of bodies into the cache. The innermost loop then iterates over the bodies within the block, ensuring that each block is reused multiple times before being evicted from the cache. This leads to fewer cache misses and more efficient use of the cache, resulting in faster code execution.

Note that the size of the block should be chosen based on the cache size and the size of the data being processed. A block size that is too small may not provide enough locality improvement, while a block size that is too large may lead to thrashing, where cache evictions become too frequent.
