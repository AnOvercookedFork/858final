#include <vector>
#include <iostream>

#include "segment_tree.hpp"

using namespace std;

SegmentTree::SegmentTree(const std::vector<int> &arr)
{
    n = arr.size();
    tree.resize(4 * n);
    build(arr, 1, 0, n - 1);
}

void SegmentTree::build(const std::vector<int> &arr, int v, int tl, int tr)
{
    if (tl == tr)
    {
        tree[v] = arr[tl];
    }
    else
    {
        int tm = (tl + tr) / 2;
        build(arr, 2 * v, tl, tm);
        build(arr, 2 * v + 1, tm + 1, tr);
        tree[v] = tree[2 * v] + tree[2 * v + 1];
    }
}

int SegmentTree::query(int l, int r)
{
    return query(1, 0, n - 1, l, r);
}

int SegmentTree::query(int v, int tl, int tr, int l, int r)
{
    if (l > r)
    {
        return 0;
    }
    if (l == tl && r == tr)
    {
        return tree[v];
    }
    int tm = (tl + tr) / 2;
    return query(2 * v, tl, tm, l, std::min(r, tm)) +
           query(2 * v + 1, tm + 1, tr, std::max(l, tm + 1), r);
}

void SegmentTree::update(int idx, int val)
{
    update(1, 0, n - 1, idx, val);
}

void SegmentTree::update(int v, int tl, int tr, int idx, int val)
{
    if (tl == tr)
    {
        tree[v] = val;
    }
    else
    {
        int tm = (tl + tr) / 2;
        if (idx <= tm)
        {
            update(2 * v, tl, tm, idx, val);
        }
        else
        {
            update(2 * v + 1, tm + 1, tr, idx, val);
        }
        tree[v] = tree[2 * v] + tree[2 * v + 1];
    }
}
