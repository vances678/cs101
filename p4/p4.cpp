#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

unsigned long djb2(string str) {
    const char* ptr = str.c_str();
    unsigned long hash = 5381;
    int c;
    while ((c = *ptr++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

class HashTable {
private:
    struct TableItem {
        string key;
        vector<int> value;
    };
    vector<TableItem> items;
    int size;
    int count;
    int uniqueCount;
    string strategy;
    int a;
    int collisions;

public:
    HashTable(int tableSize, string collisionStrategy, int dha) {
        items.resize(tableSize);
        size = tableSize;
        count = 0;
        uniqueCount = 0;
        strategy = collisionStrategy;
        a = dha;
        collisions = 0;
    }

    void resize(int newSize) {
        vector<TableItem> newItems(newSize);
        vector<TableItem> oldItems = items;
        items = newItems;
        int oldSize = size;
        size = newSize;
        for (int i = 0; i < oldSize; i++) {
            TableItem& item = oldItems.at(i);
            if (!item.key.empty()) {
                insert(item.key, item.value);
            }
        }
    }

    void insert(string word, vector<int> lineNumbers) {
        int h1 = djb2(word) % size;
        int h2 = a - (djb2(word) % a);
        int index = h1;
        int j = 0;
        bool isInserted = false;
        while (!isInserted) {
            TableItem& item = items.at(index);
            if (item.key == word) {
                for (int num : lineNumbers) {
                    item.value.push_back(num);
                }
                count++;
                isInserted = true;
            } else if (item.key.empty()) {
                item.key = word;
                item.value = lineNumbers;
                uniqueCount++;
                count++;
                isInserted = true;
            } else {
                collisions++;
                j++;
                if (strategy == "lp") {
                    index = (h1 + j) % size;
                } else if (strategy == "qp") {
                    index = (h1 + j * j) % size;
                } else if (strategy == "dh") {
                    index = (h1 + j * h2) % size;
                } else {
                    cout << "Error: invalid collision strategy" << endl;
                    exit(1);
                }
            }
        }
    }

    vector<int> find(string word) {
        vector<int> result;
        collisions = 0;
        int h1 = djb2(word) % size;
        int h2 = a - (djb2(word) % a);
        int index = h1;
        int j = 0;
        bool isFound = false;
        while (!isFound) {
            TableItem& item = items.at(index);
            if (item.key == word) {
                result = item.value;
                isFound = true;
            } else if (item.key.empty()) {
                break;
            } else {
                collisions++;
                j++;
                if (strategy == "lp") {
                    index = (h1 + j) % size;
                } else if (strategy == "qp") {
                    index = (h1 + j * j) % size;
                } else if (strategy == "dh") {
                    index = (h1 + j * h2) % size;
                } else {
                    cout << "Error: invalid collision strategy" << endl;
                    exit(1);
                }
            }
        }
        return result;
    }

    int getCount() {
        return count;
    }

    int getUniqueCount() {
        return uniqueCount;
    }

    int getCollisions() {
        return collisions;
    }
};

void readWords(string& inputFile, HashTable& hashTable) {
    ifstream file(inputFile);
    if (file.is_open()) {
        string line;
        int lineNum = 0;
        while (getline(file, line)) {
            lineNum++;
            string word = "";
            for (char c : line) {
                if (isalpha(c)) {
                    word += tolower(c);
                } else if (word != "") {
                    vector<int> lineNumVector;
                    lineNumVector.push_back(lineNum);
                    hashTable.insert(word, lineNumVector);
                    word = "";
                }
            }
            if (word != "") {
                vector<int> lineNumVector;
                lineNumVector.push_back(lineNum);
                hashTable.insert(word, lineNumVector);
            }
        }
        file.close();
    } else {
        cout << "Error: unable to open file " << inputFile << endl;
        exit(1);
    }
}

vector<string> readQueryWords(string& queryFile) {
    vector<string> words;
    ifstream file(queryFile);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            string word = "";
            for (char c : line) {
                if (isalpha(c)) {
                    word += tolower(c);
                } else if (word != "") {
                    words.push_back(word);
                    word = "";
                }
            }
            if (word != "") {
                words.push_back(word);
            }
        }
        file.close();
    } else {
        cout << "Error: unable to open file " << queryFile << endl;
        exit(1);
    }
    return words;
}

int main(int argc, char** argv) {
    string inputFile;
    string queryFile;
    int tableSize;
    string collisionStrategy;
    int dha = -1;
    if (argc == 5 || argc == 6) {
        inputFile = argv[1];
        queryFile = argv[2];
        tableSize = stoi(argv[3]);
        collisionStrategy = argv[4];
        if (argc == 6) {
            dha = stoi(argv[5]);
        }
    } else {
        return 1;
    }

    HashTable hashTable = HashTable(tableSize, collisionStrategy, dha);

    readWords(inputFile, hashTable);
    cout << "The number of words found in the file was " << hashTable.getCount() << endl;
    cout << "The number of unique words found in the file was " << hashTable.getUniqueCount() << endl;
    cout << "The number of collisions was " << hashTable.getCollisions() << endl;
    vector<string> words = readQueryWords(queryFile);
    for (string word : words) {
        vector<int> lineNumbers = hashTable.find(word);
        cout << endl << word << " appears on lines [";
        for (int num : lineNumbers) {
            cout << num;
            if (num != lineNumbers.back()) {
                cout << ",";
            }
        }
        cout << "]" << endl;
        cout << "The search had " << hashTable.getCollisions() << " collisions" << endl;
    }

    return 0;
}