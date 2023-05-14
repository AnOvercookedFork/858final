#ifndef GENERATE_TREE_HPP
#define GENERATE_TREE_HPP

#include <vector>

using namespace std;

void generate_tree(vector<vector<int>> &adj, int n, int h, int k, double b);
void generate_random_values(vector<int> &values, int n);
void generate_simple_tree(vector<vector<int>> &adj, vector<int> &values);
void print_tree(vector<vector<int>> &adj, int n);

#endif // GENERATE_TREE_HPP