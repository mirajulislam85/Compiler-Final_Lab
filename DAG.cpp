//dag
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

enum class NodeType {
    OPERATOR,
    OPERAND
};

struct DAGNode {
    NodeType type;
    string val;
    vector<shared_ptr<DAGNode>> ch;
    
    DAGNode(NodeType t, string v) : type(t), val(v) {}
};

class DAG {
private:
    unordered_map<string, shared_ptr<DAGNode>> nMap;
    shared_ptr<DAGNode> rt;
    
    shared_ptr<DAGNode> getOrCreateNode(NodeType tp, string v, 
                                       vector<shared_ptr<DAGNode>> ch = {}) 
    {
        string k = v;
        for (auto& c : ch) {
            k += "_" + c->val;
        }
        
        if (nMap.find(k) != nMap.end()) 
        {
            return nMap[k];
        }
        
        auto nd = make_shared<DAGNode>(tp, v);
        nd->ch = ch;
        nMap[k] = nd;
        return nd;
    }
    
public:
    shared_ptr<DAGNode> buildFromPostfix(const vector<string>& pf) 
    {
        vector<shared_ptr<DAGNode>> stk;
        
        for (const auto& tok : pf) 
        {
            if (tok == "+" || tok == "-" || tok == "*" || tok == "/") 
            {
                auto r = stk.back(); 
                stk.pop_back();
                auto l = stk.back(); 
                stk.pop_back();
                
                auto nd = getOrCreateNode(NodeType::OPERATOR, tok, {l, r});
                stk.push_back(nd);
            } 
            else 
            {
                auto nd = getOrCreateNode(NodeType::OPERAND, tok);
                stk.push_back(nd);
            }
        }
        
        rt = stk.back();
        return rt;
    }
    
    void print(shared_ptr<DAGNode> nd, int d = 0) 
    {
        if (!nd) return;
        
        for (int i = 0; i < d; i++) 
            cout << "  ";
        cout << nd->val << endl;
        
        for (auto& c : nd->ch) {
            print(c, d + 1);
        }
    }
};

int main() 
{
    DAG dag;
    
    vector<string> pf = {"a", "b", "c", "*", "+", "d", "e", "/", "-"};
    
    auto rt = dag.buildFromPostfix(pf);
    
    cout << "DAG Structure:" << endl;
    dag.print(rt);
    
    return 0;
}

