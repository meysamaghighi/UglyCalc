/*

Author:
Meysam Aghighi
2018-01-13

Description:
Simple calculator with lazy evaluation.

Compile: (Windows)
g++ -std=gnu++11 calc.cpp -o calc.exe

Run: (Windows)
calc.exe
calc.exe < [filename]
calc.exe [filename]

*/

#include <bits/stdc++.h>
using namespace std;

struct operation { // data structure to keep each binary operation
    string register1;
    string register2;
    string operand;
    operation() {}
    operation(const vector<string>& op)
        : register1(op[0])
        , operand(op[1])
        , register2(op[2])
    {
    }
};

inline bool isalnum_string(const string& s)
{ // checks whether a string contains only alphanumeric characters or not.
    for (auto& c : s)
        if (!isalnum(c))
            return false;
    return true;
}

inline bool isint(const string& s)
{ // checks whether a string is an integer or not.
    for (auto& c : s)
        if (!isdigit(c))
            return false;
    return true;
}

vector<string> tokenize(string s)
{ // tokenize a string into a vector of strings: "A add 2" --> {"A", "add", "2"}
    stringstream temp_stringstream(s);
    vector<string> input_vector;
    string temp_string;
    while (temp_stringstream >> temp_string) {
        input_vector.push_back(temp_string);
    }
    return input_vector;
}

int evaluate(const string& reg, const vector<operation>& data, map<string, int>& map_values, set<string>& dependent_registers)
{ // the main function that does the lazy evaluation recursively
    if (isint(reg))
        return stoi(reg); // throws exception for out_of_range inputs!
    if (map_values.count(reg))
        return map_values[reg];

    if (dependent_registers.count(reg)) {
        cerr << "There is a cycle in the dependency graph: " << reg << " depends on itself!" << endl;
        exit(1);
    }
    dependent_registers.insert(reg);

    int result = 0;
    for (int i = 0; i < data.size(); i++) {
        if (data[i].register1 != reg)
            continue;
        if (data[i].operand == "add")
            result += evaluate(data[i].register2, data, map_values, dependent_registers);
        if (data[i].operand == "subtract")
            result -= evaluate(data[i].register2, data, map_values, dependent_registers);
        if (data[i].operand == "multiply")
            result *= evaluate(data[i].register2, data, map_values, dependent_registers);
    }

    return map_values[reg] = result;
}

bool valid_command(const vector<string>& input_vector)
{ // takes tokenized command and checks its validity and prints errors if needed
    if (input_vector.size() == 1) { // one argument command should only be quit
        if (input_vector[0] == "quit" || input_vector[0] == "QUIT")
            return true;
        cerr << "Invalid command!" << endl;
        return false;
    }
    if (input_vector.size() == 2) { // two argument command should only be print
        if (input_vector[0] != "print") {
            cerr << "Invalid command!" << endl;
            return false;
        }
        if (!isalnum_string(input_vector[1])) {
            cerr << "Invalid variable name!" << endl;
            return false;
        }
        return true;
    }
    if (input_vector.size() == 3) { // three argument command should only be a binary operation
        string reg1 = input_vector[0], oper = input_vector[1], reg2 = input_vector[2];
        if (!isalnum_string(reg1) || !isalnum_string(reg2)) {
            cerr << "Invalid variable name!" << endl;
            return false;
        }
        if (oper != "add" && oper != "subtract" && oper != "multiply") {
            cerr << "Invalid command!" << endl;
            return false;
        }
        return true;
    }
    cerr << "Invalid command!" << endl;
    return false;
}

int main(int argc, char* argv[])
{

    bool is_input_file = false;
    istream* input_stream;
    ifstream input_file;

    if (argc == 1) {
        input_stream = &cin;
    }
    else if (argc == 2) {
        input_file.open(argv[1]);
        if (!input_file) {
            cerr << "Could not read from the input file!" << endl;
            return 1;
        }
        input_stream = &input_file;
        is_input_file = true;
    }
    else {
        cerr << "Too many input arguments!" << endl;
        return 1;
    }

    string input_line;
    vector<operation> data; // stores all the binary operations (e.g. a add b)
    set<string> registers; // stores all register names
    map<string, int> map_values; // stores the values of some registers (the ones whose values are needed at print)
    set<string> dependent_registers; // stores dependent registers (to the register being printed)

    while (getline(*input_stream, input_line)) { // read input line by line (from file or stdin)
        vector<string> input_vector = tokenize(input_line);

        if (!valid_command(input_vector))
            continue; // check command's validity and print error messages accordingly

        if (input_vector.size() == 1) { // a valid quit command
            return 0;
        }
        if (input_vector.size() == 2) { // a valid print command
            string reg = input_vector[1];

            if (!registers.count(reg)) { // here we have a choice: 1. Give an error, 2. Print 0 (create a new register). I choose option 1.
                cerr << "This register name does not exist!" << endl;
            }
            else {
                map_values.clear();
                dependent_registers.clear();
                cout << evaluate(reg, data, map_values, dependent_registers) << endl;
            }
        }
        if (input_vector.size() == 3) { // a valid add, subtract or multiply operation
            string reg1 = input_vector[0], reg2 = input_vector[2];
            registers.insert(reg1);
            registers.insert(reg2);

            data.push_back(operation(input_vector));
        }
    }

    if (is_input_file)
        input_file.close(); // closing the input file, if reading from a file.

    return 0;
}