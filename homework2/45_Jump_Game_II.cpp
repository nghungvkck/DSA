#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;


class Solution {
public:
    int jump(vector<int>& nums) {
        int length = nums.size();
        vector<int> dp (length + 5, 0);
        
        // for (auto& i : dp) cout << i << " ";
        // cout << endl;
        
        for ( int i = 0; i < length; i++){
            for ( int j = i +1; j <= i + nums[i]; j++){
                if ( j >= length){
                    break;
                } else {
                    if ( dp[j] == 0 ){
                        dp[j] = dp[i]+1;
                    } else {
                        dp[j] = min(dp[i]+1, dp[j]);
                    }
                }
            }
        }
        return dp[length -1];
    }
};

int main(){
    Solution sol;
    vector<int> test = {2,3,1,1,4};
    cout << sol.jump(test);
}
