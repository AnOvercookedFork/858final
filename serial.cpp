#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
typedef struct TreeNode {
    int idx;
    int data;
    TreeNode* parent;
    int depth = 0;
    TreeNode* heavy;
    int size = 1;
    std::vector<TreeNode*> children;
} TreeNode;

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
        addEdge(nodes, node_indices[parentIdx], node_indices[i]);
    }

    return nodes;
}

void printTree(const TreeNode& tree) {
    std::cout << "data: " << tree.data << ", size: " << tree.size << ", heavy: " << (tree.heavy ? tree.heavy->data : -1) << " children: ";
    for (const TreeNode* child : tree.children) {
        std::cout << child->data << " ";
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
    TreeNode* heaviest = nullptr;
    int heaviestWeight = 0;
    for (TreeNode* child : tree.children) {
        markHeavy(*child, depth + 1);
        tree.size += child->size;
        if (child->size > heaviestWeight) {
            heaviest = child;
            heaviestWeight = child->size;
        }
    }
    
    tree.heavy = heaviest;
}

// void fastDFS(int v, vector<vector<int>> const& adj) {
//     sizes[v] = 1;
//     // first iteration: calculate sizes and depths
//     int heaviest = 0; // 0 will never be a child
//     int heaviestWeight = 0;
//     for (int i : adj[v]) {
//         fastDFS(i, adj);
//         sizes[v] += sizes[i];
//         if (sizes[i] > heaviestWeight) {
//             heaviest = i;
//             heaviestWeight = sizes[i];
//         }
//     }

//     if (heaviest) {
//         heavy[v] = heaviest;
//     }
// }

int main(int argc, char* argv[]) {
    clock_t start = clock();

    int n = 20; // Desired number of nodes in the tree
    TreeNode* nodes = generateRandomTree(n);

    // printTree(*tree);
    // std::cout << std::endl;
    TreeNode* root = findRoot(nodes);

    clock_t end = clock();
    std::cout << ((double) (end - start)) / CLOCKS_PER_SEC << " seconds " << "for initialization for " << n << " nodes" << std::endl;
    markHeavy(*root, 0);
    printTree(*root);

    end = clock();
    std::cout << ((double) (end - start)) / CLOCKS_PER_SEC << " seconds " << "for " << n << " nodes" << std::endl;

    delete[] nodes;
    return 0;
}