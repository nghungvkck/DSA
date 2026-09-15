#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;


class Solution {
public:
    vector<string> phone = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
    vector<string> result;
    string path;

    void func(const string& digital, int index){
        // dk dừng
        if ( index == digital.size()){
            result.push_back(path);
            return;
        }

        // ghép từ
        for ( char ch : phone[digital[index] - '0']){
            path += ch;
            func(digital, index +1);
            path.pop_back();
        }
    }


    vector<string> letterCombinations(string digits) {
        func(digits, 0);
        return result;
    }
};

int main(){
    Solution sol;
    string digits = "23";
    for ( string x : sol.letterCombinations(digits)){
        cout << x << endl;
    }
}
