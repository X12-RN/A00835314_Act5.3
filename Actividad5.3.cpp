#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cassert>
#include <sstream>
using namespace std;

class NQueens {
private:
    int n;
    vector<vector<int>> board;
    vector<bool> cols;
    vector<bool> diag1;
    vector<bool> diag2;
    
public:
    NQueens(int size) : n(size) {
        board.assign(n, vector<int>(n, 0));
        cols.assign(n, false);
        diag1.assign(2*n-1, false);
        diag2.assign(2*n-1, false);
    }
    
    bool IsSafe(int row, int col) {
        return !cols[col] && !diag1[row - col + n - 1] && !diag2[row + col];
    }
    
    int CalculateDegrees(int row, int col) {
        int degree = 0;
        degree += (n - 1) * 2;
        degree += CountDiagonalPositions(row, col, 1, 1);
        degree += CountDiagonalPositions(row, col, 1, -1);
        return degree;
    }
    
    int CountDiagonalPositions(int row, int col, int deltaRow, int deltaCol) {
        int count = 0;
        count += CountInDirection(row, col, deltaRow, deltaCol);
        count += CountInDirection(row, col, -deltaRow, -deltaCol);
        return count;
    }
    
    int CountInDirection(int row, int col, int deltaRow, int deltaCol) {
        int count = 0;
        int r = row + deltaRow;
        int c = col + deltaCol;
        
        while (IsValidPosition(r, c)) {
            count++;
            r += deltaRow;
            c += deltaCol;
        }
        
        return count;
    }
    
    bool IsValidPosition(int row, int col) {
        return row >= 0 && row < n && col >= 0 && col < n;
    }
    
    vector<int> GetValidPositions(int row) {
        vector<pair<int, int>> validCols;
        
        for (int col = 0; col < n; col++) {
            if (IsSafe(row, col)) {
                int degree = CalculateDegrees(row, col);
                validCols.push_back({degree, col});
            }
        }
        
        sort(validCols.begin(), validCols.end(), greater<pair<int,int>>());
        return ExtractColumns(validCols);
    }
    
    vector<int> ExtractColumns(const vector<pair<int, int>>& validCols) {
        vector<int> result;
        for (const auto& p : validCols) {
            result.push_back(p.second);
        }
        return result;
    }
    
    int CountValidPositions(int startRow) {
        int count = 0;
        for (int i = startRow; i < n; i++) {
            count += CountValidInRow(i);
        }
        return count;
    }
    
    int CountValidInRow(int row) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (IsSafe(row, j)) {
                count++;
            }
        }
        return count;
    }
    
    void PlaceQueen(int row, int col) {
        board[row][col] = 1;
        cols[col] = true;
        diag1[row - col + n - 1] = true;
        diag2[row + col] = true;
    }
    
    void RemoveQueen(int row, int col) {
        board[row][col] = 0;
        cols[col] = false;
        diag1[row - col + n - 1] = false;
        diag2[row + col] = false;
    }
    
    bool HasSufficientPositions(int row) {
        return CountValidPositions(row) >= (n - row);
    }
    
    bool SolveNQueens(int row) {
        if (row == n) {
            return true;
        }
        
        if (!HasSufficientPositions(row)) {
            return false;
        }
        
        vector<int> validCols = GetValidPositions(row);
        return TryAllPositions(row, validCols);
    }
    
    bool TryAllPositions(int row, const vector<int>& validCols) {
        for (int col : validCols) {
            PlaceQueen(row, col);
            
            if (SolveNQueens(row + 1)) {
                return true;
            }
            
            RemoveQueen(row, col);
        }
        return false;
    }
    
    bool Solve() {
        if (SolveNQueens(0)) {
            PrintSolution();
            return true;
        }
        
        PrintNoSolution();
        return false;
    }
    
    void PrintNoSolution() {
        cout << "No existe solución para N = " << n << endl;
    }
    
    void PrintSolution() {
        for (int i = 0; i < n; i++) {
            PrintRow(i);
        }
    }
    
    void PrintRow(int row) {
        cout << "{";
        for (int j = 0; j < n; j++) {
            if (j > 0) {
                cout << ",";
            }
            cout << setw(3) << board[row][j];
        }
        cout << "}" << endl;
    }
    
    // Métodos adicionales para testing
    vector<vector<int>> GetBoard() {
        return board;
    }
    
    bool HasValidSolution() {
        // Verificar que hay exactamente n reinas
        int queenCount = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 1) {
                    queenCount++;
                }
            }
        }
        
        if (queenCount != n) {
            return false;
        }
        
        // Verificar que no se atacan entre sí
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 1) {
                    if (!IsQueenSafe(i, j)) {
                        return false;
                    }
                }
            }
        }
        
        return true;
    }
    
    bool IsQueenSafe(int row, int col) {
        // Verificar fila
        for (int j = 0; j < n; j++) {
            if (j != col && board[row][j] == 1) {
                return false;
            }
        }
        
        // Verificar columna
        for (int i = 0; i < n; i++) {
            if (i != row && board[i][col] == 1) {
                return false;
            }
        }
        
        // Verificar diagonales
        return CheckDiagonals(row, col);
    }
    
    bool CheckDiagonals(int row, int col) {
        // Diagonal principal
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != row && j != col && (i - j) == (row - col) && board[i][j] == 1) {
                    return false;
                }
            }
        }
        
        // Diagonal secundaria
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != row && j != col && (i + j) == (row + col) && board[i][j] == 1) {
                    return false;
                }
            }
        }
        
        return true;
    }
};

