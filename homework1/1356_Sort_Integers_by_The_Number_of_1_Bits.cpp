#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
class Solution {
public:
    //  this là để cho phép truy câp hàm hoặc biến ở trong class
    vector<int> sortByBits(vector<int>& arr) {
        sort(arr.begin(), arr.end(), [this](int a, int b){
            int ca = countBit1(a);
            int cb = countBit1(b);
            if ( ca != cb) return ca < cb;
            return a < b;
        });
        return arr;
    }

    int countBit1(int a){
        int count = 0;
        while(a > 0){
            if ( a % 2 == 1) count++;
            a = a/2;
        }
        return count;
    }
};

int main(){
    Solution sol;
    cout << sol.countBit1(8) << endl;
    return 0;
}

