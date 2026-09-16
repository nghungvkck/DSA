#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Solution {
public:
    vector<vector<int>> result;
    vector<vector<int>> permute(vector<int>& nums) {
        int length = nums.size();
        vector<bool> used(length, false);
        vector<int> arr;
        fund(arr, length, nums, used);
        return result;
    }

    void fund(vector<int> arr, int length, vector<int>& nums, vector<bool>& used) {
        if (arr.size() == length) {
            result.push_back(arr);
            return;
        }

        for (int i = 0; i < length; i++) {
            if (used[i] == true) {
                continue;
            }
            used[i] = true;
            arr.push_back(nums[i]);
            fund(arr, length, nums, used);
            arr.pop_back();
            used[i] = false;
        }
    }
};

int main(){
    Solution sol;
    vector<int> test1 = {1,2,3};
    vector<int> test2 = {2,3,5};
    vector<vector<int>> result = sol.permute(test1);
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            cout << result[i][j] << " ";
        }
        cout << endl;
    }
}