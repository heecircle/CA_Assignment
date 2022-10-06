## Project #3: Cache Simulator

### *** Due on June 10 (Friday), 24:00 ***

### Goal

To better understand the memory hierarchy in computers, build a cache simulator with the write-back feature.

### Problem Specification

- We've learned how the memory hierarchy works and how much it influences on computer performance. This programming assignment is to implement a cache simulator which supports the set-associative configuration and the write-back feature.

- During the initialization, the simulator gets inputs for the cache configuration. Specifically, the number of blocks `(nr_blocks)` is for the number of cache blocks, and `number of ways (nr_ways)` is for the ways per set. `nr_sets`, the number of sets, is set by the simulator according to the inputted cache configuration.

- You can assume that the number of blocks is always the power of 2, and so does for the number of ways and the words in a block. No need to validate the configuration by yourself.

- The provided framework is basically a command-line interface by which you can instruct the simulator to process your input. Supported commands that the simulator can handle are as follows.
  - `lw @addr`: Simulate `lw` instruction. **COMPLETE THIS FEATURE**
  - `sw @addr @value`: Simulate `sw` instruction. **COMPLETE THIS FEATURE**
  - `show`:  Show cache blocks and their status
  - `dump @starting_addr`: Dump 64 bytes from memory starting from `@starting_addr`
  - `cycles`: Print out the number of simulated clock cycles
  - `quit`: Terminate the simulator

- Complete the simulator to make `lw` and `sw` work correctly. To this end, fill in **`load_word`** and **`store_word`** functions in the template code.
  - `load_word` is called by the framework in response to `lw` command. You should loads the data at the specified address on the memory into its corresponding cache block.
  - `store_word` is called by the framework in response to `sw` command. You should updates data at `@addr` to `@data`.
  - The `@data` to be stored or loaded is a word in size, and you may take care of the case (i.e., extending bits) when the input is shorter than a word.

- The cache should be the **write-back cache**.

- **Replace the least-recently-used cache block** for multi-way set-associative configuration. You can use `cycles` as the current timestamp.

- The framework will populate `struct cache_block *cache` to hold the cache blocks. In your implementation, you can access cache blocks from `cache[0]` to `cache[nr_blocks - 1]` which are in `struct cache_block` type. **DO NOT ALTER THE ALLOCATION CODE NOR POPULATE YOUR OWN SPACE FOR CACHE**.

- The words per block might be changed during the test (e.g., to contain 2 words per block instead of 4 words default). Write your code to handle the case.

- Like the previous PAs, you may use `printf` as you want, but **DO NOT** `fprintf` to `stderr`, since it can disturb the grading system.


### Hints

- The data is always moved between the cache and the memory in the cache block granularity.

- The cache is effectively the direct mapped cache when `nr_ways == 1` whereas the cache is effectively fully associative cache when `nr_ways == nr_blocks`.

- Suppose that you have an address `@addr` to handle and the cache is with 2^n sets (i.e., `nr_sets == 1 << n`). To obtain `n`, you can use `log2_discrete(nr_sets)` in the framework.

- Following operations might help you calculating some addresses.
  - `1 << 8          = 0b 0000 .... 0001 0000 0000`
  - `(1 << 8) - 1    = 0b 0000 .... 0000 1111 1111`
  - `~((1 << 8) - 1) = 0b 1111 .... 1111 0000 0000`

- Be careful about the orders of various operators. It is a very practical habit to put parentheses always if you are not 100% sure.


### Submission / Grading

- Use [PAsubmit](https://sslab.ajou.ac.kr/pasubmit) for submission
	- 320 pts + 10 pts
  - You can submit your code up to 30 times.
	- The results of some testcases are hidden.

- Source: pa3.c (300 pts)
	- Load word (50 pts)
	- Store word (50 pts)
	- Write-back feature (100 pts)
	- LRU (100 pts)

- Document: one PDF document (20 pts) including;
	- How you implement the least-recently-used replacement policy
	- How `load_word` is handled (explain hit and miss cases separately)
	- How `store_word` is handled (explain hit and miss cases separately)
	- Lesson learned
	- No more than three pages

- WILL NOT ANSWER THE QUESTIONS ABOUT THOSE ALREADY SPECIFIED ON THE HANDOUT.
