#include <iostream>
#include <vector>
using namespace std;
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int count  = 0;
        for( int i =0; i < prices.size() -1 ; i++){
            if ( prices[i] <= prices[i+1]){
                count += prices[i+1] - prices[i];
            }
        }
        return count;
    }
};

int main() {
    // Phần 1: Đọc và chuyển dữ liệu sang vector
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]); 
    vector<int> v(arr, arr + n); 

    for (int x : v) {
        cout << x << endl; 
    }
    
    return 0;
}