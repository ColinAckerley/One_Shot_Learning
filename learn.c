#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include "learnHeader.h"
int main(int argc, char **argv)
{
  int numRows = 0;
  int numCols = 0;
  FILE* learnFile = NULL;
  char* filename = NULL;
  filename = argv[1];
  learnFile = fopen(filename, "r");
  if(learnFile == NULL)
  {
    printf("error\n");
    exit(0);
  }
  fscanf(learnFile, "%d\n", &numCols); //Get the rows & columns in the input
  fscanf(learnFile, "%d\n", &numRows);
  numCols++;
  double **xMatrix = allocateMatrix(numRows, numCols);
  double **yMatrix = allocateMatrix(numRows, 1);
  for(int i = 0; i < numRows; i++) //Make the first column all 1s
  {
    xMatrix[i][0] = 1;
  }
  for(int i = 0; i < numRows; i++) //Create the X & Y matrices
  {
    for(int j = 1; j < numCols; j++)
    {
      fscanf(learnFile, "%lf,", &xMatrix[i][j]);
    }
    fscanf(learnFile, "%lf,", &yMatrix[i][0]);
    fscanf(learnFile, "\n", NULL);
  }
  fclose(learnFile);
  filename = argv[2];
  learnFile = fopen(filename, "r");
  if(learnFile == NULL)
  {
    printf("error\n");
    exit(0);
  }
  int numHouses = 0;
  fscanf(learnFile, "%d\n", &numHouses);
  double **testingXMatrix = allocateMatrix(numHouses, numCols);
  for(int i = 0; i < numHouses; i++) //Make the first column all 1s
  {
    testingXMatrix[i][0] = 1;
  }
  for(int i = 0; i < numHouses; i++) //Create the X Matrix used for testing
  {
    for(int j = 1; j < numCols; j++)
    {
      fscanf(learnFile, "%lf,", &testingXMatrix[i][j]);
    }
    fscanf(learnFile, "\n", NULL);
  }
  fclose(learnFile);
  double **finalMatrix = allocateMatrix(numHouses, 1);
  //Series of steps to get to the final answer
  double **xTranspose = transpose(xMatrix, numRows, numCols);
  double **toInvert = multiply(xTranspose, xMatrix, numCols, numRows, numRows, numCols);
  double **inverted = invert(toInvert, numCols);
  freeMatrixMem(toInvert, numCols);
  double **invertedTimesXTranspose = multiply(inverted, xTranspose, numCols,numCols,numCols, numRows);
  freeMatrixMem(inverted, numCols);
  freeMatrixMem(xTranspose, numCols);
  freeMatrixMem(xMatrix, numRows);
  double **weightMatrix = multiply(invertedTimesXTranspose, yMatrix, numCols, numRows, numRows, 1);
  freeMatrixMem(invertedTimesXTranspose, numCols);
  finalMatrix = multiply(testingXMatrix, weightMatrix,numHouses,numCols,numCols,1);
  freeMatrixMem(testingXMatrix, numHouses);
  freeMatrixMem(weightMatrix, numCols);
  printResults(finalMatrix,numHouses);
  freeMatrixMem(finalMatrix, numHouses);
  freeMatrixMem(yMatrix, numRows);
}
double **allocateMatrix(int rows, int cols)
{
    double **matrix = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (double *)malloc(cols * sizeof(double));
    }
    return matrix;
}
void freeMatrixMem(double **matrix, int rows)
{
    for (int i = 0; i < rows; i++)
    {
         free(matrix[i]);
    }
    free(matrix);
 }
double **invert(double **matrix, int size)
 {
    double **identityMatrix = createIdentityMatrix(size);
    double **modifiedMatrix = matrix;
    double ratio = 0;
    for(int curCol = 0; curCol < size; curCol++) //Bring to row echelon form
    {
      for(int curRow = curCol +1; curRow < size; curRow++)
      {
        if(fabs(modifiedMatrix[curRow][curCol]) > 10e-7)
        {
          int tmp = curRow - 1;
          while(fabs(modifiedMatrix[tmp][curCol]) < 10e-7)
          {
            tmp--;
          }
          ratio = modifiedMatrix[curRow][curCol]/modifiedMatrix[tmp][curCol];
          modifiedMatrix = subtractRow(modifiedMatrix,curRow, tmp, size, ratio);
          identityMatrix = subtractRow(identityMatrix,curRow, tmp, size, ratio);
        }
      }
    }
    for(int curRow = 0; curRow < size-1; curRow++) //Change into reduced row echelon form
    {
      int tmp = curRow+1;
      for(int curCol = curRow + 1; curCol < size; curCol++)
      {
        ratio = modifiedMatrix[curRow][curCol]/modifiedMatrix[tmp][curCol];
        modifiedMatrix = subtractRow(modifiedMatrix, curRow, tmp, size, ratio);
        identityMatrix = subtractRow(identityMatrix, curRow, tmp, size, ratio);
        tmp++;
      }
    }
    for(int curRow = 0; curRow < size; curRow++)
    {
      for(int curCol = 0; curCol < size; curCol++)
      {
        if(curRow == curCol)
        {
          if(modifiedMatrix[curRow][curCol] != 1)
          {
            ratio = 1/(modifiedMatrix[curRow][curCol]);
            for(int i = 0; i < size; i++)
            {
              modifiedMatrix[curRow][i] = ratio * (modifiedMatrix[curRow][i]);
              identityMatrix[curRow][i] = ratio * (identityMatrix[curRow][i]);
            }
          }
        }
      }
    }
    return identityMatrix;
  }
  double **subtractRow(double **matrix, int row, int subtractor, int size, double multiplier)
  {
    double **modifiedMatrix = allocateMatrix(size, size);
    modifiedMatrix = matrix;
    for(int curCol = 0; curCol < size; curCol++)
    {
      modifiedMatrix[row][curCol] = matrix[row][curCol] - (multiplier * (matrix[subtractor][curCol]));
    }
    return modifiedMatrix;
  }
 double **multiply(double **matrixA, double **matrixB, int rowsA, int rowsB, int colsA, int colsB)
{
    double **newMatrix = allocateMatrix(rowsA, colsB);
    for(int i = 0; i < rowsA; i++)
    {
		for(int j = 0; j < colsB; j++)
		{
      newMatrix[i][j] = 0;
			for(int k=0; k<colsA; k++)
			{
				newMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
	}
    return newMatrix;
}
double **createIdentityMatrix(int size)
{
  double **identityMatrix = allocateMatrix(size, size);
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j <size; j++)
    {
      if(i == j)
        identityMatrix[i][j] = 1;
      else
        identityMatrix[i][j] = 0;
    }
  }
  return identityMatrix;
}
double **transpose(double **matrix, int rows, int cols)
{
  double **newMatrix = allocateMatrix(cols, rows);
  for(int i =0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      newMatrix[j][i] = matrix[i][j];
    }
  }
  return newMatrix;
}
void printResults(double** finalPriceMatrix, int numRows)
{
  for(int i = 0; i < numRows; i++)
  {
    printf("%0.0lf\n", finalPriceMatrix[i][0]);
  }
}
