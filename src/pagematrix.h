
/**
 * @brief The PageMatrix object is the main memory representation of a physical page
 * (equivalent to a block). The pageMatrix class and the page.h header file are at the
 * bottom of the dependency tree when compiling files.
 */

class PageMatrix{

    string matrixName;
    int row_start;
    int col_start;
    int row_end;
    int col_end;
    vector<vector<int>> submatrix;

    public:

    string pageName = "";
    PageMatrix();
    PageMatrix(string matrixName, int row_start, int col_start, int row_end, int col_end);
    PageMatrix(string matrixName, int row_start, int col_start, int row_end, int col_end, vector<vector<int>>& data);
    vector<vector<int>> getSubmatrix();
    void writePage();
};