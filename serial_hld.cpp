/*
 * Serial implementation of heavy-light decomposition.
 *
 * References:
 * - https://github.com/anudeep2011/programming/blob/master/qtree.cpp
 * - https://cp-algorithms.com/graph/hld.html#implementation
 * - https://codeforces.com/blog/entry/81317
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include "getopt.h"

#include "serial_st.hpp"
#include "tree.hpp"

using namespace std;

const int N = 1e7;
const int LOG = 27;

vector<vector<int>> adj(N); // adjacency list representation of the tree
vector<int> heavy(N, -1);   // stores the heavy child of each node u
vector<int> head(N);        // stores the head of the chain to which node u belongs
vector<int> pos(N);         // stores the position of node u in the segment tree array
vector<int> depth(N);       // stores the depth of node u
vector<int> sizes(N);       // stores the subtree size of u
vector<int> values(N);      // stores the value of node u
int kth[N][LOG];            // stores the kth ancestor of node u -- kth[u][0] is u's parent
SegmentTree *st;            // segment tree built on DFS traversal of tree

int cur_pos; // used to assign positions to nodes in the segment tree array

// Computes parent, depth, subtree size, and heavy child of each node using DFS
// O(n) time
void dfs(int v)
{
    sizes[v] = 1;
    for (int u : adj[v])
    {
        if (u != kth[v][0])
        {
            kth[u][0] = v;
            for (int i = 1; i < LOG; i++)
            {
                kth[u][i] = kth[kth[u][i - 1]][i - 1];
            }
            depth[u] = depth[v] + 1;
            dfs(u);
            sizes[v] += sizes[u];
            if (heavy[v] == -1 || sizes[u] > sizes[heavy[v]])
            {
                heavy[v] = u;
            }
        }
    }
}

// Performs heavy-light decomposition greedily
// Assumes we have parent, depth, subtree size, and heavy child of each node
// Determines where the chains should be (assigns "head" for each node)
// O(n) time
void decompose(int v, int h)
{
    head[v] = h;        // make the current node's chain whatever was passed from parent
    pos[v] = cur_pos++; // assign position in segment tree array

    // If v has a heavy child, then do special case
    if (heavy[v] != -1)
    {
        // If v has a heavy child, continue the current chain by passing in h
        // h is the head of the current chain
        decompose(heavy[v], h);
    }

    // Otherwise, for all other children:
    for (int u : adj[v])
    {
        if (u != kth[v][0] && u != heavy[v]) // Ensure that u is a light child
        {
            // Create a new chain for u
            decompose(u, u);
        }
    }

    // Now, we have set "head" for each node properly
    // i.e. chains have been determined properly
}

// Builds a segment tree for entire tree based on node values (in DFS order)
void build_st(int n)
{
    // dfs_values contains node values (in DFS order)
    vector<int> dfs_values(n);
    for (int i = 0; i < n; i++)
    {
        dfs_values[pos[i]] = values[i];
    }

    st = new SegmentTree(dfs_values);
}

// Do preprocessing on the tree
// 1. Calculate subtree size for each node
// 2. Do heavy-light decomposition to determine location of chains
// 3. Build a segment tree on node values
void preprocess(int n)
{
    dfs(0);
    decompose(0, 0);
    build_st(n);
}

// Finds the LCA of nodes u and v
// O(log n)
int lca(int u, int v)
{
    if (depth[u] < depth[v])
    {
        swap(u, v);
    }
    int diff = depth[u] - depth[v];
    for (int i = LOG - 1; i >= 0; i--)
    {
        if ((1 << i) & diff)
        {
            u = kth[u][i];
        }
    }
    if (u == v)
    {
        return u;
    }
    for (int i = LOG - 1; i >= 0; i--)
    {
        if (kth[u][i] != kth[v][i])
        {
            u = kth[u][i];
            v = kth[v][i];
        }
    }
    return kth[u][0];
}

// "Old" way of doing query_path
int query_path_old(int u, int v)
{
    int res = 0; // accumulator

    // Traverses from u -> LCA(u, v), summing along the way
    while (head[u] != head[v])
    {
        if (depth[head[u]] < depth[head[v]])
        {
            swap(u, v); // Move up both u and v together to get to LCA
        }
        res += st->query(pos[head[u]], pos[u]); // Sum along current chain
        u = kth[head[u]][0];                    // Update u to be parent of current chain
    }

    // Ensures that u is the node with smaller depth
    // aka u is LCA(u, v)
    if (depth[u] > depth[v])
    {
        swap(u, v);
    }

    // Sums nodes from LCA(u, v) -> v
    // this is not the same v we started out with!
    res += st->query(pos[u], pos[v]);
    return res;
}

// "New" way of doing query_path, using binary lifting and LCA
// This is the method from the the slides/readme
// Verified to give the same results as query_path_old
int query_path(int u, int v)
{
    int res = 0;
    if (u > v)
    {
        swap(u, v);
    }

    int lca_uv = lca(u, v);

    // sum from u to lca_uv
    while (head[u] != head[lca_uv])
    {
        res += st->query(pos[u], pos[head[u]]);
        u = kth[head[u]][0];
    }
    res += st->query(pos[lca_uv], pos[u]);

    // sum from v to lca_uv
    while (head[v] != head[lca_uv])
    {
        res += st->query(pos[head[v]], pos[v]);
        v = kth[head[v]][0];
    }
    res += st->query(pos[lca_uv] + 1, pos[v]);

    return res;
}

// Update query for node u
// O(log n)
void update_node(int u, int val)
{
    st->update(pos[u], val); // Update the value at position pos[u] to val
}

// Driver code
int main(int argc, char *argv[])
{
    // Parse command line arguments
    int opt;
    bool simple = false, debug = false;
    int n = pow(2, 16) - 1;
    int h = 16;
    int k = 2;

    while ((opt = getopt(argc, argv, "sn:k:h:b:d")) != -1)
    {
        switch (opt)
        {
        case 's':
            simple = true;
            break;
        case 'n':
            n = atoi(optarg);
            break;
        case 'k':
            k = atoi(optarg);
            break;
        case 'h':
            h = atoi(optarg);
            break;
        case 'd':
            debug = true;
            break;
        }
    }

    // If "-s" flag is specified, then generate a "simple" tree
    if (simple)
    {
        // Generate tree and values
        int n = 11;
        generate_simple_tree(adj, values);

        // Preprocessing
        preprocess(n);

        // Queries
        cout << query_path(0, 1) << "\n";
        cout << query_path(0, 2) << "\n";
        cout << query_path(1, 2) << "\n";
        cout << query_path(3, 2) << "\n";
        cout << query_path(3, 4) << "\n";
        cout << query_path(3, 5) << "\n";
        cout << query_path(4, 5) << "\n";
        cout << query_path(6, 8) << "\n";
        cout << query_path(6, 7) << "\n";
        cout << query_path(6, 5) << "\n";
        cout << query_path(9, 4) << "\n";
        cout << query_path(8, 2) << "\n";
        cout << query_path(6, 0) << "\n";
        cout << query_path(10, 6) << "\n";
        cout << query_path(10, 9) << "\n";
        return 0;
    }

    // Otherwise, generate tree using command line args
    generate_tree(adj, n, k, h);

    // Print tree if debug
    if (debug)
        print_tree(adj, n);

    // Generate random values
    srand(0);
    generate_random_values(values, n);

    // Preprocess
    preprocess(n);

    // Queries
    for (int i = 0; i < 1000000; i++)
    {
        int u = rand() % n;
        int v = rand() % n;
        query_path(u, v);
        // cout << query_path(u, v) << "\n";
    }

    return 0;
}