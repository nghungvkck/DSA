#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
using namespace std;

class Solution {
public:
    vector<vector<int>> result;
    set<vector<int>> result2;
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        int length = nums.size();
        vector<int> arr;
        vector<bool> used(length, false);
        fund(arr, length, nums, used);
        
        for ( auto x : result2){
            result.push_back(x);
        }

        return result;
    }

    
    void fund(vector<int> arr, int length, vector<int>& nums, vector<bool>& used){
        if ( arr.size() == length ){
            result2.insert(arr);
            return;
        }

        for ( int i =0 ; i< length; i++){
            if ( used[i] == true) continue;

            used[i] =true;
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
    vector<int> test2 = {1,1,2};
    vector<vector<int>> result = sol.permuteUnique(test2);
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            cout << result[i][j] << " ";
        }
        cout << endl;
    }
}