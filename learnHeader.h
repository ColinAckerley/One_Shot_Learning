#ifndef _learnHeader_h
#define _learnHeader_h
double **allocateMatrix(int, int);
void freeMatrixMem(double**, int);
double **invert(double**, int);
double **multiply(double**, double**, int, int, int, int);
double **transpose(double**, int, int);
void printResults(double**, int);
double **createIdentityMatrix(int);
double **subtractRow(double**, int, int, int, double);
#endif
