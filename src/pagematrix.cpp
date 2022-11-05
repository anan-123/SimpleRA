#include "global.h"
/**
 * @brief Construct a new PageMatrix object. Never used as part of the code
 *
 */

PageMatrix::PageMatrix()
{
    this->pageName = "";
    this->matrixName = "";
    this->col_start = -1;
    this->row_start = -1;
    this->col_end = -1;
    this->row_end = -1;
    this->submatrix.clear();
}

/**
 * @brief Construct a new PageMatrix:: PageMatrix object given the matrix name and page
 * index. When matrices are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<matrixname>_Page<pageindex>". For example, If the PageMatrix being loaded is of
 * matrix "R" and the pageIndex is 2 then the file name is "R_Page2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param matrixName 
 * @param pageIndex 
 */
PageMatrix::PageMatrix(string matrixName, int row_start, int col_start, int row_end, int col_end)
{
    logger.log("PageMatrix::PageMatrix");
    this->matrixName = matrixName;
    this->row_start = row_start;
    this->col_start = col_start;
    this->row_end = row_end;
    this->col_end = col_end;
    this->pageName = "../data/temp/" + this->matrixName + "_Page_" + to_string(row_start) + "_" + to_string(col_start) + "_" + to_string(row_end) + "_" + to_string(col_end);
    logger.log("PageMatrix::Check1");

    this->submatrix.resize(MATRIX_DIM);
    fill(this->submatrix.begin(), this->submatrix.end(), vector<int> (MATRIX_DIM, -1));

    ifstream fin(this->pageName, ios::in);
    for (int i = 0; i < MATRIX_DIM; i++) {
        for (int j = 0; j < MATRIX_DIM; j++) {
            int temp;
            fin >> temp;
            this->submatrix[i][j] = temp; // -1's are also filled
        }
    }
}

PageMatrix::PageMatrix(string matrixName, int row_start, int col_start, int row_end, int col_end, vector<vector<int>>& data)
{
    logger.log("PageMatrix::PageMatrix");
    this->matrixName = matrixName;
    this->row_start = row_start;
    this->col_start = col_start;
    this->row_end = row_end;
    this->col_end = col_end;
    this->pageName = "../data/temp/" + this->matrixName + "_Page_" + to_string(row_start) + "_" + to_string(col_start) + "_" + to_string(row_end) + "_" + to_string(col_end);
    this->submatrix = data;
}

/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */
vector<vector<int>> PageMatrix::getSubmatrix()
{
    logger.log("PageMatrix::getSubmatrix");
    return this->submatrix;
}

/**
 * @brief writes current page contents to file.
 * 
 */
void PageMatrix::writePage()
{
    ofstream fout(this->pageName, ios::trunc);
    for (int i = 0; i < MATRIX_DIM; i++) {
        for (int j = 0; j < MATRIX_DIM; j++) {
            fout << this->submatrix[i][j];
            if (j != MATRIX_DIM - 1) {
                fout << " ";
            }
        }
        fout << endl;
    }
    fout.close();
}
