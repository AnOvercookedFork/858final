#ifndef SEGMENT_TREE_HPP
#define SEGMENT_TREE_HPP

#include <vector>

class SegmentTree {
public:
    SegmentTree(const std::vector<int>& arr);
    int query(int l, int r);
    void update(int idx, int val);
private:
    int n;
    std::vector<int> tree;

    void build(const std::vector<int>& arr, int v, int tl, int tr);
    int query(int v, int tl, int tr, int l, int r);
    void update(int v, int tl, int tr, int idx, int val);
};

#endif 