#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

string* readFile(string filename, int& numVals) {
    ifstream ifs;
    ifs.open(filename);
    if (!ifs.is_open()) {
        exit(1);
    }
    string* vals = new string[20000];
    string line;
    int lineNum = 0;
    while (!ifs.eof()) {
        getline(ifs, line);
        if (line.empty()) {
            lineNum--;
        } else {
            vals[lineNum] = line;
            lineNum++;
        }
    }
    numVals = lineNum + 1;
    return vals;
}

void merge(int vals[], int i, int j, int k) {
    int mergedSize = k - i + 1;
    int mergePos = 0;
    int leftPos = i;
    int rightPos = j + 1;
    int* mergedVals = new int[mergedSize];

    while (leftPos <= j && rightPos <= k) {
        if (vals[leftPos] < vals[rightPos]) {
            mergedVals[mergePos] = vals[leftPos];
            ++leftPos;
        } else {
            mergedVals[mergePos] = vals[rightPos];
            ++rightPos;
        }
        ++mergePos;
    }

    while (leftPos <= j) {
        mergedVals[mergePos] = vals[leftPos];
        ++leftPos;
        ++mergePos;
    }

    while (rightPos <= k) {
        mergedVals[mergePos] = vals[rightPos];
        ++rightPos;
        ++mergePos;
    }

    for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
        vals[i + mergePos] = mergedVals[mergePos];
    }

    delete[] mergedVals;
}

void mergesort(int* vals, int i, int k) {
    int j;
    if (i < k) {
        j = (i + k) / 2;
        mergesort(vals, i, j);
        mergesort(vals, j + 1, k);
        merge(vals, i, j, k);
    }
}

int partition(string vals[], int i, int k) {
    int l = i;
    int h = k;
    int midpoint = i + (k - i) / 2;
    string pivot = vals[midpoint];
    string temp;
    bool done = false;

    while (!done) {
        while (vals[l] < pivot) {
            ++l;
        }
        while (pivot < vals[h]) {
            --h;
        }
        if (l >= h) {
            done = true;
        } else {
            temp = vals[l];
            vals[l] = vals[h];
            vals[h] = temp;
            ++l;
            --h;
        }
    }

    return h;
}

void quicksort(string* vals, int i, int k) {
    int j;
    if (i >= k) {
        return;
    }
    j = partition(vals, i, k);
    quicksort(vals, i, j);
    quicksort(vals, j + 1, k);
}

template <typename T>
void outputOverlap(T* valsOne, T* valsTwo, int numValsOne, int numValsTwo) {
    int i = 0;
    int j = 0;
    T lastFound;
    while (i < numValsOne && j < numValsTwo) {
        if (valsOne[i] == valsTwo[j] && valsOne[i] != lastFound) {
            lastFound = valsOne[i];
            cout << valsOne[i] << endl;
        }
        if (valsOne[i] <= valsTwo[j]) {
            i++;
        } else {
            j++;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        return 1;
    }

    int numValsOne, numValsTwo;
    string* valsOne = readFile(argv[2], numValsOne);
    string* valsTwo = readFile(argv[3], numValsTwo);

    if (strcmp(argv[1], "i") == 0) {
        int* intValsOne = new int[numValsOne];
        int* intValsTwo = new int[numValsTwo];
        for (int i = 0; i < numValsOne; i++) {
            intValsOne[i] = stoi(valsOne[i]);
        }
        for (int i = 0; i < numValsTwo; i++) {
            intValsTwo[i] = stoi(valsTwo[i]);
        }
        mergesort(intValsOne, 0, numValsOne - 1);
        mergesort(intValsTwo, 0, numValsTwo - 1);
        outputOverlap(intValsOne, intValsTwo, numValsOne, numValsTwo);
    } else if (strcmp(argv[1], "s") == 0) {
        quicksort(valsOne, 0, numValsOne - 1);
        quicksort(valsTwo, 0, numValsTwo - 1);
        outputOverlap(valsOne, valsTwo, numValsOne, numValsTwo);
    } else {
        return 1;
    }

    return 0;
}