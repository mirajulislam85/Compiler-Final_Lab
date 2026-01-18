//first and follow
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
using namespace std;

map<char, vector<string>> prod;
map<char, set<char>> fst, flw;
set<char> NT, T;
char start_sym;

bool isNT(char c) {
    return isupper(c);
}

void calc_first(char sym) {
    if(fst[sym].size()>0) return;
    
    for(const string& p : prod[sym]) {
        for(char c : p) {
            if(!isNT(c)) {
                fst[sym].insert(c);
                break;
            }else {
                calc_first(c);
                bool eps=false;
                for(char x : fst[c]) {
                    if(x=='#') eps=true;
                    else fst[sym].insert(x);
                }
                if(!eps) break;
            }
        }
    }
}

void calc_follow(char sym) {
    if(sym==start_sym) {
        flw[sym].insert('$');
    }
    
    for(auto& [n, ps] : prod) {
        for(const string& p : ps) {
            for(int i=0;i<p.length();i++) {
                if(p[i]==sym) {
                    for(int j=i+1;j<p.length();j++) {
                        if(!isNT(p[j])) {
                            flw[sym].insert(p[j]);
                            break;
                        }else{
                            bool eps=false;
                            for(char x : fst[p[j]]) {
                                if(x=='#') eps=true;
                                else flw[sym].insert(x);
                            }
                            if(!eps) break;
                            if(j==p.length()-1) {
                                for(char x : flw[n]) {
                                    flw[sym].insert(x);
                                }
                            }
                        }
                    }
                    if(i==p.length()-1 || 
                        (i<p.length()-1 && fst[p[i+1]].count('#'))) {
                        for(char x : flw[n]) {
                            flw[sym].insert(x);
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int num;
    cout<<"Enter number of productions: ";
    cin>>num;
    cin.ignore();
    
    cout<<"Enter productions (format: A->BC|d, use # for epsilon):\n";
    for(int i=0;i<num;i++) {
        string ln;
        getline(cin,ln);
        char lhs=ln[0];
        NT.insert(lhs);
        if(i==0) start_sym=lhs;
        
        string rhs=ln.substr(3);
        string temp="";
        for(char ch : rhs) {
            if(ch=='|') {
                prod[lhs].push_back(temp);
                temp="";
            }else {
                temp+=ch;
                if(!isNT(ch) && ch!='#') T.insert(ch);
            }
        }
        prod[lhs].push_back(temp);
    }
    
    for(char nt : NT) {
        calc_first(nt);
    }
    
    for(int i=0;i<2;i++) {
        for(char nt : NT) {
            calc_follow(nt);
        }
    }
    
    cout<<"\nFIRST Sets:\n";
    for(char nt : NT) {
        cout<<"FIRST("<<nt<<") = { ";
        for(char f : fst[nt]) {
            cout<<f<<" ";
        }
        cout<<"}\n";
    }
    
    cout<<"\nFOLLOW Sets:\n";
    for(char nt : NT) {
        cout<<"FOLLOW("<<nt<<") = { ";
        for(char f : flw[nt]) {
            cout<<f<<" ";
        }
        cout<<"}\n";
    }
    
    return 0;
}