/*
 * Utility functions to generate different trees to test on.
 *
 * References:
 * - https://github.com/anudeep2011/programming/blob/master/qtree.cpp
 * - https://cp-algorithms.com/data_structures/segment_tree.html
 */

#include <vector>
#include <queue>
#include <cmath>
#include <iostream>

#include "tree.hpp"

using namespace std;

void add_child(vector<vector<int>> &adj, int parent, int child)
{
    adj[parent].push_back(child);
    adj[child].push_back(parent);
}

// Generates a tree with n nodes, k arity, and b balance
void generate_tree(vector<vector<int>> &adj, int n, int k, int h)
{
    if (n == 0 || k == 0)
    {
        return;
    }

    int num_nodes = 1;
    queue<int> q;
    q.push(0);

    // Generate tree using BFS
    for (int i = 0; i < h; i++)
    {
        int level_size = min(num_nodes * k, n) - num_nodes;
        for (int j = 0; j < level_size; j++)
        {
            int parent = q.front();
            q.pop();
            for (int l = 0; l < k && num_nodes < n; l++)
            {
                int child = num_nodes++;
                add_child(adj, parent, child);
                q.push(child);
            }
        }
    }
}

// Fills in values vector with random values
void generate_random_values(vector<int> &values, int n)
{
    int max_val = sqrt(__INT32_MAX__);
    for (int i = 0; i < n; i++)
    {
        values[i] = rand() % max_val;
    }
}

/* Generates this tree:
        0
       / \
      1   2
     / \   \
    3   4   5
   /|\  | \
  6 7 8 9  10

  Good for testing correctness:
  query_path(3, 5) = 11
  query_path(6, 5) = 17
  query_path(9, 4) = 13
  query_path(8, 2) = 14
  query_path(10, 6) = 24
  query_path(10, 9) = 23
*/
void generate_simple_tree(vector<vector<int>> &adj, vector<int> &values)
{
    add_child(adj, 0, 1);
    add_child(adj, 0, 2);
    add_child(adj, 1, 3);
    add_child(adj, 1, 4);
    add_child(adj, 2, 5);
    add_child(adj, 3, 6);
    add_child(adj, 3, 7);
    add_child(adj, 3, 8);
    add_child(adj, 4, 9);
    add_child(adj, 4, 10);

    for (int i = 0; i < 11; i++)
    {
        values[i] = i;
    }
}

// Prints the adjacency list for all nodes
void print_tree(vector<vector<int>> &adj, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << i << ": ";
        for (int j = 0; j < adj[i].size(); j++)
        {
            cout << adj[i][j] << ",";
        }
        cout << "\n";
    }
}