#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n = nums.size();
        vector<bool> dp(n, false);

        dp[0] = true;
        for ( int i  = 1; i < n; i++){
            for ( int j = 0;j < n; j++){
                if ( dp[j] == true && j + nums[j] >= i){
                    dp[i] = true;
                    break;
                }
            }
        }
        return dp[n-1];
    }
};