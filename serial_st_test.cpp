#include <iostream>
#include <vector>
#include "serial_st.hpp"

using namespace std;

// Simple test of SegmentTree data structure
int main()
{
    vector<int> arr = {1, 3, 5, 7, 9, 11};
    SegmentTree tree(arr);
    cout << tree.query(1, 3) << endl; // outputs 15
    tree.update(2, 6);
    cout << tree.query(1, 3) << endl; // outputs 16
    return 0;
}