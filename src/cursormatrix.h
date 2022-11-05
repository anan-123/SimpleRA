class CursorMatrix{
    public:
    PageMatrix page;
    int row_start;
    int col_start;
    int row_end;
    int col_end;
    string matrixName;
    int pagePointer;

    public:
    CursorMatrix(string matrixName, int row_start, int col_start, int row_end, int col_end);
    PageMatrix getMatrixPage();
};