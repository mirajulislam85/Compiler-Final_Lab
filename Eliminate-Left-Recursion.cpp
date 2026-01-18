//eliminate left recursion 
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

class Grammar {
private:
    map<string, vector<vector<string>>> prod;
    set<string> nt;
    
public:
    void add(string l, vector<string> r) {
        prod[l].push_back(r);
        nt.insert(l);
    }
    
    void removeLeftRec() {
        vector<string> ntList(nt.begin(), nt.end());
        
        for(size_t i=0;i<ntList.size();i++) {
            string A = ntList[i];
            
            // immediate left recursion elimination
            vector<vector<string>> alph; 
            vector<vector<string>> bet;  
            
            for(auto& p : prod[A]) {
                if(!p.empty() && p[0]==A) {
                    vector<string> t(p.begin()+1, p.end());
                    alph.push_back(t);
                } 
                else {
                    bet.push_back(p);
                }
            }
            
            if(!alph.empty()) {
                string Anew = A+"'";
                nt.insert(Anew);
                
                prod[A].clear();
                for(auto& b : bet) {
                    vector<string> np=b;
                    np.push_back(Anew);
                    prod[A].push_back(np);
                }
                
                prod[Anew].clear();
                for(auto& a : alph) {
                    vector<string> np=a;
                    np.push_back(Anew);
                    prod[Anew].push_back(np);
                }
                prod[Anew].push_back({{"epsilon"}});
            }
        }
    }
    
    void show() {
        cout<<"Grammar after eliminating left recursion:\n";
        for(auto& e : prod) {
            cout<<e.first<<" -> ";
            for(size_t i=0;i<e.second.size();i++) {
                for(auto& s : e.second[i]) {
                    cout<<s<<" ";
                }
                if(i+1<e.second.size()) cout<<"| ";
            }
            cout<<endl;
        }
    }
};

int main() {
    Grammar gr;
    
    string lhs,arr;
    vector<string> sym;
    
    cout<<"Enter grammar productions (format: LHS -> symbol1 symbol2 ... | symbol3 ...)\n";
    cout<<"Press Ctrl+Z (Windows) or Ctrl+D (Unix/Linux) and Enter for EOF:\n\n";
    
    while(cin>>lhs) {
        cin>>arr; 
        
        string tok;
        vector<string> curr;
        
        while(cin>>tok && tok!="\n") {
            if(tok=="|") {
                if(!curr.empty()) {
                    gr.add(lhs,curr);
                    curr.clear();
                }
            } 
            else {
                curr.push_back(tok);
            }
            
            if(cin.peek()=='\n') {
                break;
            }
        }
        
        if(!curr.empty()) {
            gr.add(lhs,curr);
        }
    }
    
    cout<<"\n";
    gr.removeLeftRec();
    gr.show();
    
    return 0;
}