#include <iostream>
#include <vector>

using namespace std;

static int solution_count = 0;

bool can_place(vector<int>& matrix, int col, int row) {
    for (int k = 0; k < col; k++) {
        // Check left
        if (matrix[k] == row) {
            return false;
        }

        // Check diagonals
        if ((k - matrix[k]) == (col - row)) {
            return false;
        }

        if ((k + matrix[k]) == (col + row)) {
            return false;
        }
    }

    return true;
}

void solve(vector<int>& matrix, int col) {
    if (col == matrix.size()) {
        cout << "Solution #" << ++solution_count << endl;
        for (int col = 0; col < matrix.size(); col++) {
            cout << "[" << matrix[col] << ", " << col << "]" << endl;
        }
        return;
    }

    for (int row = 0; row < matrix.size(); ++row) {
        if (can_place(matrix, col, row)) {
            matrix[col] = row;
            solve(matrix, col + 1);
        }
    }
}

int main() {
    vector<int> matrix(8);
    solve(matrix, 0);
    return 0;
}
