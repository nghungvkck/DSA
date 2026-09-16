#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Solution {
public:
    int trap(vector<int>& height) {
        int left  = 0;
        int right = height.size() -1 ;

        int count =0;
        int maxLeft = 0;
        int maxRight = 0;
        while ( left < right){
            if ( height[left] <= height[right]){
                if ( height[left] >= maxLeft){
                    maxLeft = height[left];
                } else {
                    count += maxLeft - height[left];
                }
                left++;
            } else {
                if ( height[right] >= maxRight){
                    maxRight = height[right];
                } else {
                    count += maxRight - height[right];
                }

                right--;
            }
        }
        return count;
    }
};

int main(){
    Solution sol;
    vector<int> test1 = {0,1,0,2,1,0,1,3,2,1,2,1};
    vector<int> test2 = {4,2,0,3,2,5};
    cout << sol.trap(test2) << endl;
}