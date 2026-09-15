#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;


class Solution {
public:
    string result = "1";
    string countAndSay(int n) {
        if ( n == 1) return "1";
        int count = 1;
        while( count < n){
            result  = RLE(result);
            count++;
        }
        return result;
    }

    string RLE(string str){
        string test = "";
        for ( int i = 0; i< str.length(); i++){
            char s = str[i];
            int count  = 0;
            for ( int j = i; j < str.length(); j++){
                if ( s != str[j]) {
                    break;
                } 
                count++;
            }
            test = test + to_string(count) + s;
            i += count-1  ;
        }
        return test;
    }
};

int main(){
    Solution sol;
    cout << sol.RLE("1144455551") << endl;  
    cout << sol.countAndSay(4) << endl;
}
