#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

vector<vector<int>> adj;
vector<int> parent, depth, heavy, head, pos;//, sizes;
int cur_pos;

void addEdge(int parentIdx, int childIdx) {
    adj[parentIdx].push_back(childIdx);
    parent[childIdx] = parentIdx;
}

void generateRandomTree(int n) {
    std::vector<int> node_indices(n);
    for (int i = 0; i < n; i++) {
        node_indices[i] = i;
    }

    for (int i = 1; i < n; i++) {
        int parentIdx = rand() % i;
        addEdge(node_indices[parentIdx], node_indices[i]);
    }
}

int dfs(int v, vector<vector<int>> const& adj) {
    int size = 1;
    int max_c_size = 0;
    for (int c : adj[v]) {
        if (c != parent[v]) {
            parent[c] = v, depth[c] = depth[v] + 1;
            int c_size = dfs(c, adj);
            size += c_size;
            if (c_size > max_c_size)
                max_c_size = c_size, heavy[v] = c;
        }
    }
    return size;
}

void decompose(int v, int h, vector<vector<int>> const& adj) {
    head[v] = h;
    pos[v] = cur_pos++;
    if (heavy[v] != -1)
        decompose(heavy[v], h, adj);
    for (int c : adj[v]) {
        if (c != parent[v] && c != heavy[v])
            decompose(c, c, adj);
    }
}

int query(int a, int b) {
    int res = 0;
    for (; head[a] != head[b]; b = parent[head[b]]) {
        if (depth[head[a]] > depth[head[b]])
            swap(a, b);
        int cur_heavy_path_max = segment_tree_query(pos[head[b]], pos[b]);
        res = max(res, cur_heavy_path_max);
    }
    if (depth[a] > depth[b])
        swap(a, b);
    int last_heavy_path_max = segment_tree_query(pos[a], pos[b]);
    res = max(res, last_heavy_path_max);
    return res;
}

int main(int argc, char* argv[]) {
    clock_t start = clock();

    int n = 10000000; // Desired number of nodes in the tree
    adj = vector<vector<int>>(n);
    parent = vector<int>(n);
    depth = vector<int>(n);
    heavy = vector<int>(n, -1);
    head = vector<int>(n);
    pos = vector<int>(n);
    // sizes = vector<int>(n);
    generateRandomTree(n);



    clock_t end = clock();
    cout << ((double) (end - start)) / CLOCKS_PER_SEC << " seconds " << "for initialization for " << n << " nodes" << endl;

    dfs(0, adj);
    decompose(0, 0, adj);
    cout << query(10, 20) << endl;

    end = clock();
    cout << ((double) (end - start)) / CLOCKS_PER_SEC << " seconds " << "for " << n << " nodes" << endl;

    return 0;
}