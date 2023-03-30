# CacheSimulator

- Project Description:

In this project, you need to implement a simple cache simulator that takes as an input the
configurations of the cache to simulate, such as: size, associativity and replacement policy.
When you run you simulator, you need to additionally provide the path of the trace file that
includes the memory accesses.

Your simulator will parse the trace file, which looks like:

R 0x2356257
W 0x257777

Each line consists of two parts, the operation type (read or write) and byte address in hexadecimal.

After reading each line, the simulator will simulate the impact of that access on the cache state, e.g., the LRU state of the accessed set and the current valid blocks in the set. Your simulator needs to maintain information such as hits, misses and other useful statistics throughout the whole run.

In this project, you need to implement two different cache replacement policies: LRU and FIFO.

In LRU, the least-recently-used element gets evicted, whereas in FIFO, the element that was inserted the earliest gets evicted.


- Inputs to Simulator

The name of your executable should be SIM, and your simulator should take inputs as following:
  ./SIM <CACHE_SIZE> <ASSOC> <REPLACEMENT> <WB> <TRACE_FILE>
  <CACHE_SIZE> is the size of the simulated cache in bytes
  <ASSOC> is the associativity
  <REPLACEMENT> replacement policy: 0 means LRU, 1 means FIFO
  <WB> Write-back policy: 0 means write-through, 1 means write-back
  <TRACE_FILE> trace file name with full path
Example:
  ./SIM 32768 8 1 1 /home/TRACES/MCF.t
  This will simulate a 32KB write-back cache with 8-way associativity and FIFO replacement
  policy. The memory trace will be read from /home/TRACES/MCF.t

Note: the trace file will contain addresses that can be for 64-bit system, so you might need data
types that are large enough to read them correctly and bookkeep the metadata in your simulator.
For example, if the tag is 9 bytes and you allocate your tag array bookkeeping array as an array of
integers, you will not be able to store the whole 9 bytes; integer is only 4 bytes. Accordingly, use
data types such as long long int and its equivalents in other languages.

- Output from Simulator:

The following outputs are expected from your simulator:

  a. The total miss ratio for L1 cache
  b. The # writes to memory
  c. The # reads from memory
