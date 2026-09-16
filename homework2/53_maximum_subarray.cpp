#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int cur  = nums[0];
        int result = nums[0];

        for ( int i = 1; i< nums.size(); i++){
            cur = max(nums[i], cur+ nums[i]);
            result = max(result, cur);
        }

        return result;
    }
};