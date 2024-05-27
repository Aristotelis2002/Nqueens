#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>

#include <numeric>

using namespace std;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> distribution(1, 10);
int timesRestarted = 0;

int getRandomIntFast(int n) {
    return std::rand() % n;
}

struct vectorOfMaxes {
    int arrayMaxes[100000];
    int size;
    int index;
    int currentMax;
    vectorOfMaxes() {
        size = 0;
        index = 0;
        currentMax = 0;
    }

    private:
    void pushElement(int element) {
        arrayMaxes[index] = element;
        index++;
        size++;
    }

    public:
    void tryAdding(int element, int confVal) {
        if (size >= 99998) {
            return;
        }
        if (confVal == currentMax) {
            pushElement(element);
        }
        if (confVal > currentMax) {
            currentMax = confVal;
            index = 0;
            size = 0;
            pushElement(element);
        }
    }
    int getMax() {
        int toBeReturned = arrayMaxes[getRandomIntFast(size)];
        size = 0;
        index = 0;
        currentMax = 0;
        return toBeReturned;
    }
    void clear() {
        index = 0;
        size = 0;
        currentMax = 0;
    }
};

void generateRandomPermutation(vector<int>& numbers, int n) {
    for (int i = 0; i < n; ++i) {
        numbers[i] = i;
    }
    std::shuffle(numbers.begin(), numbers.end(), gen);
}

void printSolution(const vector<int>& board) {
    for (size_t i = 0; i < board.size(); i++) {
        for (size_t j = 0; j < board.size(); j++) {
            if (board[j] == i) {
                cout << "* ";
            } else {
                cout << "_ ";
            }
        }
        cout << endl;
    }
}

void printVector(const vector<int>& vektor) {
    for (size_t i = 0; i < vektor.size(); i++) {
        cout << vektor[i] << " ";
    }
    cout << endl;
}

int getConflictValueTwoIndexes(int col,
                               int row,
                               const vector<int>& numberOfQueensOnRow,
                               const vector<int>& diagonalLeftToRight,
                               const vector<int>& diagonalRightToLeft,
                               int oldRow) {
    int conflictValue = 0;
    if (row == oldRow) {
        conflictValue -= 3;
    }
    conflictValue += max(numberOfQueensOnRow[row], 0);
    conflictValue += max(diagonalLeftToRight[col + row], 0);
    conflictValue += max(
        diagonalRightToLeft[(numberOfQueensOnRow.size() - row - 1) + col], 0);
    return conflictValue;
}

int getConflictValue(int index,
                     const vector<int>& board,
                     const vector<int>& numberOfQueensOnRow,
                     const vector<int>& diagonalLeftToRight,
                     const vector<int>& diagonalRightToLeft) {
    int conflictValue = 0;
    int col = index;
    int row = board[index];
    conflictValue += max(numberOfQueensOnRow[row] - 1, 0);
    conflictValue += max(diagonalLeftToRight[col + row] - 1, 0);
    conflictValue += max(
        diagonalRightToLeft[(numberOfQueensOnRow.size() - row - 1) + col] - 1,
        0);
    return conflictValue;
}

void calculateConflicts(const vector<int>& board,
                        const vector<int>& numberOfQueensOnRow,
                        const vector<int>& diagonalLeftToRight,
                        const vector<int>& diagonalRightToLeft,
                        vector<int>& conflicts,
                        vectorOfMaxes& maxes) {
    for (int i = 0; i < board.size(); i++) {
        conflicts[i] =
            getConflictValue(i, board, numberOfQueensOnRow, diagonalLeftToRight,
                             diagonalRightToLeft);
        maxes.tryAdding(i, conflicts[i]);
    }
}

bool factCheckBoard(const vector<int>& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = i; j < board.size() - 1; j++) {
            if (board[i] == board[j + 1]) {
                printf("Row i:%d\n", i);
                return false;
            }
        }
    }
    int n = board.size();
    vector<int> numberOfQueensOnRow(n);
    vector<int> diagonalLeftToRight(2 * n - 1);
    vector<int> diagonalRightToLeft(2 * n - 1);
    vector<int> conflicts(n);
    vectorOfMaxes maxes = vectorOfMaxes();
    calculateConflicts(board, numberOfQueensOnRow, diagonalLeftToRight,
                       diagonalRightToLeft, conflicts, maxes);
    for (int i = 0; i < n; i++) {
        if (conflicts[i] != 0) {
            printf("i: %d, conf[i]:%d\n", i, conflicts[i]);
            return false;
        }
    }

    return true;
}

void calculateVectorsRowDiag(const vector<int>& board,
                             vector<int>& numberOfQueensOnRow,
                             vector<int>& diagonalLeftToRight,
                             vector<int>& diagonalRightToLeft) {
    int n = board.size();
    for (int i = 0; i < n; i++) {
        numberOfQueensOnRow[board[i]]++;
        diagonalLeftToRight[i + board[i]]++;
        diagonalRightToLeft[n - board[i] - 1 + i]++;
    }
}

void updateVectorsAfterSwap(int rowOld,
                            int rowNew,
                            int col,
                            vector<int>& numberOfQueensOnRow,
                            vector<int>& diagonalLeftToRight,
                            vector<int>& diagonalRightToLeft) {
    numberOfQueensOnRow[rowOld]--;
    diagonalLeftToRight[rowOld + col]--;
    diagonalRightToLeft[(numberOfQueensOnRow.size() - rowOld - 1) + col]--;

    numberOfQueensOnRow[rowNew]++;
    diagonalLeftToRight[rowNew + col]++;
    diagonalRightToLeft[(numberOfQueensOnRow.size() - rowNew - 1) + col]++;
}

