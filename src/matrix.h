#include "cursormatrix.h"

/**
 * @brief The Matrix class holds all information related to a loaded matrix. It
 * also implements methods that interact with the parsers, executors, cursors
 * and the buffer manager. 
 *
 */
class Matrix
{
    vector<unordered_set<int>> distinctValuesInColumns;

public:
    string sourceFileName = "";
    string matrixName = "";
    vector<uint> distinctValuesPerColumnCount;
    int columnCount = 0;
    int rowCount = 0;
    int numcolsperblock = 0;
    int numrowsperblock = 0;
    uint blockCount = 0;
    uint maxRowsPerBlock = 0;
    vector<uint> rowsPerBlockCount;
   
    bool blockify();
    void updateStatistics(vector<int> row);
    Matrix();
    Matrix(string MatrixName);
    bool load();
    void print();
    void makePermanent();
    bool isPermanent();
    bool getNumberOfRows(string firstLine);
    CursorMatrix getCursorMatrix(int row_start, int col_start, int row_end, int col_end);
 
    /**
 * @brief Static function that takes a vector of valued and prints them out in a
 * comma seperated format.
 *
 * @tparam T current usaages include int and string
 * @param row 
 */
template <typename T>
void writeRow(vector<T> row, int columnCount, ostream &fout)
{
    logger.log("Matrix::printRow");
    for (int columnCounter = 0; columnCounter < min(columnCount,(int)row.size()); columnCounter++)
    {
        if (columnCounter != 0)
            fout << " ";
        fout << row[columnCounter];
    }
    fout << endl;
}

/**
 * @brief Static function that takes a vector of valued and prints them out in a
 * comma seperated format.
 *
 * @tparam T current usaages include int and string
 * @param row 
 */
template <typename T>
void writeRow(vector<T> row, int columnCount)
{
    logger.log("Matrix::printRow");
    ofstream fout(this->sourceFileName, ios::app);
    this->writeRow(row, fout);
    fout.close();

    logger.log("Matrix::printRow");
    for (int columnCounter = 0; columnCounter < min(columnCount,(int)row.size()); columnCounter++)
    {
        if (columnCounter != 0)
            fout << " ";
        fout << row[columnCounter];
    }
    fout << endl;
}

template <typename T>
void writeRowWithCommas(vector<T> row, int columnCount, ostream &fout)
{
    logger.log("Matrix::writeRowWithCommas");
    for (int columnCounter = 0; columnCounter < min(columnCount,(int)row.size()); columnCounter++)
    {
        if (columnCounter != 0)
            fout << ", ";
        fout << row[columnCounter];
    }
    fout << endl;
}
};