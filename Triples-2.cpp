#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Triple structure
struct Triple {
    string o, a1, a2;
    int idx;

    Triple(int i, string op, string arg1, string arg2 = "")
        : idx(i), o(op), a1(arg1), a2(arg2) {}

    void show() {
        cout << "(" << idx << ") " << o << " " << a1;
        if (!a2.empty()) cout << " " << a2;
        cout << endl;
    }
};

class TripleGen {
private:
    vector<Triple> t;
    int nxt;

public:
    TripleGen() : nxt(0) {}

    int add(string op, string a1, string a2 = "") {
        t.push_back(Triple(nxt, op, a1, a2));
        return nxt++;
    }

    void display() {
        cout << "\nThree Address Code (Triples):\n";
        for (int i = 0; i < t.size(); i++) {
            t[i].show();
        }
    }

    string getRef(int i) {
        return "(" + to_string(i) + ")";
    }
};

int main() {
    TripleGen gen;

    int n;
    cout << "Enter number of operations: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        string op, a1, a2;
        cout << "\nOperation " << i + 1 << endl;
        cout << "Operator: ";
        cin >> op;
        cout << "Argument 1: ";
        cin >> a1;
        cout << "Argument 2 (or 0 if none): ";
        cin >> a2;

        if (a2 == "0")
            gen.add(op, a1);
        else
            gen.add(op, a1, a2);
    }

    gen.display();

    return 0;
}
