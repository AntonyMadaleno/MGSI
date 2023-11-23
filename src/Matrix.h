/**
 * @file Matrix.h
 * @brief Header for struct Matrix
 * @author Antony Madaleno
 * @version 1.0
 * @date 12-01-2023
 *
 * Header pour les struct de Matrices
 *
 */

#pragma once
#include <stdbool.h>

#define MATRIX_IMPLEMENTATION

/**
 * @struct Matrix
 * @brief Matrix contain its number of rows and colums and a pointeur to its data (floats)
 */
typedef struct Matrix
{
    unsigned short n_cols, n_rows;
    float * data;
} Matrix;

/// @fn Matrix * Matrix_generate(unsigned short n_cols, unsigned short n_rows );
/// @brief generate a matrix containings zeros.
/// @param n_cols unsigned short, number of columns in the matrix
/// @param n_rows unsigned short, number of rows in the matrix
/// @return a pointer to a matrix of size (n_cols, n_rows) containing zeros
Matrix * Matrix_generate(unsigned short n_cols, unsigned short n_rows );

/// @fn float * Matrix_at(Matrix * mat, unsigned short i, unsigned short j );
/// @brief return pointer to value at position mat[i][j]
/// @param mat a pointer to a matrix
/// @param i column position
/// @param j row position
/// @return pointer to value at given position in matrix
float * Matrix_at(Matrix * mat, unsigned short i, unsigned short j );

/// @fn unsigned char Matrix_setAt(Matrix * mat, unsigned short i, unsigned short j, float x);
/// @brief set value at position mat[i][j] to value x
/// @param mat pointer to Matrix
/// @param i column position
/// @param j row position
/// @param x float value
/// @return 0 if problem occured, 1 otherwise
unsigned char Matrix_setAt(Matrix * mat, unsigned short i, unsigned short j, float x);

/// @fn void Matrix_scalarMultiply(Matrix * mat, float x);
/// @brief multiply every values in mat by x
/// @param mat pointer to Matrix
/// @param x float value
void Matrix_scalarMultiply(Matrix * mat, float x);

/// @fn Matrix * Matrix_multiply(Matrix * m1, Matrix * m2);
/// @brief multiply 2 Matrix m1 * m2
/// @param m1 pointer to Matrix
/// @param m2 pointer to Matrix
/// @return pointer to resultant Matrix
Matrix * Matrix_multiply(Matrix * m1, Matrix * m2);

/// @fn Matrix * Matrix_addition(Matrix * m1, Matrix * m2);
/// @brief add two matrix together
/// @param m1 pointer to Matrix
/// @param m2 pointer to Matrix
/// @return pointer to result matrix
Matrix * Matrix_addition(Matrix * m1, Matrix * m2);

/// @fn Matrix * Matrix_substract(Matrix * m1, Matrix * m2);
/// @brief substract m2 from m1
/// @param m1 pointer to Matrix
/// @param m2 pointer to Matrix
/// @return pointer to result matrix
Matrix * Matrix_substract(Matrix * m1, Matrix * m2);

/// @fn bool Matrix_equals(Matrix * m1, Matrix * m2);
/// @brief return if two given Matrix are equals
/// @param m1 
/// @param m2 
/// @return true if equals else false
bool Matrix_equals(Matrix * m1, Matrix * m2);

/// @fn Matrix * Matrix_copy(Matrix * mat);
/// @brief copy Matrix and return pointer to copy
/// @param mat pointer to matrix
/// @return pointer to Matrix copy of mat
Matrix * Matrix_copy(Matrix * mat);

/// @fn Matrix * Matrix_copy(Matrix * mat);
/// @brief calculate determinant of matrix
/// @param mat pointer to matrix
/// @return determinant of matrix
float Matrix_determinant(Matrix * mat);

/// @fn void Matrix_free(Matrix * m);
/// @brief free memory used by Matrix
/// @param mat pointer to matrix
void Matrix_free(Matrix * m);

/// @fn void Matrix_orderRows(Matrix * m);
/// @brief order Matrix given its rows
/// @param m pointer to Matrix
void Matrix_orderRows(Matrix * m);

/// @fn Matrix * Matrix_getTransposed(Matrix * m);
/// @brief calculate and return Transposed Matrix
/// @param m pointer to Matrix
/// @return pointer to Transposed Matrix
Matrix * Matrix_getTransposed(Matrix * m);

/// @fn void Matrix_print(Matrix * m);
/// @brief print information of matrix in console
/// @param m pointer to Matrix
void Matrix_print(Matrix * m);

/// @brief 
/// @param m 
/// @return 
Matrix ** Matrix_fft(Matrix * m);

/// @brief 
/// @param m 
/// @return 
Matrix * Matrix_ifft(Matrix ** F);

/// @brief 
/// @param m 
/// @return minimum value in matrix
float Matrix_min(Matrix * m);

/// @brief 
/// @param m 
/// @return maximum value in matrix
float Matrix_max(Matrix * m);

