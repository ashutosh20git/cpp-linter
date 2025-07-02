#include <iostream>
#include <vector>
using namespace std;

int* create() {
    int* p;
    return p;
}

void longFunction() {
    // simulate long function
    for (int i = 0; i < 55; i++) {
        cout << i << endl;
    }
}

int main() {
    vector<int> v(10);
    cout << v[5] << endl;  // unsafe access
    goto skip;             // bad practice
    int x;
skip:
}
