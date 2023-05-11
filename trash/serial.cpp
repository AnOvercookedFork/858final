#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
typedef struct TreeNode {
    int idx;
    int data;
    TreeNode* head;
    int pos;
    TreeNode* parent;
    int depth = 0;
    TreeNode* heavy;
    int size = 1;
    std::vector<TreeNode*> children;
} TreeNode;

typedef struct SegmentTree {
    std::vector<int> tree;
    int size;

    SegmentTree(int n) {
        size = n;
        tree.assign(4 * n, 0);
    }

    void build(const std::vector<int> &values, int idx, int left, int right) {
        if (left == right) {
            tree[idx] = values[left];
        } else {
            int mid = (left + right) / 2;
            build(values, idx * 2 + 1, left, mid);
            build(values, idx * 2 + 2, mid + 1, right);
            tree[idx] = tree[idx * 2 + 1] + tree[idx * 2 + 2];
        }
    }

    int query(int idx, int left, int right, int query_left, int query_right) {
        if (query_left <= left && right <= query_right) {
            return tree[idx];
        }
        if (query_right < left || right < query_left) {
            return 0;
        }
        int mid = (left + right) / 2;
        return query(idx * 2 + 1, left, mid, query_left, query_right) +
               query(idx * 2 + 2, mid + 1, right, query_left, query_right);
    }

    void update(int idx, int left, int right, int pos, int new_value) {
        if (left == right) {
            tree[idx] = new_value;
        } else {
            int mid = (left + right) / 2;
            if (pos <= mid) {
                update(idx * 2 + 1, left, mid, pos, new_value);
            } else {
                update(idx * 2 + 2, mid + 1, right, pos, new_value);
            }
            tree[idx] = tree[idx * 2 + 1] + tree[idx * 2 + 2];
        }
    }
} SegmentTree;

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void fisherYatesShuffle(std::vector<int> &arr) {
    srand(0);
    for (int i = arr.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(arr[i], arr[j]);
    }
}

void addEdge(TreeNode* nodes, int parentIdx, int childIdx) {
    TreeNode* parent = &nodes[parentIdx];
    TreeNode* child = &nodes[childIdx];
    parent->children.push_back(child);
    child->parent = parent;
}

TreeNode* generateRandomTree(int n) {
    std::vector<int> node_indices(n);
    for (int i = 0; i < n; i++) {
        node_indices[i] = i;
    }

    fisherYatesShuffle(node_indices);

    TreeNode* nodes = new TreeNode[n];
    for (int i = 0; i < n; i++) {
        nodes[i].idx = i;
        nodes[i].data = node_indices[i];
    }

    for (int i = 1; i < n; i++) {
        int parentIdx = rand() % i;
        addEdge(nodes, parentIdx, i);
    }

    return nodes;
}

void printTree(const TreeNode& tree) {
    std::cout << "idx: " << tree.idx << " data: " << tree.data << ", size: " << tree.size << ", heavy: " << (tree.heavy ? tree.heavy->data : -1);
    std::cout << " heavy head: " << tree.head->idx << " st pos " << tree.pos << " children: ";
    for (const TreeNode* child : tree.children) {
        std::cout << child->idx << " ";
    }
    std::cout << std::endl;

    for (const TreeNode* child : tree.children) {
        printTree(*child);
    }
}

// walks up tree to find the root
TreeNode* findRoot(TreeNode* tree) {
    while(tree->parent) {
        tree = tree->parent;
    }
    return tree;
}

// iterate through tree and update depth, size, and heaviness
// works because size is initialized to 1
void markHeavy(TreeNode& tree, int depth) {
    tree.depth = depth;

    // first iteration: calculate sizes and depths
    int heaviestWeight = 0;
    for (TreeNode* child : tree.children) {
        markHeavy(*child, depth + 1);
        tree.size += child->size;
        if (child->size > heaviestWeight) {
            tree.heavy = child;
            heaviestWeight = child->size;
        }
    }
}

int curPos = 0;
void decompose(TreeNode* v, TreeNode* h, std::vector<int>& values, std::vector<int>& order) {
    v->head = h;
    v->pos = curPos++;
    values.push_back(v->data);
    order.push_back(v->idx);
    if (v->heavy)
        decompose(v->heavy, h, values, order);
    for (TreeNode* c : v->children) {
        if (c != v->heavy)
            decompose(c, c, values, order);
    }
}

int query(TreeNode* a, TreeNode* b, SegmentTree& st, int n) {
    int res = 0;
    for (; a->head != b->head; b = b->head->parent) {
        if (a->head->depth > b->head->depth) {
            TreeNode* temp = a;
            a = b;
            b = temp;
        }
        int cur_heavy_path_max = st.query(0, 0, n-1, b->head->pos, b->pos);
        std::cout << "st query " << b->head->pos << "," << b->pos << "was " << cur_heavy_path_max << std::endl;
        res = std::max(res, cur_heavy_path_max);
    }
    if (a->depth > b->depth) {
        TreeNode* temp = a;
        a = b;
        b = temp;
    }
    int last_heavy_path_max = st.query(0, 0, n-1, a->pos, b->pos);
    std::cout << "st query " << a->pos << "," << b->pos << "was " << last_heavy_path_max << std::endl;
    res = std::max(res, last_heavy_path_max);
    return res;
}

int main(int argc, char* argv[]) {
    clock_t start = clock();

    int n = 5; // Desired number of nodes in the tree
    TreeNode* nodes = generateRandomTree(n);

    // printTree(*tree);
    // std::cout << std::endl;
    TreeNode* root = findRoot(nodes);

    clock_t end = clock();
    std::cout << ((double) (end - start)) / CLOCKS_PER_SEC << " seconds " << "for initialization for " << n << " nodes" << std::endl;
    markHeavy(*root, 0);
    std::vector<int> hldValues;
    std::vector<int> order;
    decompose(root, root, hldValues, order);
    printTree(*root);

    SegmentTree st = SegmentTree(n);
    st.build(hldValues, 0, 0, n-1);
    int u = 4;
    int v = 3;
    std::cout << query(&nodes[u], &nodes[v], st, n) << std::endl;
    std::cout << nodes[u].idx << " " << nodes[v].idx << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << order[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << hldValues[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << nodes[i].idx << " ";
    }
    std::cout << std::endl;

    end = clock();
    std::cout << ((double) (end - start)) / CLOCKS_PER_SEC << " seconds " << "for " << n << " nodes" << std::endl;

    delete[] nodes;
    return 0;
}