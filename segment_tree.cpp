#include <vector>
#include <iostream>

using namespace std;

class SegmentTree
{
public:
    SegmentTree(const vector<int> &arr)
    {
        n = arr.size();
        tree.resize(4 * n);
        build(arr, 1, 0, n - 1);
    }

    int query(int l, int r)
    {
        return query(1, 0, n - 1, l, r);
    }

    void update(int index, int value)
    {
        update(1, 0, n - 1, index, value);
    }

private:
    int n;
    vector<int> tree;

    void build(const vector<int> &arr, int node, int start, int end)
    {
        if (start == end)
        {
            tree[node] = arr[start];
        }
        else
        {
            int mid = (start + end) / 2;
            build(arr, 2 * node, start, mid);
            build(arr, 2 * node + 1, mid + 1, end);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

    int query(int node, int start, int end, int l, int r)
    {
        if (r < start || end < l)
        {
            return 0;
        }
        if (l <= start && end <= r)
        {
            return tree[node];
        }
        int mid = (start + end) / 2;
        int left_sum = query(2 * node, start, mid, l, r);
        int right_sum = query(2 * node + 1, mid + 1, end, l, r);
        return left_sum + right_sum;
    }

    void update(int node, int start, int end, int index, int value)
    {
        if (start == end)
        {
            tree[node] = value;
        }
        else
        {
            int mid = (start + end) / 2;
            if (start <= index && index <= mid)
            {
                update(2 * node, start, mid, index, value);
            }
            else
            {
                update(2 * node + 1, mid + 1, end, index, value);
            }
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }
};

int main()
{
    vector<int> arr = {1, 3, 5, 7, 9, 11};
    SegmentTree tree(arr);
    cout << tree.query(1, 3) << endl; // outputs 15
    tree.update(2, 6);
    cout << tree.query(1, 3) << endl; // outputs 16
    return 0;
}