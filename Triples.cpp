//Triples
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Triple {
    string o;
    string a1;
    string a2;
    int idx;
    
    Triple(int i, string op, string arg1, string arg2 = "") 
        : idx(i), o(op), a1(arg1), a2(arg2) {}
    
    void show() {
        cout<<"("<<idx<<") "<<o<<" "<<a1;
        if(!a2.empty()){
            cout<<" "<<a2;
        }
        cout<<endl;
    }
};

class TripleGen {
private:
    vector<Triple> t;
    int nxt;
    
public:
    TripleGen() : nxt(0) {}
    
    int add(string op,string a1,string a2="") {
        t.push_back(Triple(nxt,op,a1,a2));
        return nxt++;
    }
    
    void display() {
        cout<<"Three Address Code (Triples):"<<endl;
        cout<<"--------------------------------"<<endl;
        for(int i=0;i<t.size();i++) {
            t[i].show();
        }
    }
    
    string getRef(int i) {
        return "("+to_string(i)+")";
    }
};

int main() 
{
    TripleGen gen;
    
    // Example: a = b + c * d
    int t1=gen.add("*","c","d");
    int t2=gen.add("+","b",gen.getRef(t1));
    gen.add("=","a",gen.getRef(t2));
    
    gen.display();
    
    return 0;
}