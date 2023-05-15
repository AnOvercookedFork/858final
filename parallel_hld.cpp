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
#include "parallel.h"
#include "get_time.h"
#include "primitives.h"
#include "sequence.h"

using namespace std;
using namespace parlay;

int n;
int log_n;

vector<int> *adj; // adjacency list representation of the tree
int *heavy;       // stores the heavy child of each node u
int *head;        // stores the head of the chain to which node u belongs
int *pos;         // stores the position of node u in the segment tree array
int *depth;       // stores the depth of node u
int *sizes;       // stores the subtree size of u
int *values;      // stores the value of node u
int **kth;        // stores the kth ancestor of node u -- kth[u][0] is u's parent

// Extra stuff for parallel
// vector<pair<int, int>> edges;

SegmentTree *st; // segment tree built on DFS traversal of tree
int cur_pos;     // used to assign positions to nodes in the segment tree array

void alloc_all()
{
    log_n = (int) log2(n);
    adj = (vector<int> *)malloc(n * sizeof(vector<int>)); // allocate memory for adj
    heavy = (int *)malloc(n * sizeof(int));               // allocate memory for heavy
    head = (int *)malloc(n * sizeof(int));                // allocate memory for head
    pos = (int *)malloc(n * sizeof(int));                 // allocate memory for pos
    depth = (int *)malloc(n * sizeof(int));               // allocate memory for depth
    sizes = (int *)malloc(n * sizeof(int));               // allocate memory for sizes
    values = (int *)malloc(n * sizeof(int));              // allocate memory for values
    kth = (int **)malloc(n * sizeof(int *));              // allocate memory for kth
    for (int i = 0; i < n; i++)
    {
        kth[i] = (int *)malloc(log_n * sizeof(int)); // allocate memory for each row of kth
    }
}


// Computes parent, depth, subtree size, and heavy child of each node using DFS
// O(n) time
void dfs(int v)
{
    sizes[v] = 1;
    heavy[v] = -1;
    for (int u : adj[v])
    {
        if (u != kth[v][0])
        {
            kth[u][0] = v;
            for (int i = 1; i < log_n; i++)
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

struct Edge {
    int u;
    int v;
    Edge* next;

    // Constructor with parameters
    Edge(int u, int v, Edge* next = nullptr) : u(u), v(v), next(next) {}
};

void euler_tour(int n)
{

    // IDEA: find succ in nested
    // 1. Construct a list of edges
    // NOTE: tabulate vs map??

    Edge *edges = (Edge *)malloc(n * sizeof(Edge));

    parlay::parallel_for(0, n, [&](int v){
        int deg = adj[v].size();

        parlay::parallel_for(0, deg, [&](int i){
            int u = adj[v][i];
            int w = adj[v][(i + 1) % deg];

            auto edge = Edge(u, v);
            auto next = new Edge(v, w);
            edge.next = next;

            edges[i] = edge;
        });
    });

    // auto nested = parlay::tabulate(n, [&](int v){
    //     for (int i = 0; i < adj[v].size(); i++)
    //     {
    //         auto u_i = adj[i];
    //         auto u_i_plus_one = adj[(i + 1) % adj[v].size()];

    //         edges.push_back(make_pair(u_i, v));
    //         succs.push_back(make_pair(v, u_i_plus_one));
    //     }
        
    //     return edges;
    // });

    // auto edges = parlay::flatten(nested);
    // parlay::sort_inplace(edges);

    

    // 2. Find successor of each edge
    // auto succ = vector<pair<int, int>>(n);



    // // 2. Find successor of each edge
    // auto succ = vector<pair<int, int>>(n);
    // parlay::parallel_for(0, n, [&](int i){
    //     auto edge = edges[i];
    //     return edge;
    // });



    // 2. 

    // for (auto edge : flattened)
    //     cout << edge.first << ", " << edge.second << endl;

    // 2. 

    // cout << adj.size() << "\n";


    // parlay::sort_inplace(edges);
    // parlay::sort_inplace(edges, [&](pair<int, int> a, pair<int, int> b){ return a.first < b.first; });

    // Sort the edge list lexicographically
    

    // for (int v : adj[u])
    // {
    //     if (v != kth[u][0])
    //     {
    //         et.push_back(make_pair(u, v));
    //         kth[v][0] = u;              // assign parent
    //         depth[v] = depth[u] + 1;    // assign depth
    //         euler_tour(v);
    //         et.push_back(make_pair(v, u));
    //     }
    // }
}

// Do parallel preprocessing on the tree
// 1. Construct Euler tour of tree in parallel
// 2. Calculate subtree size for each node in parallel
// 3. Do heavy-light decomposition in parallel
// 4. Build a segment tree on the nodes in parallel
void parallel_preprocess(int n)
{
    cur_pos = 0;
    euler_tour(n);

    // for (pair<int, int> edge : edges)
    // {
    //     cout << "(" << edge.first << ", " << edge.second << ")" << "\n";
    // }

    // dfs(0);
    // decompose(0, 0);
    // build_st(n);
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
    for (int i = log_n - 1; i >= 0; i--)
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
    for (int i = log_n - 1; i >= 0; i--)
    {
        if (kth[u][i] != kth[v][i])
        {
            u = kth[u][i];
            v = kth[v][i];
        }
    }
    return kth[u][0];
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
    res += st->query(pos[u], pos[lca_uv]);

    // sum from v to lca_uv
    while (head[v] != head[lca_uv])
    {
        res += st->query(pos[head[v]], pos[v]);
        v = kth[head[v]][0];
    }
    res += st->query(pos[v], pos[lca_uv]) - values[lca_uv];
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
    srand(0);


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

    if (simple)
        n = 11;

    ::n = n;
    alloc_all();

    // Generate tree
    if (simple)
        generate_simple_tree(adj, values);
    else
    {
        generate_tree(adj, n, k, h);
        generate_random_values(values, n);
    }

    // Print if debug
    if (debug)
        print_tree(adj, n);

    int num_rounds = 1;

    // Preprocess
    double total_time = 0;

    for (int i = 0; i < num_rounds; i++)
    {
        parlay::internal::timer t;
        parallel_preprocess(n);
        t.stop();
        cout << "Round " << i << " preprocess time: " << t.total_time() << endl;
        total_time += t.total_time();
    }

    cout << "Average preprocess time: " << total_time / num_rounds << endl;
    total_time = 0;

    return 0;

    // Query
    for (int i = 0; i < num_rounds; i++)
    {
        parlay::internal::timer t;
        if (simple)
        {
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
        }
        else
        {
            int u = (n - 1) / 2;
            int v = n - 1;

            int ans = query_path(u, v);

            if (i == 0)
                std::cout << "Total sum: " << ans << endl;
        }
        t.stop();
        cout << "Round " << i << " query time: " << t.total_time() << endl;
        total_time += t.total_time();
    }

    cout << "Average query time: " << total_time / num_rounds << endl;

    return 0;
}