#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
using namespace std;

class Solution {
public:
    vector<vector<int>> result;
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        for ( int i = 0; i < candidates.size(); i++ ){
            vector<int> arr;
            if (candidates[i] == target){
                arr.push_back(candidates[i]);
                result.push_back(arr);
                continue;
            } else {
                arr.push_back(candidates[i]);
                fund(candidates, candidates[i] , arr, target);
            }
        }

        set<vector<int>> s;
        for (auto& v : result) {
            sort(v.begin(), v.end());   
            s.insert(v);
        }
        result.assign(s.begin(), s.end());

        return result;
    }

    void fund(const vector<int>& candidates, int sum, vector<int> arr, int target){
        for ( int i = 0 ; i< candidates.size(); i++){
            int newSum = sum + candidates[i];
            if ( newSum > target){
                continue;
            }

            if ( newSum == target){
                arr.push_back(candidates[i]);
                result.push_back(arr);
                arr.pop_back();
            }

            if ( newSum < target){
                arr.push_back(candidates[i]);
                fund(candidates, newSum , arr, target);
                arr.pop_back();
            }
        }
    }
}; 


int main(){
    Solution sol;
    vector<int> test1 = {2,3,6,7};
    vector<int> test2 = {2,3,5};
    vector<vector<int>> result = sol.combinationSum(test2, 8);
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            cout << result[i][j] << " ";
        }
        cout << endl;
    }
}