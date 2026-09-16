#include <iostream>
#include <algorithm>
#include <string>
#include <stack>
using namespace std;


class Solution {
public:
    stack<char> st;
    bool isValid(string s) {
        for ( int i = 0; i< s.length(); i++){
            if( st.empty()){
                if ( s[i] == ']' || s[i] == '}' || s[i] == ')') return false;
                else st.push(s[i]);
            } else {
                char ch = st.top();
                if ( s[i] == ']' && ch != '['){
                    return false;
                } else if ( s[i] == ']' && ch == '['){
                    st.pop();
                } else if (s[i] == '['){
                    st.push(s[i]);
                }

                if ( s[i] == ')' && ch != '('){
                    return false;
                } else if ( s[i] == ')' && ch == '('){
                    st.pop();
                } else if (s[i] == '('){
                    st.push(s[i]);
                }


                if ( s[i] == '}' && ch != '{'){
                    return false;
                } else if ( s[i] == '}' && ch == '{'){
                    st.pop();
                } else if (s[i] == '{'){
                    st.push(s[i]);
                }  
            }
        } return st.empty();
    }
};