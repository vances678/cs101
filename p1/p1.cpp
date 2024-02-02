#include <cctype>
#include <climits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

string** readCSV(string filename, int& numRows, int& numCols) {
    ifstream ifs;
    ifs.open(filename);
    if (!ifs.is_open()) {
        cout << "Failed to open \"" + filename + "\"" << endl;
        exit(1);
    }
    string** data = new string*[5000];
    string line;
    int lineNum = 0;
    while (!ifs.eof()) {
        getline(ifs, line);
        if (line.empty()) {
            lineNum--;
        } else {
            istringstream iss(line);
            string* row = new string[50];
            string value;
            int colNum = 0;
            while (getline(iss, value, ',')) {
                row[colNum] = value;
                colNum++;
            }
            numCols = colNum + 1;
            string* resizedRow = new string[colNum + 1];
            for (int i = 0; i < colNum + 1; i++) {
                resizedRow[i] = row[i];
            }
            delete[] row;
            data[lineNum] = resizedRow;
            lineNum++;
        }
    }
    numRows = lineNum + 1;
    string** resizedData = new string*[lineNum + 1];
    for (int i = 0; i < lineNum + 1; i++) {
        resizedData[i] = data[i];
    }
    delete[] data;
    return resizedData;
}

void displayTable(string** data, int numRows, int numCols, int colWidth) {
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            cout << setw(colWidth) << data[r][c];
        }
        cout << endl;
    }
}

double* getVals(string** data, int numRows, int numCols, string colName, int& valNum) {
    double* vals = new double[numRows - 1]();
    int colIndex = -1;
    for (int c = 0; c < numCols; c++) {
        if (data[0][c] == colName) {
            colIndex = c;
            break;
        }
    }
    if (colIndex != -1) {
        for (int r = 1; r < numRows; r++) {
            string val = data[r][colIndex];
            if (val != "") {
                vals[valNum] = stod(val);
                valNum++;
            }
        }
    }
    return vals;
}

void help() {
    cout << "command list:" << endl;
    cout << "        print" << endl;
    cout << "        cols" << endl;
    cout << "        search *|col_name val" << endl;
    cout << "        min col_name" << endl;
    cout << "        max col_name" << endl;
    cout << "        avg col_name" << endl;
}

void cols(string* columns, int numCols) {
    for (int i = 0; i < numCols; i++) {
        cout << columns[i] << endl;
    }
}

void search(string** data, int numRows, int numCols, string colName, string value, int colWidth) {
    int colIndex = -1;
    for (int c = 0; c < numCols; c++) {
        if (data[0][c] == colName) {
            colIndex = c;
            break;
        }
    }
    if (colIndex == -1 && colName != "*") {
        cout << "No results" << endl;
    } else {
        string** foundRows = new string*[numRows];
        foundRows[0] = data[0];
        int numFoundRows = 1;
        for (int r = 0; r < numRows; r++) {
            if (colName == "*") {
                for (int c = 0; c < numCols; c++) {
                    if (data[r][c] == value) {
                        foundRows[numFoundRows] = data[r];
                        numFoundRows++;
                        break;
                    }
                }
            } else {
                if (data[r][colIndex] == value) {
                    foundRows[numFoundRows] = data[r];
                    numFoundRows++;
                }
            }
        }
        if (numFoundRows == 1) {
            cout << "No results" << endl;
        } else {
            displayTable(foundRows, numFoundRows, numCols, colWidth);
            delete[] foundRows;
        }
    }
}

void min(string** data, int numRows, int numCols, string colName) {
    int valNum = 0;
    double* vals = getVals(data, numRows, numCols, colName, valNum);
    double min = LONG_MAX;
    for (int v = 0; v < valNum; v++) {
        if (vals[v] < min) {
            min = vals[v];
        }
    }
    cout << "The min for col \"" << colName << "\" = " << min << endl;
}

void max(string** data, int numRows, int numCols, string colName) {
    int valNum = 0;
    double* vals = getVals(data, numRows, numCols, colName, valNum);
    double max = LONG_MIN;
    for (int v = 0; v < valNum; v++) {
        if (vals[v] > max) {
            max = vals[v];
        }
    }
    cout << "The max for col \"" << colName << "\" = " << max << endl;
}

void avg(string** data, int numRows, int numCols, string colName) {
    int valNum = 0;
    double* vals = getVals(data, numRows, numCols, colName, valNum);
    double avg = 0;
    for (int v = 0; v < valNum; v++) {
        avg += vals[v];
    }
    avg /= valNum;
    cout << "The avg for col \"" << colName << "\" = " << ((valNum == 0) ? "N/A" : to_string(avg)) << endl;
}

int main(int argc, char* argv[]) {
    if (argc == 3) {
        int numRows;
        int numCols;
        string** data = readCSV(argv[1], numRows, numCols);
        string input;
        while (true) {
            cout << "Enter a command or \"help\" for a command list:" << endl;
            getline(cin, input);
            input += " ";
            string command, colName, value;
            string argument = "";
            int argNum = 0;
            for (int i = 0; i < input.length(); i++) {
                char c = input.at(i);
                if (c == '\"') {
                    i++;
                    while (input.at(i) != '\"') {
                        argument += input.at(i);
                        i++;
                    }
                } else if (c == ' ') {
                    if (argNum == 0) {
                        command = argument;
                    } else if (argNum == 1) {
                        colName = argument;
                    } else if (argNum == 2) {
                        value = argument;
                    }
                    argument = "";
                    argNum++;
                    continue;
                } else {
                    argument += c;
                }
            }
            bool isValidCol = true;
            if (colName != "") {
                bool colExists = false;
                for (int i = 0; i < numCols; i++) {
                    if (colName == data[0][i] || colName == "*") {
                        colExists = true;
                    }
                }
                if (!colExists) {
                    isValidCol = false;
                }
            }
            if (command == "quit") {
                return 0;
            } else if (command == "help") {
                help();
            } else if (command == "print") {
                displayTable(data, numRows, numCols, atoi(argv[2]));
            } else if (command == "cols") {
                cols(data[0], numCols);
            } else if (command == "search") {
                if (!isValidCol) {
                    cout << "Invalid column \"" + colName + "\"" << endl;
                } else {
                    search(data, numRows, numCols, colName, value, atoi(argv[2]));
                }
            } else if (command == "min") {
                if (!isValidCol) {
                    cout << "Invalid column \"" + colName + "\"" << endl;
                } else {
                    min(data, numRows, numCols, colName);
                }
            } else if (command == "max") {
                if (!isValidCol) {
                    cout << "Invalid column \"" + colName + "\"" << endl;
                } else {
                    max(data, numRows, numCols, colName);
                }
            } else if (command == "avg") {
                if (!isValidCol) {
                    cout << "Invalid column \"" + colName + "\"" << endl;
                } else {
                    avg(data, numRows, numCols, colName);
                }
            } else {
                cout << "Invalid command" << endl;
            }
            cout << endl;
        }
    } else {
        cout << "Usage: ./a database.csv #col_width" << endl;
        return 1;
    }
    return 0;
}