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
    vector<int> degrees;        // Grado de cada posición (para poda pesada)
    
public:
    NQueens(int size) : n(size) {
        board.assign(n, vector<int>(n, 0));
        cols.assign(n, false);
        diag1.assign(2*n-1, false);
        diag2.assign(2*n-1, false);
        calculateDegrees();
    }
    
    // Calcula el grado de cada posición (número de casillas que ataca)
    void calculateDegrees() {
        degrees.assign(n*n, 0);
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                int pos = i * n + j;
                
                // Contar posiciones en la misma fila
                degrees[pos] += n - 1;
                
                // Contar posiciones en la misma columna
                degrees[pos] += n - 1;
                
                // Contar posiciones en diagonal principal
                int diag_count1 = 0;
                for(int k = 0; k < n; k++) {
                    for(int l = 0; l < n; l++) {
                        if(k != i && l != j && (k - l) == (i - j)) {
                            diag_count1++;
                        }
                    }
                }
                degrees[pos] += diag_count1;
                
                // Contar posiciones en diagonal secundaria
                int diag_count2 = 0;
                for(int k = 0; k < n; k++) {
                    for(int l = 0; l < n; l++) {
                        if(k != i && l != j && (k + l) == (i + j)) {
                            diag_count2++;
                        }
                    }
                }
                degrees[pos] += diag_count2;
            }
        }
    }
    
    // Verifica si es seguro colocar una reina en (row, col)
    bool isSafe(int row, int col) {
        return !cols[col] && !diag1[row - col + n - 1] && !diag2[row + col];
    }
    
    // Cuenta el número de posiciones válidas restantes para las filas siguientes
    int countValidPositions(int row) {
        int count = 0;
        for(int i = row; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(isSafe(i, j)) {
                    count++;
                }
            }
        }
        return count;
    }
    
    // Obtiene las posiciones válidas para una fila ordenadas por grado (poda pesada)
    vector<int> getValidPositions(int row) {
        vector<pair<int, int>> validCols; // {grado, columna}
        
        for(int col = 0; col < n; col++) {
            if(isSafe(row, col)) {
                int pos = row * n + col;
                validCols.push_back({degrees[pos], col});
            }
        }
        
        // Ordenar por grado descendente (poda pesada - elegir posiciones más restrictivas primero)
        sort(validCols.begin(), validCols.end(), greater<pair<int,int>>());
        
        vector<int> result;
        for(auto& p : validCols) {
            result.push_back(p.second);
        }
        
        return result;
    }
    
    // Función principal de backtracking
    bool solveNQueens(int row) {
        // Caso base: todas las reinas están colocadas
        if(row == n) {
            return true;
        }
        
        // Poda: verificar si quedan suficientes posiciones válidas
        if(countValidPositions(row) < (n - row)) {
            return false;
        }
        
        // Obtener posiciones válidas ordenadas por grado (poda pesada)
        vector<int> validCols = getValidPositions(row);
        
        for(int col : validCols) {
            // Colocar reina
            board[row][col] = 1;
            cols[col] = true;
            diag1[row - col + n - 1] = true;
            diag2[row + col] = true;
            
            // Recursión
            if(solveNQueens(row + 1)) {
                return true;
            }
            
            // Backtrack: quitar reina
            board[row][col] = 0;
            cols[col] = false;
            diag1[row - col + n - 1] = false;
            diag2[row + col] = false;
        }
        
        return false;
    }
    
    // Resuelve el problema y muestra la solución
    bool solve() {
        if(solveNQueens(0)) {
            printSolution();
            return true;
        } else {
            cout << "No existe solución para N = " << n << endl;
            return false;
        }
    }
    
    // Imprime la solución en el formato requerido
    void printSolution() {
        for(int i = 0; i < n; i++) {
            cout << "{";
            for(int j = 0; j < n; j++) {
                if(j > 0) cout << ",";
                cout << setw(3) << board[i][j];
            }
            cout << "}" << endl;
        }
    }
};

int main() {
    int n;
    cin >> n;
    
    // Validar entrada
    if(n != 4 && n != 8) {
        cout << "Error: N debe ser 4 u 8" << endl;
        return 1;
    }
    
    NQueens nqueens(n);
    nqueens.solve();
    
    return 0;
}