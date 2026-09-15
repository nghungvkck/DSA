#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
using namespace std;
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {

        // Bước 1 tìm từ có length ngắn nhất
        string result = "";
        int length = INT_MAX;
        for (string x : strs){
            if (x.length() <= length) length = x.length();
        }

        // Bước 2
        for ( int i = 0; i < length; i++){
            for (int j = 0; j < strs.size()-1; j++){
                if ( strs[j][i] != strs[j+1][i]){
                    return result;
                }
            }
            result += strs[0][i];
        }
        return result;
    }
};

int main(){
    vector<string> strs = {"flower", "flow", "flight"};
    Solution sol;
    cout << sol.longestCommonPrefix(strs);
}
