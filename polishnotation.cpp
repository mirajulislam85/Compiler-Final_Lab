//polish notation
#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

int prec(char o) {
    if (o == '+' || o == '-')
        return 1;
    if (o == '*' || o == '/')
        return 2;
    if (o == '^')
        return 3;
    return 0;
}

string infixToPrefix(string s) {
    reverse(s.begin(), s.end());
    
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(')
            s[i] = ')';
        else if (s[i] == ')')
            s[i] = '(';
    }
    
    stack<char> st;
    string res = "";
    
    for (int i = 0; i < s.length(); i++) {
        char ch = s[i];
        
        if (isalnum(ch)) {
            res += ch;
        }
        else if (ch == '(') {
            st.push(ch);
        }
        else if (ch == ')') {
            while (!st.empty() && st.top() != '(') {
                res += st.top();
                st.pop();
            }
            if (!st.empty())
                st.pop();
        }
        else {
            while (!st.empty() && prec(ch) < prec(st.top())) {
                res += st.top();
                st.pop();
            }
            st.push(ch);
        }
    }
    
    while (!st.empty()) {
        res += st.top();
        st.pop();
    }
    
    reverse(res.begin(), res.end());
    return res;
}

int main() 
{
    string inp;
    cout << "Enter an infix expression: ";
    cin >> inp;
    
    string pref = infixToPrefix(inp);
    cout << "Polish Notation (Prefix): " << pref << endl;
    
    return 0;
}