# Project5 Report

20160463



Cache 관련해서 생각

- https://code.google.com/archive/p/cpp-btree/ : 
  - For an in-memory data structure, the same property yields a performance boost by keeping cache-line misses to a minimum. C++ B-tree containers make better use of the cache by performing multiple key-comparisons per node when searching the tree. Although B-tree algorithms are more complex, compared with the Red-Black tree algorithms, the improvement in cache behavior may account for a **significant speedup** in accessing large containers.
- **cache line** is 64 bytes.
- 1 node가 64 byte size 이하가 되게!



CPU에서 제공해주는 Vectorized Operation

- https://stackoverflow.com/questions/1516622/what-does-vectorization-mean



B+ tree

- SCAN이 빠름
- SCAN : 인자로 준 값과 같거나 큰 최소 key에 대해 (lower_bound) 거기서부터 차례대로 value 더해서 리턴
- https://github.com/deepaktabraham/BPlus-Tree



Trie

- READ가 빠름
- https://twpower.github.io/187-trie-concept-and-basic-problem

