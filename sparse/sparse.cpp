#include <iostream>
using namespace std;

int main(){
	int rows = 13;
	int** matrix = new int*[rows];
	for (int i = 0; i < rows; ++i)
		matrix[i] = new int[rows];

	//assign values to matrix
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < rows; j++){
			matrix[i][j] = 0;
		}
	}
	matrix[0][1] = 1;
	matrix[1][0] = 1;
	matrix[0][2] = 1;
	matrix[2][0] = 1;
	matrix[1][3] = 1;
	matrix[3][1] = 1;
	matrix[2][5] = 1;
	matrix[5][2] = 1;
	matrix[3][4] = 1;
	matrix[4][3] = 1;
	matrix[3][6] = 1;
	matrix[6][3] = 1;
	matrix[5][6] = 1;
	matrix[6][5] = 1;
	matrix[5][8] = 1;
	matrix[8][5] = 1;
	matrix[6][9] = 1;
	matrix[9][6] = 1;
	matrix[7][4] = 1;
	matrix[4][7] = 1;
	matrix[7][10] = 1;
	matrix[10][7] = 1;
	matrix[8][5] = 1;
	matrix[5][8] = 1;
	matrix[8][11] = 1;
	matrix[11][8] = 1;
	matrix[9][10] = 1;
	matrix[10][9] = 1;
	matrix[9][12] = 1;
	matrix[12][9] = 1;
	matrix[11][12] = 1;
	matrix[12][11] = 1;

	int aux;
	for (int i = 0; i < rows; i++){
		aux = 0;
		for (int j = 0; j < rows; j++){
			if(i != j){
				aux += matrix[i][j];
			}
		}
		matrix[i][i] = -aux;
	}





	//print values in matrix
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < rows; j++){
			cout << matrix[i][j] << ' ';
		}
		cout << '\n';
	}



	for (int i = 0; i < rows; ++i)
		delete [] matrix[i];
	delete [] matrix;

}



/*
0 1 1 0 0 0 0 0 0 0 0 0 0 
1 0 0 1 0 0 0 0 0 0 0 0 0 
1 0 0 0 0 1 0 0 0 0 0 0 0 
0 1 0 0 1 0 1 0 0 0 0 0 0 
0 0 0 1 0 0 0 1 0 0 0 0 0 
0 0 1 0 0 0 1 0 1 0 0 0 0 
0 0 0 1 0 1 0 0 0 1 0 0 0 
0 0 0 0 1 0 0 0 0 0 1 0 0 	* x = 
0 0 0 0 0 1 0 0 0 0 0 1 0 
0 0 0 0 0 0 1 0 0 0 1 0 1 
0 0 0 0 0 0 0 1 0 1 0 0 0 
0 0 0 0 0 0 0 0 1 0 0 0 1 
0 0 0 0 0 0 0 0 0 1 0 1 0


\sum L * D *(pi - pj) = 0



*/