//LL(1)

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <string>
using namespace std;

struct Grammar {
    map<char, vector<string>> prod;
    set<char> nt;
    set<char> t;
    char start;
};

bool isNT(char c) {
    return isupper(c);
}

set<char> getFirst(char sym, Grammar& g, map<char, set<char>>& fst) {
    if (fst.find(sym) != fst.end()) {
        return fst[sym];
    }
    
    set<char> res;
    
    if (!isNT(sym)) {
        res.insert(sym);
        return res;
    }
    
    for(int i=0;i<g.prod[sym].size();i++) {
        string p = g.prod[sym][i];
        bool eps = true;
        for(int j=0;j<p.length();j++) {
            char ch = p[j];
            set<char> temp = getFirst(ch, g, fst);
            for (char x : temp) {
                if(x != 'e') res.insert(x);
            }
            if (temp.find('e') == temp.end()) {
                eps = false;
                break;
            }
        }
        if(eps) res.insert('e');
    }
    
    fst[sym] = res;
    return res;
}

void getFollow(Grammar& g, map<char, set<char>>& fst, map<char, set<char>>& flw) {
    flw[g.start].insert('$');
    
    bool chg = true;
    while(chg) {
        chg = false;
        for (auto& p : g.prod) {
            char A = p.first;
            for(int k=0;k<p.second.size();k++) {
                string rhs = p.second[k];
                for(int i=0; i<rhs.length(); i++) {
                    if(isNT(rhs[i])) {
                        bool allEps = true;
                        for(int j=i+1; j<rhs.length(); j++) {
                            set<char> f = getFirst(rhs[j], g, fst);
                            for(char x : f) {
                                if(x != 'e' && flw[rhs[i]].find(x) == flw[rhs[i]].end()) {
                                    flw[rhs[i]].insert(x);
                                    chg = true;
                                }
                            }
                            if(f.find('e') == f.end()) {
                                allEps = false;
                                break;
                            }
                        }
                        if(allEps) {
                            for(char x : flw[A]) {
                                if(flw[rhs[i]].find(x) == flw[rhs[i]].end()) {
                                    flw[rhs[i]].insert(x);
                                    chg = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void makeTable(Grammar& g, map<char, set<char>>& fst, map<char, set<char>>& flw) {
    map<pair<char, char>, string> tbl;
    
    for (auto& p : g.prod) {
        char A = p.first;
        for(int k=0;k<p.second.size();k++) {
            string rhs = p.second[k];
            set<char> f;
            bool hasEps = true;
            for(int i=0;i<rhs.length();i++) {
                char c = rhs[i];
                set<char> temp = getFirst(c, g, fst);
                for(char x : temp) {
                    if(x != 'e') f.insert(x);
                }
                if(temp.find('e') == temp.end()) {
                    hasEps = false;
                    break;
                }
            }
            if(hasEps) f.insert('e');
            
            for(char x : f) {
                if(x != 'e') {
                    tbl[{A, x}] = rhs;
                }
            }
            if(f.find('e') != f.end()) {
                for(char x : flw[A]) {
                    tbl[{A, x}] = rhs;
                }
            }
        }
    }
    
    cout << "\nLL(1) Parsing Table:\n";
    for(auto& e : tbl) {
        cout << "M[" << e.first.first << ", " << e.first.second << "] = " 
             << e.first.first << " -> " << e.second << endl;
    }
}

int main() {
    Grammar g;
    int n;
    
    cout << "Enter number of productions: ";
    cin >> n;
    
    cout << "Enter productions (e.g., E=E+T or E=e for epsilon):\n";
    for(int i=0; i<n; i++) {
        string s;
        cin >> s;
        char lhs = s[0];
        string rhs = s.substr(2);
        
        g.nt.insert(lhs);
        g.prod[lhs].push_back(rhs);
        
        for(int j=0;j<rhs.length();j++) {
            char c = rhs[j];
            if(!isNT(c) && c != 'e') {
                g.t.insert(c);
            }
        }
    }
    
    g.start = g.prod.begin()->first;
    
    map<char, set<char>> fst, flw;
    
    for(char x : g.nt) {
        getFirst(x, g, fst);
    }
    
    cout << "\nFIRST Sets:\n";
    for(auto& f : fst) {
        cout << "FIRST(" << f.first << ") = { ";
        for(char c : f.second) {
            cout << c << " ";
        }
        cout << "}\n";
    }
    
    getFollow(g, fst, flw);
    
    cout << "\nFOLLOW Sets:\n";
    for(auto& f : flw) {
        cout << "FOLLOW(" << f.first << ") = { ";
        for(char c : f.second) {
            cout << c << " ";
        }
        cout << "}\n";
    }
    
    makeTable(g, fst, flw);
    
    return 0;
}