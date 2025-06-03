#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

class NQueens {
private:
    int n;
    vector<vector<int>> board;
    vector<bool> cols;          // Columnas ocupadas
    vector<bool> diag1;         // Diagonales principales (i-j+n-1)
    vector<bool> diag2;         // Diagonales secundarias (i+j)
    
public:
    NQueens(int size) : n(size) {
        board.assign(n, vector<int>(n, 0));
        cols.assign(n, false);
        diag1.assign(2*n-1, false);
        diag2.assign(2*n-1, false);
    }
    
    // Verifica si es seguro colocar una reina en (row, col)
    bool isSafe(int row, int col) {
        return !cols[col] && !diag1[row - col + n - 1] && !diag2[row + col];
    }
    
    // Calcula el grado de una posición específica (cuántas posiciones ataca)
    int calculateDegrees(int row, int col) {
        int degree = 0;
        
        // Contar en fila y columna
        degree += (n - 1) * 2;
        
        // Contar en diagonales
        degree += countDiagonalPositions(row, col, 1, 1);   // Diagonal principal
        degree += countDiagonalPositions(row, col, 1, -1);  // Diagonal secundaria
        
        return degree;
    }
    
    // Cuenta posiciones en una dirección diagonal específica
    int countDiagonalPositions(int row, int col, int deltaRow, int deltaCol) {
        int count = 0;
        
        // Contar hacia adelante
        count += countInDirection(row, col, deltaRow, deltaCol);
        
        // Contar hacia atrás
        count += countInDirection(row, col, -deltaRow, -deltaCol);
        
        return count;
    }
    
    // Cuenta posiciones válidas en una dirección específica
    int countInDirection(int row, int col, int deltaRow, int deltaCol) {
        int count = 0;
        int r = row + deltaRow;
        int c = col + deltaCol;
        
        while (isValidPosition(r, c)) {
            count++;
            r += deltaRow;
            c += deltaCol;
        }
        
        return count;
    }
    
    // Verifica si una posición está dentro del tablero
    bool isValidPosition(int row, int col) {
        return row >= 0 && row < n && col >= 0 && col < n;
    }
    
    // Obtiene las posiciones válidas para una fila ordenadas por grado
    vector<int> getValidPositions(int row) {
        vector<pair<int, int>> validCols;
        
        for (int col = 0; col < n; col++) {
            if (isSafe(row, col)) {
                int degree = calculateDegrees(row, col);
                validCols.push_back({degree, col});
            }
        }
        
        // Ordenar por grado descendente (poda pesada)
        sort(validCols.begin(), validCols.end(), greater<pair<int,int>>());
        
        return extractColumns(validCols);
    }
    
    // Extrae solo las columnas del vector de pares
    vector<int> extractColumns(const vector<pair<int, int>>& validCols) {
        vector<int> result;
        for (const auto& p : validCols) {
            result.push_back(p.second);
        }
        return result;
    }
    
    // Cuenta posiciones válidas restantes
    int countValidPositions(int startRow) {
        int count = 0;
        for (int i = startRow; i < n; i++) {
            count += countValidInRow(i);
        }
        return count;
    }
    
    // Cuenta posiciones válidas en una fila específica
    int countValidInRow(int row) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (isSafe(row, j)) {
                count++;
            }
        }
        return count;
    }
    
    // Coloca una reina en el tablero
    void placeQueen(int row, int col) {
        board[row][col] = 1;
        cols[col] = true;
        diag1[row - col + n - 1] = true;
        diag2[row + col] = true;
    }
    
    // Quita una reina del tablero
    void removeQueen(int row, int col) {
        board[row][col] = 0;
        cols[col] = false;
        diag1[row - col + n - 1] = false;
        diag2[row + col] = false;
    }
    
    // Verifica si hay suficientes posiciones para continuar
    bool hasSufficientPositions(int row) {
        return countValidPositions(row) >= (n - row);
    }
    
    // Función principal de backtracking
    bool solveNQueens(int row) {
        if (row == n) {
            return true; // Todas las reinas colocadas
        }
        
        if (!hasSufficientPositions(row)) {
            return false; // Poda: no hay suficientes posiciones
        }
        
        vector<int> validCols = getValidPositions(row);
        
        return tryAllPositions(row, validCols);
    }
    
    // Intenta todas las posiciones válidas en una fila
    bool tryAllPositions(int row, const vector<int>& validCols) {
        for (int col : validCols) {
            placeQueen(row, col);
            
            if (solveNQueens(row + 1)) {
                return true;
            }
            
            removeQueen(row, col);
        }
        return false;
    }
    
    // Resuelve el problema y muestra la solución
    bool solve() {
        if (solveNQueens(0)) {
            printSolution();
            return true;
        }
        
        printNoSolution();
        return false;
    }
    
    // Imprime mensaje cuando no hay solución
    void printNoSolution() {
        cout << "No existe solución para N = " << n << endl;
    }
    
    // Imprime la solución en el formato requerido
    void printSolution() {
        for (int i = 0; i < n; i++) {
            printRow(i);
        }
    }
    
    // Imprime una fila de la solución
    void printRow(int row) {
        cout << "{";
        for (int j = 0; j < n; j++) {
            if (j > 0) cout << ",";
            cout << setw(3) << board[row][j];
        }
        cout << "}" << endl;
    }
};

// Valida la entrada del usuario
bool isValidInput(int n) {
    return n == 4 || n == 8;
}

// Función principal
int main() {
    int n;
    cin >> n;
    
    if (!isValidInput(n)) {
        cout << "Error: N debe ser 4 u 8" << endl;
        return 1;
    }
    
    NQueens nqueens(n);
    nqueens.solve();
    
    return 0;
}