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
    
    // Función de heurística para poda pesada
    // Cuenta el número de posiciones seguras disponibles para cada fila
    int countSafePositions(int row) {
        int count = 0;
        for (int col = 0; col < n; col++) {
            if (isSafe(row, col)) {
                count++;
            }
        }
        return count;
    }
    
    // Verifica si es seguro colocar una reina en (row, col)
    bool isSafe(int row, int col) {
        return !cols[col] && !diag1[row - col + n - 1] && !diag2[row + col];
    }
    
    // Coloca una reina en (row, col)
    void placeQueen(int row, int col) {
        board[row][col] = 1;
        cols[col] = true;
        diag1[row - col + n - 1] = true;
        diag2[row + col] = true;
    }
    
    // Remueve una reina de (row, col)
    void removeQueen(int row, int col) {
        board[row][col] = 0;
        cols[col] = false;
        diag1[row - col + n - 1] = false;
        diag2[row + col] = false;
    }
    
    // Función principal de backtracking con poda pesada
    bool solveNQueens(int row) {
        // Caso base: todas las reinas han sido colocadas
        if (row == n) {
            return true;
        }
        
        // Poda pesada: crear vector de columnas ordenadas por seguridad
        vector<pair<int, int>> columns;
        for (int col = 0; col < n; col++) {
            if (isSafe(row, col)) {
                // Simular colocación temporal para calcular heurística
                placeQueen(row, col);
                int safeCount = 0;
                
                // Contar posiciones seguras en filas restantes
                for (int futureRow = row + 1; futureRow < n; futureRow++) {
                    safeCount += countSafePositions(futureRow);
                }
                
                removeQueen(row, col);
                columns.push_back({safeCount, col});
            }
        }
        
        // Poda: si no hay columnas seguras, retroceder
        if (columns.empty()) {
            return false;
        }
        
        // Ordenar por número de posiciones seguras (descendente)
        // Esto implementa la "poda pesada" - explorar primero las opciones más prometedoras
        sort(columns.begin(), columns.end(), greater<pair<int, int>>());
        
        // Probar cada columna en orden de mayor a menor seguridad
        for (auto& colPair : columns) {
            int col = colPair.second;
            
            // Colocar reina
            placeQueen(row, col);
            
            // Recursión
            if (solveNQueens(row + 1)) {
                return true;
            }
            
            // Backtracking: remover reina
            removeQueen(row, col);
        }
        
        return false;
    }
    
public:
    NQueens(int size) : n(size) {
        board = vector<vector<int>>(n, vector<int>(n, 0));
        cols = vector<bool>(n, false);
        diag1 = vector<bool>(2 * n - 1, false);
        diag2 = vector<bool>(2 * n - 1, false);
    }
    
    bool solve() {
        return solveNQueens(0);
    }
    
    void printSolution() {
        for (int i = 0; i < n; i++) {
            cout << "{";
            for (int j = 0; j < n; j++) {
                cout << setw(2) << board[i][j];
                if (j < n - 1) cout << ", ";
            }
            cout << "}" << endl;
        }
    }
};

int main() {
    int n;
    cin >> n;
    
    // Validar entrada
    if (n != 4 && n != 8) {
        cout << "Error: N debe ser 4 u 8" << endl;
        return 1;
    }
    
    NQueens queens(n);
    
    if (queens.solve()) {
        queens.printSolution();
    } else {
        cout << "No se encontró solución para N = " << n << endl;
    }
    
    return 0;
}