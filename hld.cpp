// References:
// https://github.com/anudeep2011/programming/blob/master/qtree.cpp
// https://cp-algorithms.com/graph/hld.html#implementation

#include <iostream>
#include <vector>
#include "segment_tree.hpp"

using namespace std;

const int N = 100005;

vector<int> adj[N];       // adjacency list representation of the tree
vector<int> heavy(N, -1); // stores the heavy child of each node u
vector<int> head(N);      // stores the head of the chain to which node u belongs
vector<int> pos(N);       // stores the position of node u in the segment tree array
vector<int> parent(N);    // stores the parent of node u
vector<int> depth(N);     // stores the depth of node u
vector<int> sizes(N);     // stores the subtree size of u

int cur_pos; // used to assign positions to nodes in the segment tree array

// Computes parent, depth, subtree size, and heavy child of each node using DFS
// O(n) time
void dfs(int v)
{
    sizes[v] = 1;
    for (int u : adj[v])
    {
        if (u != parent[v])
        {
            parent[u] = v;
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
        if (u != parent[v] && u != heavy[v]) // Ensure that u is a light child
        {
            // Create a new chain for u
            decompose(u, u);
        }
    }

    // Now, we have set "head" for each node properly
    // i.e. chains have been determined properly
}

// Performs range query on the path between nodes u and v, i.e. Sum(A, B) from slides
// O(log^2 n)
int query_path(SegmentTree &tree, int u, int v)
{
    int res = 0; // accumulator

    // Traverses from u -> LCA(u, v), summing along the way
    while (head[u] != head[v])
    {
        if (depth[head[u]] < depth[head[v]])
        {
            swap(u, v); // Move up both u and v together to get to LCA
        }
        res += tree.query(pos[head[u]], pos[u]); // Sum along current chain
        u = parent[head[u]];                     // Update u to be parent of current chain
    }

    // Ensures that u is the node with smaller depth
    // aka u is LCA(u, v)
    if (depth[u] > depth[v])
    {
        swap(u, v);
    }

    // Sums nodes from LCA(u, v) -> v
    // this is not the same v we started out with!
    res += tree.query(pos[u], pos[v]);
    return res;
}

// Update query for node u
// O(log n)
void update_node(SegmentTree &tree, int u, int val)
{
    tree.update(pos[u], val); // Update the value at position pos[u] to val
}

// Driver code for building tree from cin
int main()
{
    int n; // number of nodes
    int q; // number of queries
    cin >> n >> q;

    // edges
    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // run dfs from root node (1)
    dfs(1);

    // run decompose from root node (1)
    // start with just 1 chain
    decompose(1, 1);

    // create a segment tree from values (initially no values)
    vector<int> values(n);
    SegmentTree tree(values);

    // run range and update queries q times
    while (q--)
    {
        char type;
        int u, v;
        cin >> type >> u >> v;
        if (type == 'Q')
        {
            cout << query_path(tree, u, v) << endl;
        }
        else
        {
            update_node(tree, u, v);
        }
    }
    return 0;
}
