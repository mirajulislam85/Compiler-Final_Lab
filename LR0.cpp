//LR(0)

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

struct Production {
    char nt;
    string prod;
};

struct Item {
    char nt;
    string prod;
    int pos;
    
    bool operator<(const Item& o) const {
        if (nt != o.nt) return nt < o.nt;
        if (prod != o.prod) return prod < o.prod;
        return pos < o.pos;
    }
    
    bool operator==(const Item& o) const {
        return nt == o.nt && 
               prod == o.prod && 
               pos == o.pos;
    }
};

class LR0Parser {
private:
    vector<Production> g;
    char start;
    set<char> nts;
    vector<set<Item>> sts;
    map<pair<int, char>, int> trans;
    
    void addProd(char n, string p) 
    {
        g.push_back({n, p});
        nts.insert(n);
    }
    
    set<Item> getClosure(set<Item> itms) 
    {
        bool chg = true;
        while (chg) {
            chg = false;
            set<Item> newitms = itms;
            for (const auto& it : itms) 
            {
                if (it.pos < it.prod.length()) {
                    char nxt = it.prod[it.pos];
                    if (nts.count(nxt)) {
                        for (const auto& pr : g) 
                        {
                            if (pr.nt == nxt) {
                                Item ni = {pr.nt, pr.prod, 0};
                                if (newitms.find(ni) == newitms.end()) {
                                    newitms.insert(ni);
                                    chg = true;
                                }
                            }
                        }
                    }
                }
            }
            itms = newitms;
        }
        return itms;
    }
    
    set<Item> goTo(const set<Item>& itms, char sym) 
    {
        set<Item> res;
        for (const auto& it : itms) 
        {
            if (it.pos < it.prod.length() && 
                it.prod[it.pos] == sym) {
                res.insert({it.nt, it.prod, it.pos + 1});
            }
        }
        return getClosure(res);
    }
    
    int findSt(const set<Item>& st) 
    {
        for (int i = 0; i < sts.size(); i++) {
            if (sts[i] == st) return i;
        }
        return -1;
    }
    
public:
    void construct() 
    {
        char aug = start + '\'';
        g.insert(g.begin(), {aug, string(1, start)});
        
        set<Item> init = {{aug, string(1, start), 0}};
        sts.push_back(getClosure(init));
        
        for (int i = 0; i < sts.size(); i++) 
        {
            set<char> syms;
            for (const auto& it : sts[i]) {
                if (it.pos < it.prod.length()) {
                    syms.insert(it.prod[it.pos]);
                }
            }
            
            for (char s : syms) 
            {
                set<Item> nst = goTo(sts[i], s);
                if (!nst.empty()) {
                    int idx = findSt(nst);
                    if (idx == -1) {
                        sts.push_back(nst);
                        idx = sts.size() - 1;
                    }
                    trans[{i, s}] = idx;
                }
            }
        }
    }
    
    void display() 
    {
        for (int i = 0; i < sts.size(); i++) 
        {
            cout << "State " << i << ":\n";
            for (const auto& it : sts[i]) 
            {
                cout << "  " << it.nt << " -> ";
                for (int j = 0; j < it.prod.length(); j++) {
                    if (j == it.pos) cout << ".";
                    cout << it.prod[j];
                }
                if (it.pos == it.prod.length()) cout << ".";
                cout << "\n";
            }
            cout << "\n";
        }
    }
    
    void setSym(char s) { start = s; }
    void addRule(char n, string p) { addProd(n, p); }
};

int main() 
{
    LR0Parser parser;
    parser.setSym('E');
    parser.addRule('E', "E+T");
    parser.addRule('E', "T");
    parser.addRule('T', "T*F");
    parser.addRule('T', "F");
    parser.addRule('F', "(E)");
    parser.addRule('F', "id");
    
    parser.construct();
    parser.display();
    
    return 0;
}