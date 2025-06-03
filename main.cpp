#include <iostream>
#include <vector>
using namespace std;

bool isSafe(vector<vector<int>>& board, int row, int col, int N) {
    // Verificar la fila a la izquierda
    for (int i = 0; i < col; i++) {
        if (board[row][i]) {
            return false;
        }
    }
    
    // Verificar diagonal superior izquierda
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j]) {
            return false;
        }
    }
    
    // Verificar diagonal inferior izquierda
    for (int i = row + 1, j = col - 1; i < N && j >= 0; i++, j--) {
        if (board[i][j]) {
            return false;
        }
    }
    
    return true;
}

bool solveNQUtil(vector<vector<int>>& board, int col, int N) {
    if (col >= N) {
        return true;
    }
    
    for (int i = 0; i < N; i++) {
        if (isSafe(board, i, col, N)) {
            board[i][col] = 1;
            if (solveNQUtil(board, col + 1, N)) {
                return true;
            }
            board[i][col] = 0;
        }
    }
    return false;
}

int main() {
    int N;
    cin >> N;
    
    vector<vector<int>> board(N, vector<int>(N, 0));
    
    if (solveNQUtil(board, 0, N)) {
        for (int i = 0; i < N; i++) {
            cout << "{";
            for (int j = 0; j < N; j++) {
                if (j == 0) {
                    cout << " " << board[i][j];
                } else {
                    cout << ",  " << board[i][j];
                }
            }
            cout << "}" << endl;
        }
    } else {
        cout << "No existe solucion." << endl;
    }
    
    return 0;
}