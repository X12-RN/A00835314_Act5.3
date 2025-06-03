#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
using namespace std;

class NQueens {
private:
    int n;
    vector<vector<int>> board;
    vector<bool> cols;        // Track occupied columns
    vector<bool> diag1;       // Track main diagonals (\) - indexed by (row - col + n - 1)
    vector<bool> diag2;       // Track anti-diagonals (/) - indexed by (row + col)
    long long nodes_explored; // Counter for performance analysis
    
public:
    NQueens(int size) : n(size), nodes_explored(0) {
        board.assign(n, vector<int>(n, 0));
        cols.assign(n, false);
        diag1.assign(2 * n - 1, false);
        diag2.assign(2 * n - 1, false);
    }
    
    // Check if placing a queen at (row, col) is safe
    bool isSafe(int row, int col) {
        return !cols[col] && 
               !diag1[row - col + n - 1] && 
               !diag2[row + col];
    }
    
    // Calculate heuristic value (degree) for a position
    // Higher degree means this position constrains more future positions
    int calculateDegree(int row, int col) {
        if (!isSafe(row, col)) return -1;
        
        int degree = 0;
        
        // Count future positions this placement would block
        for (int r = row + 1; r < n; r++) {
            for (int c = 0; c < n; c++) {
                // Check if this future position would be attacked
                if (c == col ||                           // Same column
                    r - c == row - col ||                 // Same main diagonal
                    r + c == row + col) {                 // Same anti-diagonal
                    degree++;
                }
            }
        }
        
        return degree;
    }
    
    bool isValidPosition(int row, int col) {
        return row >= 0 && row < n && col >= 0 && col < n;
    }
    
    // Get valid positions for current row, ordered by heuristic
    // Most Constraining Variable (MCV) heuristic: choose position that eliminates most options
    vector<int> getOrderedValidPositions(int row) {
        vector<pair<int, int>> validCols; // (degree, column)
        
        for (int col = 0; col < n; col++) {
            if (isSafe(row, col)) {
                int degree = calculateDegree(row, col);
                validCols.push_back({degree, col});
            }
        }
        
        // Sort by degree in descending order (most constraining first)
        sort(validCols.begin(), validCols.end(), greater<pair<int, int>>());
        
        vector<int> result;
        for (const auto& p : validCols) {
            result.push_back(p.second);
        }
        return result;
    }
    
    // Count remaining valid positions from given row (for pruning)
    int countRemainingValidPositions(int startRow) {
        int count = 0;
        for (int r = startRow; r < n; r++) {
            for (int c = 0; c < n; c++) {
                if (isSafe(r, c)) {
                    count++;
                }
            }
        }
        return count;
    }
    
    // Place a queen and update constraint tracking
    void placeQueen(int row, int col) {
        board[row][col] = 1;
        cols[col] = true;
        diag1[row - col + n - 1] = true;
        diag2[row + col] = true;
    }
    
    // Remove a queen and update constraint tracking
    void removeQueen(int row, int col) {
        board[row][col] = 0;
        cols[col] = false;
        diag1[row - col + n - 1] = false;
        diag2[row + col] = false;
    }
    
    // Forward checking: ensure we have enough valid positions for remaining queens
    bool hasViableFuture(int row) {
        int remainingQueens = n - row;
        int validPositions = countRemainingValidPositions(row);
        return validPositions >= remainingQueens;
    }
    
    // Main backtracking algorithm with optimizations
    bool solveNQueens(int row) {
        nodes_explored++;
        
        // Base case: all queens placed successfully
        if (row == n) {
            return true;
        }
        
        // Pruning: Forward checking
        if (!hasViableFuture(row)) {
            return false;
        }
        
        // Get valid positions ordered by MCV heuristic
        vector<int> validCols = getOrderedValidPositions(row);
        
        // Early pruning: if no valid positions, backtrack
        if (validCols.empty()) {
            return false;
        }
        
        // Try each valid position
        for (int col : validCols) {
            placeQueen(row, col);
            
            if (solveNQueens(row + 1)) {
                return true;
            }
            
            removeQueen(row, col);
        }
        
        return false;
    }
    
    // Solve the N-Queens problem
    bool solve() {
        auto start = chrono::high_resolution_clock::now();
        
        bool hasSolution = solveNQueens(0);
        
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        
        if (hasSolution) {
            cout << "\n=== SOLUCIÓN ENCONTRADA PARA N = " << n << " ===" << endl;
            printSolution();
            cout << "\nEstadísticas de rendimiento:" << endl;
            cout << "- Nodos explorados: " << nodes_explored << endl;
            cout << "- Tiempo de ejecución: " << duration.count() << " microsegundos" << endl;
        } else {
            cout << "No existe solución para N = " << n << endl;
            cout << "Nodos explorados: " << nodes_explored << endl;
        }
        
        return hasSolution;
    }
    
    // Print the solution in a readable format
    void printSolution() {
        cout << "\nTablero (" << n << "x" << n << "):" << endl;
        cout << "  ";
        for (int j = 0; j < n; j++) {
            cout << setw(4) << j;
        }
        cout << endl;
        
        for (int i = 0; i < n; i++) {
            cout << i << " ";
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 1) {
                    cout << setw(4) << "Q";
                } else {
                    cout << setw(4) << ".";
                }
            }
            cout << endl;
        }
        
        cout << "\nPosiciones de las reinas (fila, columna):" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 1) {
                    cout << "Reina " << (i + 1) << ": (" << i << ", " << j << ")" << endl;
                }
            }
        }
        
        // Original format for compatibility
        cout << "\nFormato original:" << endl;
        for (int i = 0; i < n; i++) {
            cout << "{";
            for (int j = 0; j < n; j++) {
                if (j > 0) cout << ",";
                cout << setw(3) << board[i][j];
            }
            cout << "}" << endl;
        }
    }
    
    // Verify that the solution is correct
    bool verifySolution() {
        int queensCount = 0;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 1) {
                    queensCount++;
                    
                    // Check if this queen conflicts with any other
                    for (int k = 0; k < n; k++) {
                        for (int l = 0; l < n; l++) {
                            if (board[k][l] == 1 && (i != k || j != l)) {
                                // Check for conflicts
                                if (i == k || j == l ||                    // Same row/column
                                    abs(i - k) == abs(j - l)) {            // Same diagonal
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        return queensCount == n;
    }
};

int main() {
    cout << "=== SOLUCIONADOR DE N-REINAS OPTIMIZADO ===" << endl;
    cout << "Ingrese el valor de N (4 u 8): ";
    
    int n;
    cin >> n;
    
    // Validate input
    if (n != 4 && n != 8) {
        cout << "Error: N debe ser 4 u 8" << endl;
        cout << "Nota: El algoritmo funciona para cualquier N ≥ 4, pero por requisitos del problema solo acepta 4 u 8." << endl;
        return 1;
    }
    
    cout << "\nResolviendo el problema de " << n << "-Reinas..." << endl;
    cout << "Usando backtracking con:" << endl;
    cout << "- Heurística MCV (Most Constraining Variable)" << endl;
    cout << "- Forward Checking" << endl;
    cout << "- Seguimiento eficiente de restricciones" << endl;
    
    NQueens nqueens(n);
    bool solved = nqueens.solve();
    
    if (solved) {
        cout << "\n¿Verificar solución? (s/n): ";
        char verify;
        cin >> verify;
        if (verify == 's' || verify == 'S') {
            if (nqueens.verifySolution()) {
                cout << "✓ Solución verificada correctamente!" << endl;
            } else {
                cout << "✗ Error en la verificación de la solución." << endl;
            }
        }
    }
    
    return 0;
}