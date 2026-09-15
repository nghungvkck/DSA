#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Solution {
public:
    vector<string> result;
    string path;
    vector<string> generateParenthesis(int n) {
        func("", 0, 0, n);
        return result;
    }

    void func(string path, int l, int r, const int& n){
        if ( (l + r)  == 2* n ){
            result.push_back(path);
            return;
        }

        if ( l < n){
            func(path +'(', l+1, r, n);
        }

        if (  r < l ){
            func(path +')', l, r+1, n);
        }
    }
};