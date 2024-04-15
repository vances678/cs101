#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Node {
public:
    int value;
    Node* left;
    Node* right;

    Node(int value) : value(value), left(nullptr), right(nullptr) {}
};

class HuffmanTree {
private:
    Node* root;
    map<string, int> codeTable;

    void buildTree(vector<int>& inorder, vector<int>& levelorder, Node* node) {
        if (inorder.empty() || levelorder.empty() || !node) {
            return;
        }

        int rootIndex = -1;
        for (int i = 0; i < inorder.size(); i++) {
            if (inorder.at(i) == node->value) {
                rootIndex = i;
                break;
            }
        }

        vector<int> inorderLeft(inorder.begin(), inorder.begin() + rootIndex);
        vector<int> inorderRight(inorder.begin() + rootIndex + 1, inorder.end());
        vector<int> levelorderLeft, levelorderRight;
        for (int i = 1; i < levelorder.size(); i++) {
            bool found = false;
            for (int j = 0; j < inorderLeft.size(); j++) {
                if (levelorder.at(i) == inorderLeft.at(j)) {
                    found = true;
                    break;
                }
            }
            if (found) {
                levelorderLeft.push_back(levelorder.at(i));
            } else {
                levelorderRight.push_back(levelorder.at(i));
            }
        }

        if (!inorderLeft.empty()) {
            node->left = new Node(levelorderLeft.at(0));
            buildTree(inorderLeft, levelorderLeft, node->left);
        }
        if (!inorderRight.empty()) {
            node->right = new Node(levelorderRight.at(0));
            buildTree(inorderRight, levelorderRight, node->right);
        }
    }

    void generateCodes(Node* node, string code) {
        if (!node->left && !node->right) {
            codeTable[code] = node->value;
            return;
        }

        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }

public:
    HuffmanTree(vector<int>& inorder, vector<int>& levelorder) {
        root = new Node(levelorder.at(0));
        buildTree(inorder, levelorder, root);
        generateCodes(root, "");
    }

    string decode(string encoded) {
        string message = "";
        string currentCode = "";
        for (char c : encoded) {
            currentCode += c;
            if (codeTable.count(currentCode)) {
                message += char(codeTable[currentCode]);
                currentCode = "";
            }
        }
        return message;
    }
};

vector<int> readTraversal(string filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error opening file" << endl;
        exit(1);
    }

    vector<int> traversal;
    int value;
    while (file >> value) {
        traversal.push_back(value);
    }

    file.close();
    return traversal;
}

string readEncoded(string filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error opening file" << endl;
        exit(1);
    }

    string encoded;
    string line;
    while (getline(file, line)) {
        encoded += line;
    }

    file.close();
    return encoded;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Invalid argument count" << endl;
        exit(1);
    }

    vector<int> inorder = readTraversal(argv[1]);
    vector<int> levelorder = readTraversal(argv[2]);
    string encoded = readEncoded(argv[3]);

    HuffmanTree tree(inorder, levelorder);
    string message = tree.decode(encoded);
    cout << message;

    return 0;
}