#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
typedef struct TreeNode {
    int data;
    TreeNode* parent;
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
        nodes[i].data = node_indices[i];
    }

    for (int i = 1; i < n; i++) {
        int parentIdx = rand() % i;
        addEdge(nodes, node_indices[parentIdx], node_indices[i]);
    }

    return nodes;
}

void printTree(const TreeNode& tree) {
    std::cout << tree.data << ": ";
    for (const TreeNode* child : tree.children) {
        std::cout << child->data << " ";
    }
    std::cout << std::endl;

    for (const TreeNode* child : tree.children) {
        printTree(*child);
    }
}

TreeNode* findRoot(TreeNode* tree) {
    while(tree->parent) {
        tree = tree->parent;
    }
    return tree;
}

int main(int argc, char* argv[]) {
    int n = 100; // Desired number of nodes in the tree
    TreeNode* tree = generateRandomTree(n);

    printTree(*tree);
    std::cout << std::endl;
    printTree(*findRoot(tree));
    // Use the tree for testing your algorithm
    // ...

    delete[] tree;
    return 0;
}