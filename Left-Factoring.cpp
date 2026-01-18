//left fectorization
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

class LeftFactoring {
private:
    map<string, vector<string>> g;
    int cnt = 0;

public:
    void inputGrammar() {
        int n;
        cout << "Enter number of productions: ";
        cin >> n;
        cin.ignore();
        
        cout << "Enter productions in format 'A->alpha|beta' (one per line):\n";
        for (int i = 0; i < n; i++) {
            string l;
            getline(cin, l);
            parseProduction(l);
        }
    }

    void parseProduction(string l) {
        size_t ap = l.find("->");
        if (ap == string::npos) return;
        
        string nt = l.substr(0, ap);
        string p = l.substr(ap + 2);
        
        size_t pos = 0;
        string t;
        while ((pos = p.find('|')) != string::npos) {
            t = p.substr(0, pos);
            g[nt].push_back(t);
            p.erase(0, pos + 1);
        }
        g[nt].push_back(p);
    }

    string findCommonPrefix(vector<string>& prods) {
        if (prods.size() < 2) return "";
        
        string pre = "";
        string f = prods[0];
        
        for (size_t i = 0; i < f.length(); i++) {
            char c = f[i];
            bool match = true;
            
            for (size_t j = 1; j < prods.size(); j++) {
                if (i >= prods[j].length() || prods[j][i] != c) {
                    match = false;
                    break;
                }
            }
            
            if (match) {
                pre += c;
            } 
            else {
                break;
            }
        }
        
        return pre;
    }

    void performLeftFactoring() {
        bool chg = true;
        
        while (chg) {
            chg = false;
            map<string, vector<string>> ng = g;
            
            for (auto& [nt, prods] : g) {
                string pre = findCommonPrefix(prods);
                
                if (!pre.empty()) {
                    chg = true;
                    string nnt = nt + "'";
                    if (cnt > 0) {
                        nnt = nt + string(cnt + 1, '\'');
                    }
                    cnt++;
                    
                    vector<string> np;
                    vector<string> rp;
                    
                    for (const string& pr : prods) {
                        if (pr.substr(0, pre.length()) == pre) {
                            string rem = pr.substr(pre.length());
                            if (rem.empty()) rem = "ε";
                            np.push_back(rem);
                        } 
                        else {
                            rp.push_back(pr);
                        }
                    }
                    
                    rp.push_back(pre + nnt);
                    ng[nt] = rp;
                    ng[nnt] = np;
                    
                    break;
                }
            }
            
            g = ng;
        }
    }

    void displayGrammar() {
        cout << "\nGrammar after left factoring:\n";
        for (const auto& [nt, prods] : g) {
            cout << nt << " -> ";
            for (size_t i = 0; i < prods.size(); i++) {
                cout << prods[i];
                if (i < prods.size() - 1) cout << " | ";
            }
            cout << endl;
        }
    }
};

int main() {
    LeftFactoring lf;
    
    cout << "=== Left Factoring of CFG ===\n";
    lf.inputGrammar();
    lf.performLeftFactoring();
    lf.displayGrammar();
    
    return 0;
}