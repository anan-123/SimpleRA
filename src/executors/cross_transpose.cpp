#include "global.h"
/**
 * @brief 
 * SYNTAX: CROSS_TRANSPOSE matrix1 matrix2
 */
bool syntacticParseCROSS_TRANSPOSE()
{
    logger.log("syntacticParseCROSS_TRANSPOSE");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = CROSS_TRANSPOSE;
    parsedQuery.crossTransposeFirstMatrix = tokenizedQuery[1];
    parsedQuery.crossTransposeSecondMatrix = tokenizedQuery[2];
    return true;
}

bool semanticParseCROSS_TRANSPOSE()
{
    logger.log("semanticParseCROSS_TRANSPOSE");
    if (!matrixCatalogue.isMatrix(parsedQuery.crossTransposeFirstMatrix))
    {
        cout << "SEMANTIC ERROR: Matrix1 does not exist" << endl;
        return false;
    }
    if (!matrixCatalogue.isMatrix(parsedQuery.crossTransposeSecondMatrix))
    {
        cout << "SEMANTIC ERROR: Matrix2 does not exist" << endl;
        return false;
    }
    return true;
}

void transpose(vector<vector<int>>& submatrix){
    for(int i=0; i<MATRIX_DIM; i++){
        for(int j=i+1; j<MATRIX_DIM; j++){
            swap(submatrix[i][j], submatrix[j][i]);
        }
    }
}

void executeCROSS_TRANSPOSE()
{
    logger.log("executeCROSS_TRANSPOSE");

    Matrix *matrix1 = matrixCatalogue.getMatrix(parsedQuery.crossTransposeFirstMatrix);
    Matrix *matrix2 = matrixCatalogue.getMatrix(parsedQuery.crossTransposeSecondMatrix);

    int blockCount = matrix1->blockCount;

    for (int block_i = 0; block_i < blockCount; block_i++) {
        
        for (int block_j = 0; block_j < blockCount; block_j++) {

            CursorMatrix cursormatrix1(matrix1->matrixName, block_i,block_j,block_i,block_j);
            PageMatrix page1 = cursormatrix1.getMatrixPage();
            vector<vector<int>> submatrix1 = page1.getSubmatrix();
            transpose(submatrix1);

            CursorMatrix cursormatrix2(matrix2->matrixName, block_j,block_i,block_j,block_i);
            PageMatrix page2 = cursormatrix2.getMatrixPage();
            vector<vector<int>> submatrix2 = page2.getSubmatrix();
            transpose(submatrix2);

            // for(int i=0; i<MATRIX_DIM; i++){
            //     for(int j=0; j<MATRIX_DIM; j++){
            //         cout<<submatrix1[i][j]<<" ";
            //     }
            //     cout<<endl;
            // }
            

            bufferManager.writeMatrixPage(matrix1->matrixName, block_i, block_j, block_i, block_j, submatrix2);
            bufferManager.writeMatrixPage(matrix2->matrixName, block_j, block_i, block_j, block_i, submatrix1);
        }
    }
    cout<<" CROSS TRANSPOSED Matrices "<<matrix1->matrixName<<" and "<<matrix2->matrixName<<endl;

    return;
}