#include "global.h"

CursorMatrix::CursorMatrix(string matrixName, int row_start, int col_start, int row_end, int col_end)
{
    logger.log("Cursor::CursorMatrix");
    this->page = bufferManager.getMatrixPage(matrixName, row_start, col_start, row_end, col_end);
    this->pagePointer = 0;
    this->matrixName = matrixName;
    this->row_start = row_start;
    this->col_start = col_start;
    this->row_end = row_end;
    this->col_end = col_end;
}

PageMatrix CursorMatrix::getMatrixPage()
{
    logger.log("Cursor::nextMatrixPage");
    return this->page;
}