#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

class NQueens {
private:
    int n;
    vector<vector<int>> board;
    vector<bool> cols;        // Para verificar columnas ocupadas
    vector<bool> diag1;       // Para verificar diagonales (\)
    vector<bool> diag2;       // Para verificar diagonales (/)
    
public:
    NQueens(int size) : n(size) {
        board.assign(n, vector<int>(n, 0));
        cols.assign(n, false);
        diag1.assign(2*n-1, false);  // row-col+n-1
        diag2.assign(2*n-1, false);  // row+col
    }
    
    // Verifica si es seguro colocar una reina en (row, col)
    bool isSafe(int row, int col) {
        return !cols[col] && 
               !diag1[row - col + n - 1] && 
               !diag2[row + col];
    }
    
    // Calcula el grado de una posición (heurística para poda pesada)
    int calculateDegree(int row, int col) {
        int degree = 0;
        
        // Contar posiciones que pueden ser atacadas
        for (int i = 0; i < n; i++) {
            // Columna
            if (i != row && !cols[col]) degree++;
            // Fila
            if (i != col && isSafe(row, i)) degree++;
        }
        
        // Diagonales
        for (int i = 1; i < n; i++) {
            // Diagonal principal (\)
            if (isValidPosition(row + i, col + i) && isSafe(row + i, col + i)) degree++;
            if (isValidPosition(row - i, col - i) && isSafe(row - i, col - i)) degree++;
            
            // Diagonal secundaria (/)
            if (isValidPosition(row + i, col - i) && isSafe(row + i, col - i)) degree++;
            if (isValidPosition(row - i, col + i) && isSafe(row - i, col + i)) degree++;
        }
        
        return degree;
    }
    
    bool isValidPosition(int row, int col) {
        return row >= 0 && row < n && col >= 0 && col < n;
    }
    
    // Obtiene las posiciones válidas ordenadas por grado (poda pesada)
    vector<int> getValidPositions(int row) {
        vector<pair<int, int>> validCols;
        
        for (int col = 0; col < n; col++) {
            if (isSafe(row, col)) {
                int degree = calculateDegree(row, col);
                validCols.push_back({degree, col});
            }
        }
        
        // Ordenar por grado descendente (mayor grado primero)
        sort(validCols.begin(), validCols.end(), greater<pair<int,int>>());
        
        vector<int> result;
        for (const auto& p : validCols) {
            result.push_back(p.second);
        }
        return result;
    }
    
    // Cuenta posiciones válidas restantes (para poda)
    int countValidPositions(int startRow) {
        int count = 0;
        for (int i = startRow; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (isSafe(i, j)) {
                    count++;
                }
            }
        }
        return count;
    }
    
    void placeQueen(int row, int col) {
        board[row][col] = 1;
        cols[col] = true;
        diag1[row - col + n - 1] = true;
        diag2[row + col] = true;
    }
    
    void removeQueen(int row, int col) {
        board[row][col] = 0;
        cols[col] = false;
        diag1[row - col + n - 1] = false;
        diag2[row + col] = false;
    }
    
    // Poda pesada: verifica si hay suficientes posiciones válidas
    bool hasSufficientPositions(int row) {
        return countValidPositions(row) >= (n - row);
    }
    
    // Algoritmo de backtracking con poda pesada
    bool solveNQueens(int row) {
        if (row == n) {
            return true; // Todas las reinas han sido colocadas
        }
        
        // Poda pesada: si no hay suficientes posiciones, retroceder
        if (!hasSufficientPositions(row)) {
            return false;
        }
        
        // Obtener posiciones válidas ordenadas por grado
        vector<int> validCols = getValidPositions(row);
        
        for (int col : validCols) {
            placeQueen(row, col);
            
            if (solveNQueens(row + 1)) {
                return true;
            }
            
            removeQueen(row, col);
        }
        
        return false;
    }
    
    bool solve() {
        if (solveNQueens(0)) {
            printSolution();
            return true;
        } else {
            cout << "No existe solución para N = " << n << endl;
            return false;
        }
    }
    
    void printSolution() {
        for (int i = 0; i < n; i++) {
            cout << "{";
            for (int j = 0; j < n; j++) {
                if (j > 0) cout << ",";
                cout << setw(3) << board[i][j];
            }
            cout << "}" << endl;
        }
    }
};

int main() {
    int n;
    cin >> n;
    
    // Validar que N sea 4 u 8
    if (n != 4 && n != 8) {
        cout << "Error: N debe ser 4 u 8" << endl;
        return 1;
    }
    
    NQueens nqueens(n);
    nqueens.solve();
    
    return 0;
}
