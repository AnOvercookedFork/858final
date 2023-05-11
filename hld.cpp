#include <iostream>
#include <vector>
#include "segment_tree.hpp"

using namespace std;

const int N = 100005;

vector<int> adj[N], heavy(N, -1), head(N), pos(N), parent(N), depth(N), sizes(N);
int cur_pos;

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

void decompose(int v, int h)
{
    head[v] = h;
    pos[v] = cur_pos++;
    if (heavy[v] != -1)
    {
        decompose(heavy[v], h);
    }
    for (int u : adj[v])
    {
        if (u != parent[v] && u != heavy[v])
        {
            decompose(u, u);
        }
    }
}

int query_path(SegmentTree &tree, int u, int v)
{
    int res = 0;
    while (head[u] != head[v])
    {
        if (depth[head[u]] < depth[head[v]])
        {
            swap(u, v);
        }
        res += tree.query(pos[head[u]], pos[u]);
        u = parent[head[u]];
    }
    if (depth[u] > depth[v])
    {
        swap(u, v);
    }
    res += tree.query(pos[u], pos[v]);
    return res;
}

void update_node(SegmentTree &tree, int u, int val)
{
    tree.update(pos[u], val);
}

int main()
{
    int n, q;
    cin >> n >> q;
    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1);
    decompose(1, 1);
    vector<int> values(n);
    SegmentTree tree(values);
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

// int main()
// {
//     vector<int> arr = {1, 3, 5, 7, 9, 11};
//     SegmentTree tree(arr);
//     cout << tree.query(1, 3) << endl; // outputs 15
//     tree.update(2, 6);
//     cout << tree.query(1, 3) << endl; // outputs 16
//     return 0;
// }