/**
 * @file Matrix.c
 * @brief Implement Matrix.h
 * @author Antony Madaleno
 * @version 1.0
 * @date 12-01-2023
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "math.h"
#include "Matrix.h"

Matrix * Matrix_generate(unsigned short n_cols, unsigned short n_rows )
{
    if (n_cols == 0 || n_rows == 0)
        return NULL;
    
    Matrix * mat = (Matrix *) calloc( 1, sizeof(Matrix) );
    mat->n_cols = n_cols;
    mat->n_rows = n_rows;

    //ALLOCATION OF MEMORY
    mat->data = (float *) calloc( (size_t) n_cols * n_rows, sizeof(float) );
    return mat;
}

float * Matrix_at(Matrix * mat, unsigned short i, unsigned short j )
{
    return &(mat->data[i + j * mat->n_cols]);
}

unsigned char Matrix_setAt(Matrix * mat, unsigned short i, unsigned short j, float x)
{
    if (mat == NULL || i >= mat->n_cols || j >= mat->n_rows )
        return 0;

    * Matrix_at(mat, i, j) = x;
    return 1;
}

void Matrix_scalarMultiply(Matrix * mat, float x)
{
    if (mat == NULL)
        return;

    for (unsigned short i = 0; i < mat->n_cols; i++)
        for (unsigned short j = 0; j < mat->n_rows; j++)
            * Matrix_at(mat, i, j) *= x;
}

Matrix * Matrix_multiply(Matrix * m1, Matrix * m2)
{
    if (m1 == NULL || m2 == NULL || m1->n_cols != m2->n_rows)
        return NULL;

    Matrix * res = Matrix_generate(m2->n_cols, m1->n_rows);

    //for each row of m1
    for (unsigned short i = 0; i < m1->n_rows; i++)
    {   
        //for each column of m2
        for (unsigned short j = 0; j < m2->n_cols; j++)
        {
            Matrix_setAt(res, j, i, 0);
            //for each element of m1 row and m2 column
            for (unsigned short k = 0; k < m1->n_cols; k++)
                * Matrix_at(res, j, i) += ( * Matrix_at(m1, k, i) ) *  ( * Matrix_at(m2, j, k) );
        }
    }

    return res;
}

Matrix * Matrix_addition(Matrix * m1, Matrix * m2)
{
    if (m1 == NULL || m2 == NULL || m1->n_cols != m2->n_cols || m1->n_rows != m2->n_rows)
        return NULL;

    Matrix * result = Matrix_generate(m1->n_cols, m1->n_rows);

    for (unsigned short i = 0; i < m1->n_cols; i++)
        for (unsigned short j = 0; j < m1->n_rows; j++)
            * Matrix_at(result, i, j) = ( * Matrix_at(m1, i, j) ) + ( * Matrix_at(m2, i, j) );

    return result;
}

Matrix * Matrix_substract(Matrix * m1, Matrix * m2)
{
    if (m1 == NULL || m2 == NULL || m1->n_cols != m2->n_cols || m1->n_rows != m2->n_rows)
        return NULL;

    Matrix * result = Matrix_generate(m1->n_cols, m1->n_rows);

    for (unsigned short i = 0; i < m1->n_cols; i++)
        for (unsigned short j = 0; j < m1->n_rows; j++)
            * Matrix_at(result, i, j) = ( * Matrix_at(m1, i, j) ) - ( * Matrix_at(m2, i, j) );

    return result;
}

bool Matrix_equals(Matrix * m1, Matrix * m2)
{
    if (m1 == m2)
        return true;
    else if (m1 == NULL || m2 == NULL || m1->n_rows != m2->n_rows || m1->n_cols != m2->n_cols)
        return false;

    for (unsigned short i = 0; i < m1->n_cols; i++)
        for (unsigned short j = 0; j < m1->n_rows; j++)
            if ( * Matrix_at(m1, i, j) != * Matrix_at(m2, i, j) )
                return false;
    
    return true;
}

Matrix * Matrix_copy(Matrix * mat)
{
    if (mat == NULL)
        return NULL;

    Matrix * cpy = Matrix_generate(mat->n_cols, mat->n_rows);

    for (unsigned int i = 0; i < mat->n_cols * mat->n_rows; i++)
        cpy->data[i] = mat->data[i];

    return cpy;
}

float Matrix_determinant(Matrix * mat)
{
    if (mat != NULL && mat->n_cols != mat->n_rows)
        exit(-1);
    else if (mat->n_cols == 1)
        return * Matrix_at(mat, 0, 0);
    else if (mat->n_cols == 2)
        return ( * Matrix_at(mat, 0, 0) ) * ( * Matrix_at(mat, 1, 1) ) - ( * Matrix_at(mat, 1, 0) ) * ( * Matrix_at(mat, 0, 1) );


    float result = 0;

    //for each column of matrix
    for (unsigned short i = 0; i < mat->n_cols; i++)
    {
        Matrix * tmp = Matrix_generate(mat->n_cols - 1, mat->n_rows - 1);
        //for each row under the first one
        for (unsigned short j = 1; j < mat->n_rows; j++)
        {
            //for each colum of matrix
            for (unsigned short k = 0; k < mat->n_cols; k++)
            {
                if (k < i)
                    Matrix_setAt( tmp, k, j-1, * Matrix_at(mat, k, j) );
                else if (k > i)
                    Matrix_setAt( tmp, k-1, j-1, * Matrix_at(mat, k, j) );
            }
        }

        if (i % 2 == 0)
            result += ( * Matrix_at(mat, i, 0) ) * Matrix_determinant(tmp);
        else   
            result -= ( * Matrix_at(mat, i, 0) ) * Matrix_determinant(tmp);

        Matrix_free(tmp);
    }

    return result;
}

void Matrix_orderRows(Matrix * m)
{
    unsigned short * indices = (unsigned short *) calloc( m->n_rows, sizeof(unsigned short) );
    int * leads = (int *) calloc( m->n_rows, sizeof( int ) );

    //FOR EACH ROW SEARCH FIRST NON-ZEROS VALUE POSITION
    for (unsigned short j = 0; j < m->n_rows; j++)
    {
        for (unsigned short i = 0; i < m->n_cols; i++)
        {
            if ( * Matrix_at(m, i, j) != 0 )
            {
                leads[j] = i;
                i = m->n_cols;
            }
        }
    }

    for (unsigned short i = 0; i < m->n_rows; i++)
    {
        int smallest_found = -1;
        int position_found = -1;
        //FOR EACH ROW WE COMPARE POSITION OF FIRST NON-ZERO
        for (unsigned short a = 0; a < m->n_rows; a++)
        {
            if ( smallest_found == -1 || smallest_found > leads[a] && leads[a] >= 0 )
            {
                smallest_found = leads[a];
                position_found = a;
            }
        }
        indices[i] = position_found;
        leads[position_found] = -1;
    }

    Matrix * cpy = Matrix_copy(m);

    for (unsigned short row = 0; row < m->n_rows; row++)
    {
        for (unsigned short col = 0; col < m->n_cols; col++)
        {
            Matrix_setAt(m, col, row, * Matrix_at(cpy, col, indices[row]) );
        }
    }

    free(indices);
    free(leads);
    Matrix_free(cpy);
}

void Matrix_free(Matrix * m)
{
    free(m->data);
    free(m);
    return;
}

Matrix * Matrix_getTransposed(Matrix * m)
{
    Matrix * trans = Matrix_generate(m->n_rows, m->n_cols);

    for (unsigned short i = 0; i < m->n_cols; i++)
        for (unsigned short j = 0; j < m->n_rows; j++)
            Matrix_setAt(trans, j, i, * Matrix_at( m, i, j ) );

    return trans;
}

void Matrix_print(Matrix * m)
{
    for (unsigned short j = 0; j < m->n_rows; j++)
    {
        for (unsigned short i = 0; i < m->n_cols; i++)
            printf("%3.2f\t", * Matrix_at(m, i, j) );

        printf("\n\n");
    }
}

float Matrix_min(Matrix * m)
{
    float min = * Matrix_at(m, 0,0);

    for (unsigned short j = 0; j < m->n_rows; j++)
        for (unsigned short i = 0; i < m->n_cols; i++)
            if (min > * Matrix_at(m, i, j))
                min = * Matrix_at(m, i, j);

    return min;
}

float Matrix_max(Matrix * m)
{
    float max = * Matrix_at(m, 0,0);

    for (unsigned short j = 0; j < m->n_rows; j++)
        for (unsigned short i = 0; i < m->n_cols; i++)
            if (max < * Matrix_at(m, i, j))
                max = * Matrix_at(m, i, j);

    return max;
}