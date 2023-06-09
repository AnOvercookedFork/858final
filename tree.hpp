#ifndef GENERATE_TREE_HPP
#define GENERATE_TREE_HPP

#include <vector>

using namespace std;

void generate_tree(vector<int> *adj, int n, int k, int h);
void generate_random_values(int *values, int n);
void generate_simple_tree(vector<int> *adj, int *values);
void print_tree(vector<int> *adj, int n);

#endif // GENERATE_TREE_HPP