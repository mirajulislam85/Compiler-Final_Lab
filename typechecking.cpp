//typecheck

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <map>

using namespace std;

map<string, string> symbolTable;

bool isNumber(const string& str) {
    for (char c : str) {
        if (!isdigit(c) && c != '.') return false;
    }
    return !str.empty();
}

bool typeCheck(const string& var, const string& value) {
    if (symbolTable.find(var) == symbolTable.end()) {
        // Variable not declared
        return false;
    }
    
    string varType = symbolTable[var];
    
    if (varType == "int") {
        return isNumber(value) && value.find('.') == string::npos;
    } else if (varType == "float" || varType == "double") {
        return isNumber(value);
    } else if (varType == "string") {
        return true;
    }
    
    return false;
}

int main() {
    string input;
    
    cout << "Simple Type Checker (Commands: declare <type> <var>, assign <var> <value>, exit)\n";
    
    while (true) {
        cout << "> ";
        getline(cin, input);
        
        if (input == "exit") break;
        
        istringstream iss(input);
        string command;
        iss >> command;
        
        if (command == "declare") {
            string type, var;
            iss >> type >> var;
            symbolTable[var] = type;
            cout << "Declared " << var << " as " << type << endl;
        } else if (command == "assign") {
            string var, value;
            iss >> var >> value;
            
            if (typeCheck(var, value)) {
                cout << "Type check passed for " << var << " = " << value << endl;
            } else {
                cout << "Type check failed for " << var << " = " << value << endl;
            }
        } else {
            cout << "Unknown command" << endl;
        }
    }
    
    return 0;
}