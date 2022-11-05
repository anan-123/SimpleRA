#include "matrix.h"

/**
 * @brief The MatrixCatalogue acts like an index of Matrixs existing in the
 * system. Everytime a Matrix is added(removed) to(from) the system, it needs to
 * be added(removed) to(from) the MatrixCatalogue. 
 *
 */
class MatrixCatalogue
{

    unordered_map<string, Matrix*> matrixs;

public:
    MatrixCatalogue() {}
    void insertMatrix(Matrix* Matrix);
    void deleteMatrix(string MatrixName);
    Matrix* getMatrix(string MatrixName);
    bool isMatrix(string MatrixName);
    void print();
    //~MatrixCatalogue();
};
