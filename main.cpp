#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// ANSI color codes
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";

// Helper
bool contains(const string& line, const string& word) {
    return line.find(word) != string::npos;
}

// Rule functions
void checkRawPointers(const string& line, int lineNumber, int& warnings) {
    if (contains(line, "*") && !contains(line, "new") && !contains(line, "#include") && !contains(line, "char* argv")) {
        cout << YELLOW << "Warning [Line " << lineNumber << "]: Avoid using raw pointers. Use smart pointers instead." << RESET << endl;
        warnings++;
    }
}

void checkNamespaceStd(const string& line, int lineNumber, int& warnings) {
    if (contains(line, "using namespace std")) {
        cout << YELLOW << "Warning [Line " << lineNumber << "]: Avoid 'using namespace std;'. Use std:: prefix explicitly." << RESET << endl;
        warnings++;
    }
}

void checkVectorAccess(const string& line, int lineNumber, int& warnings) {
    if (contains(line, "[") && contains(line, "]") && contains(line, "vector")) {
        cout << YELLOW << "Warning [Line " << lineNumber << "]: Use '.at(i)' instead of '[]' for bounds checking in vectors." << RESET << endl;
        warnings++;
    }
}

void checkIOUsage(const string& line, int lineNumber, int& warnings) {
    if ((contains(line, "cin") || contains(line, "cout")) && !contains(line, "std::") && !contains(line, "using namespace std")) {
        cout << YELLOW << "Warning [Line " << lineNumber << "]: Use 'std::cin' and 'std::cout' or declare 'using namespace std;' (not recommended)." << RESET << endl;
        warnings++;
    }
}

void checkConstMethods(const string& line, int lineNumber, int& warnings) {
    if ((contains(line, "get") || contains(line, "Get")) && contains(line, "()") && !contains(line, "const") && contains(line, "{")) {
        cout << YELLOW << "Warning [Line " << lineNumber << "]: Consider marking non-modifying methods as 'const'." << RESET << endl;
        warnings++;
    }
}

void checkUninitVars(const string& line, int lineNumber, int& warnings) {
    if (contains(line, "int ") && line.find('=') == string::npos && line.find(';') != string::npos && !contains(line, "for")) {
        cout << YELLOW << "Warning [Line " << lineNumber << "]: Variable declared but not initialized." << RESET << endl;
        warnings++;
    }
}

void checkUnusedIncludes(const string& line, int lineNumber) {
    if (contains(line, "#include") && (contains(line, "<bits") || contains(line, "<vector>") || contains(line, "<map>"))) {
        cout << CYAN << "Note [Line " << lineNumber << "]: Check if this header is actually used in the code." << RESET << endl;
    }
}

void checkGoto(const string& line, int lineNumber, int& warnings) {
    if (contains(line, "goto ")) {
        cout << YELLOW << "Warning [Line " << lineNumber << "]: Avoid using 'goto'. It makes code hard to read and maintain." << RESET << endl;
        warnings++;
    }
}

void checkMissingReturnMain(const string& line, int lineNumber) {
    if (contains(line, "int main") && !contains(line, "return")) {
        cout << CYAN << "Note [Line " << lineNumber << "]: Consider ending 'main' with 'return 0;' for clarity." << RESET << endl;
    }
}

void runLinter(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    int lineNumber = 0;
    int warnings = 0;
    int functionLineCount = 0;
    bool inFunction = false;

    while (getline(file, line)) {
        lineNumber++;

        checkRawPointers(line, lineNumber, warnings);
        checkNamespaceStd(line, lineNumber, warnings);
        checkVectorAccess(line, lineNumber, warnings);
        checkIOUsage(line, lineNumber, warnings);
        checkConstMethods(line, lineNumber, warnings);
        checkUninitVars(line, lineNumber, warnings);
        checkUnusedIncludes(line, lineNumber);
        checkGoto(line, lineNumber, warnings);
        checkMissingReturnMain(line, lineNumber);

        if (contains(line, "{") && !contains(line, "class") && !contains(line, "namespace") && !inFunction) {
            inFunction = true;
            functionLineCount = 1;
        } else if (inFunction) {
            functionLineCount++;
            if (contains(line, "}")) {
                if (functionLineCount > 50) {
                    cout << YELLOW << "Warning [Line " << lineNumber << "]: Function is too long (" << functionLineCount << " lines)." << RESET << endl;
                    warnings++;
                }
                inFunction = false;
            }
        }
    }

    file.close();
    cout << "\nSummary: " << warnings << " warning(s) found in " << filename << "." << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./linter <filename.cpp>" << endl;
        return 1;
    }

    runLinter(argv[1]);
    return 0;
}