void updateConflicts(int rowOld,
                     int rowNew,
                     int col,
                     const vector<int>& board,
                     vector<int>& conflicts,
                     vectorOfMaxes& maxes) {
    int diagL1Val = col + rowOld;
    int diagR1Val = board.size() - rowOld - 1 + col;
    int diagL2Val = col + rowNew;
    int diagR2Val = board.size() - rowNew - 1 + col;
    int n = board.size();
    for (int i = 0; i < board.size(); i++) {
        if (i == col) {
            continue;
        }
        if (board[i] == rowOld) {
            conflicts[i]--;
        }
        if ((board[i] + i) == diagL1Val) {
            conflicts[i]--;
        }
        if ((n - board[i] - 1 + i) == diagR1Val) {
            conflicts[i]--;
        }
        if (board[i] == rowNew) {
            conflicts[i]++;
        }
        if ((board[i] + i) == diagL2Val) {
            conflicts[i]++;
        }
        if ((n - board[i] - 1 + i) == diagR2Val) {
            conflicts[i]++;
        }
        maxes.tryAdding(i, conflicts[i]);
    }
}

int getOptimalIndex(int col,
                    int oldVal,
                    const vector<int>& board,
                    const vector<int>& numberOfQueensOnRow,
                    const vector<int>& diagonalLeftToRight,
                    const vector<int>& diagonalRightToLeft,
                    int& newVal) {
    int optimalIndex = board[col];
    int oldRow = board[col];
    int minVal = oldVal;
    int currentVal;
    for (int i = 0; i < board.size(); i++) {
        currentVal = getConflictValueTwoIndexes(col, i, numberOfQueensOnRow,
                                                diagonalLeftToRight,
                                                diagonalRightToLeft, oldRow);
        if (currentVal == minVal && distribution(gen) == 1) {
            optimalIndex = i;
        }
        if (currentVal < minVal) {
            minVal = currentVal;
            optimalIndex = i;
        }
    }
    newVal = minVal;
    return optimalIndex;
}

void nullify(vector<int>& vektor) {
    for (size_t i = 0; i < vektor.size(); i++) {
        vektor[i] = 0;
    }
}

void restart(vector<int>& board,
             vector<int>& numberOfQueensOnRow,
             vector<int>& diagonalLeftToRight,
             vector<int>& diagonalRightToLeft,
             vector<int>& conflicts,
             vectorOfMaxes& maxes) {
    generateRandomPermutation(board, board.size());
    nullify(numberOfQueensOnRow);
    nullify(diagonalLeftToRight);
    nullify(diagonalRightToLeft);
    calculateVectorsRowDiag(board, numberOfQueensOnRow, diagonalLeftToRight,
                            diagonalRightToLeft);
    maxes.clear();
    calculateConflicts(board, numberOfQueensOnRow, diagonalLeftToRight,
                       diagonalRightToLeft, conflicts, maxes);
    timesRestarted++;
}

void solve(int n,
           vector<int>& board,
           vector<int>& numberOfQueensOnRow,
           vector<int>& diagonalLeftToRight,
           vector<int>& diagonalRightToLeft,
           vector<int>& conflicts) {
    vectorOfMaxes maxes = vectorOfMaxes();
    calculateVectorsRowDiag(board, numberOfQueensOnRow, diagonalLeftToRight,
                            diagonalRightToLeft);
    calculateConflicts(board, numberOfQueensOnRow, diagonalLeftToRight,
                       diagonalRightToLeft, conflicts, maxes);
    int maxCol;
    int newRow;
    int oldRow;
    int newVal;
    int failCount = 0;
    while (true) {
        maxCol = maxes.getMax();
        if (conflicts[maxCol] == 0) {
            break;
        }
        oldRow = board[maxCol];
        newRow = getOptimalIndex(maxCol, conflicts[maxCol], board,
                                 numberOfQueensOnRow, diagonalLeftToRight,
                                 diagonalRightToLeft, newVal);
        if (oldRow == newRow) {
            failCount++;
        } else {
            if (failCount > 0) {
                failCount--;
            }
        }
        if (failCount == 3) {
            failCount = 0;
            restart(board, numberOfQueensOnRow, diagonalLeftToRight,
                    diagonalRightToLeft, conflicts, maxes);
            continue;
        }
        board[maxCol] = newRow;
        conflicts[maxCol] = newVal;
        updateVectorsAfterSwap(oldRow, newRow, maxCol, numberOfQueensOnRow,
                               diagonalLeftToRight, diagonalRightToLeft);
        updateConflicts(oldRow, newRow, maxCol, board, conflicts, maxes);
    }
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int n;
    cin >> n;
    if (n < 4) {
        cout << "-1" << endl;
        return 0;
    }
    vector<int> board(n);
    vector<int> numberOfQueensOnRow(n);
    vector<int> diagonalLeftToRight(2 * n - 1);
    vector<int> diagonalRightToLeft(2 * n - 1);
    vector<int> conflicts(n);
    generateRandomPermutation(board, n);

    auto start = std::chrono::high_resolution_clock::now();
    solve(n, board, numberOfQueensOnRow, diagonalLeftToRight,
          diagonalRightToLeft, conflicts);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    double seconds = duration.count() / 1e3;

    if (n < 101) {
        printSolution(board);
    } else {
        std::cout << "Time taken by function: " << std::fixed
          << std::setprecision(3) << seconds << " seconds" << std::endl;
    }
    return 0;
}