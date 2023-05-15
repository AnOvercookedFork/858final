# Parallel implementation of heavy-light decomposition

## Usage

```
make serial_hld
make parallel_hld

./serial_hld -n 1000000 -h 20

./parallel_hld -n 10000000 -h 24

./serial_hld -s -d
```

## Problem

We want to efficiently solve range queries on a tree. For instance, we want to find the
sum of all nodes along the path from node *A* to node *B*.

## Naive approach

1. Use DFS from node *A* to find node *B*: $O(n)$
2. Find the sum of nodes along the path: $O(n)$

## Better approach

1. Find the LCA of nodes *A* and *B*: $O(\log n)$ with binary lifting
2. Find the sum of nodes from *A* to LCA(*A*, *B*): $O(n)$
3. Find the sum of nodes from LCA(*A*, *B*) to *B*: $O(n)$

## Segment trees

The *segment tree* data structure allows us to solve range queries on a *list* in
$O(\log n)$ time. For instance, we can sum all the elements between element *X*
and element *Y* in $O(\log n)$.

Can we use segment trees to efficiently solve range queries on a tree?

We can *decompose* the tree into different *chains* (a chain is a path from an ancestor
node to a descendant node), and build a ST for each chain. During our range query,
any time we encounter a chain, we can query that chain in $O(\log n)$ time. Thus, for any
nodes *A* and *B*, our total time in the worst case is $O(\log n)$ * # of chains.

How to minimize # of chains?

## Heavy-light decomposition

Start with the root being a chain. At each node *u*, if *u* is the *heavy* child (it
has the largest subtree size of its sublings), continue the parent's chain. Otherwise,
*u* is a *light* child, and we start a new chain. We repeat this procedure with DFS.

This approach guarantees we end up with a maximum of $O(\log n)$ chains, resulting in
a $O(\log n)$ * $O(\log n)$ = $O(\log^2 n)$ range query in the worst case.

## Pseudocode

### Preprocessing

1. Calculate subtree size of each node: $O(n)$
1. Calculate kth ancestor of each node (binary lifting): $O(n \log n)$
1. Greedily decompose tree into chains using above approach: $O(n)$

### Querying

1. Find LCA(*A*, *B*) with binary lifting: $O(\log n)$
1. Find the sum of nodes from *A* to LCA(*A*, *B*) with ST: $O(\log^2 n)$
1. Find the sum of nodes from LCA(*A*, *B*) to *B* with ST: $O(\log^2 n)$ 

## Parallelizing

### Preprocessing

1. Construct Euler tour in parallel: $O(n)$ work, $O(\log n)$ depth
1. Calculate subtree size in parallel using ET: $O(n)$ work, $O(\log n)$ depth
1. Decompose in parallel using ET: $O(n)$ work, $O(\log n)$ depth
1. Construct RMQ sparse table in parallel: $O(n)$ work, $O(n)$ depth

### Querying

1. Find LCA(*A*, *B*): $O(1)$ work and depth
1. Find the sum of nodes from *A* to LCA(*A*, *B*): $O(\log^2 n)$ work and depth
1. Find the sum of nodes from LCA(*A*, *B*) to *B*: $O(\log^2 n)$ work and depth,

where steps 2 and 3 can be done in parallel.

## Implementation details

- Trees are represented using (`values`, `adj`) where:
    - `values` contains the value of each node *u*
    - `adj` contains the neighbors of each node *u*
    - Edges are *undirected*, so parents and children have each other in their adj. lists
- Instead of building a segment tree for each chain, we build a large segment tree
for the whole tree
    - We assign each node its corresponding index in the segment-tree array based on its chain
    - Keeps chains intact in the segment-tree array
    - Reduces ST construction overhead
