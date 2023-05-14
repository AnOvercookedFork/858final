# Parallel implementation of heavy-light decomposition

## Problem

We want to efficiently solve range queries on a tree. For instance, we want to find the
sum of all nodes along the path from node *A* to node *B*.

## Naive approach

1. Use DFS from node *A* to find node *B*
2. Find the sum of nodes along the path

## Better approach

1. Find the LCA of nodes *A* and *B*
2. Find the sum of nodes from *A* to LCA(*A*, *B*)
3. Find the sum of nodes from LCA(*A*, *B*) to *B*

## Segment trees

The *segment tree* data structure allows us to solve range queries on a *list* in
O(log *n*) time. For instance, we can sum all the elements between element *X*
and element *Y* in O(log *n*).

Can we use segment trees to solve range queries on a tree?

We can *decompose* the tree into different *chains* (a chain is a path from an ancestor
node to a descendant node), and build a ST for each chain. During our range query,
any time we encounter a chain, we can query that chain in O(log *n*) time. Thus, our 
total query time is O(log *n*) * # of chains. How to minimize # of chains?

## Heavy-light decomposition

Greedy approach where at each node, we continue the current chain if the current node
is a *heavy* child (it has the largest subtree size of its siblings). Otherwise, we
create a new chain for the current node. This approach guarantees we end up with
O(log *n*) chains, resulting in O(log *n*) * O(log *n*) = O(log^2 *n*) query time
in the worst case.

## Pseudocode

### Preprocessing

1. Calculate subtree size of each node - O(*n*)
1. Greedily decompose tree into chains using above approach - O(*n*)
1. Binary lifting - O(*n* log *n*)

### Querying

1. Find LCA(*A*, *B*) - O(log *n*) with binary lifting
1. Find the sum of nodes from *A* to LCA(*A*, *B*) - O(log^2 *n*)
1. Find the sum of nodes from LCA(*A*, *B*) to *B* - O(log^2 *n*)

## Parallelizing

### Preprocessing

1. Construct Euler tour in parallel - O(*n*) work, O(log *n*) depth
1. Calculate subtree size in parallel using ET - O(*n*) work, O(log *n*) depth
1. Decompose in parallel using ET - O(*n*) work, O(log *n*) depth
1. Construct RMQ sparse table in parallel - O(*n*) work, O(*n*) depth

### Querying

1. Find LCA(*A*, *B*) - O(1) work and depth
1. Find the sum of nodes from *A* to LCA(*A*, *B*) - O(log^2 *n*) work and depth
1. Find the sum of nodes from LCA(*A*, *B*) to *B* - O(log^2 *n*) work and depth,

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