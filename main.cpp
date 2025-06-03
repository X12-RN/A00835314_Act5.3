
/* 
A0XXXXXXX - A0YYYYYYY
Actividad 5.3 Implementación backtracking con poda pesada
Fecha: 2024-06-03
Copyright (c) 2024 por A0XXXXXXX y A0YYYYYYY
*/

#include <iostream>
#include <vector>
using namespace std;

bool is_safe(vector<vector<int>>& board, int row, int col, int n) {
    // Verificar la fila hacia la izquierda
    for (int i = 0; i < col; i++) {
        if (board[row][i]) {
            return false;
        }
    }

    // Verificar diagonal superior izquierda
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j]) {
            return false;
        }
    }

    // Verificar diagonal inferior izquierda
    for (int i = row, j = col; j >= 0 && i < n; i++, j--) {
        if (board[i][j]) {
            return false;
        }
    }

    return true;
}

bool solve_n_queens_util(vector<vector<int>>& board, int col, int n) {
    if (col >= n) {
        return true;
    }

    for (int i = 0; i < n; i++) {
        if (is_safe(board, i, col, n)) {
            board[i][col] = 1;
            if (solve_n_queens_util(board, col + 1, n)) {
                return true;
            }
            board[i][col] = 0;
        }
    }
    return false;
}

int main() {
    int n;
    cin >> n;

    vector<vector<int>> board(n, vector<int>(n, 0));

    if (solve_n_queens_util(board, 0, n)) {
        for (int i = 0; i < n; i++) {
            cout << "{";
            for (int j = 0; j < n; j++) {
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