bool is_valid_input(int n) {
    bool isValid4 = (n == 4);
    bool isValid8 = (n == 8);
    return isValid4 || isValid8;
}

// Funciones de prueba
void test_is_valid_input() {
    assert(is_valid_input(4) == true);
    assert(is_valid_input(8) == true);
    assert(is_valid_input(3) == false);
    assert(is_valid_input(5) == false);
    assert(is_valid_input(0) == false);
    cout << "✓ test_is_valid_input passed" << endl;
}

void test_nqueens_constructor() {
    NQueens nq4(4);
    NQueens nq8(8);
    cout << "✓ test_nqueens_constructor passed" << endl;
}

void test_is_valid_position() {
    NQueens nq(4);
    assert(nq.IsValidPosition(0, 0) == true);
    assert(nq.IsValidPosition(3, 3) == true);
    assert(nq.IsValidPosition(-1, 0) == false);
    assert(nq.IsValidPosition(0, -1) == false);
    assert(nq.IsValidPosition(4, 0) == false);
    assert(nq.IsValidPosition(0, 4) == false);
    cout << "✓ test_is_valid_position passed" << endl;
}

void test_is_safe() {
    NQueens nq(4);
    assert(nq.IsSafe(0, 0) == true);
    assert(nq.IsSafe(0, 1) == true);
    
    nq.PlaceQueen(0, 1);
    assert(nq.IsSafe(0, 0) == false); // Misma fila
    assert(nq.IsSafe(1, 1) == false); // Misma columna
    assert(nq.IsSafe(1, 2) == false); // Diagonal
    
    cout << "✓ test_is_safe passed" << endl;
}

void test_place_remove_queen() {
    NQueens nq(4);
    
    nq.PlaceQueen(0, 1);
    assert(nq.GetBoard()[0][1] == 1);
    
    nq.RemoveQueen(0, 1);
    assert(nq.GetBoard()[0][1] == 0);
    
    cout << "✓ test_place_remove_queen passed" << endl;
}

void test_solve_4queens() {
    NQueens nq(4);
    bool solved = nq.SolveNQueens(0);
    assert(solved == true);
    assert(nq.HasValidSolution() == true);
    cout << "✓ test_solve_4queens passed" << endl;
}

void test_solve_8queens() {
    NQueens nq(8);
    bool solved = nq.SolveNQueens(0);
    assert(solved == true);
    assert(nq.HasValidSolution() == true);
    cout << "✓ test_solve_8queens passed" << endl;
}

void test_calculate_degrees() {
    NQueens nq(4);
    int degrees = nq.CalculateDegrees(0, 0);
    assert(degrees > 0);
    cout << "✓ test_calculate_degrees passed" << endl;
}

void test_count_valid_positions() {
    NQueens nq(4);
    int count = nq.CountValidPositions(0);
    assert(count > 0);
    cout << "✓ test_count_valid_positions passed" << endl;
}

void run_all_tests() {
    cout << "Running all tests..." << endl;
    
    test_is_valid_input();
    test_nqueens_constructor();
    test_is_valid_position();
    test_is_safe();
    test_place_remove_queen();
    test_calculate_degrees();
    test_count_valid_positions();
    test_solve_4queens();
    test_solve_8queens();
    
    cout << "All tests passed! ✓" << endl;
}

int main() {
    #ifdef TESTING
        run_all_tests();
        return 0;
    #endif
    
    int n;
    cin >> n;
    
    if (!is_valid_input(n)) {
        cout << "Error: N debe ser 4 u 8" << endl;
        return 1;
    }
    
    NQueens nqueens(n);
    nqueens.Solve();
    
    return 0;
}