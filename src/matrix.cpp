#include "global.h"

/**
 * @brief Construct a new Matrix:: Matrix object
 *
 */
Matrix::Matrix()
{
    logger.log("Matrix::Matrix");
}

/**
 * @brief Construct a new Matrix:: Matrix object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param matrixName 
 */
Matrix::Matrix(string matrixName)
{
    logger.log("Matrix::Matrix");
    this->sourceFileName = "../data/" + matrixName + ".csv";
    this->matrixName = matrixName;
}

/**
 * @brief The load function is used when the LOAD command is encountered. It
 * reads data from the source file, splits it into blocks and updates matrix
 * statistics.
 *
 * @return true if the matrix has been successfully loaded 
 * @return false if an error occurred 
 */
bool Matrix::load()
{
    logger.log("Matrix::load");
    fstream fin(this->sourceFileName, ios::in);
    string line;
    if (getline(fin, line))
    {
        fin.close();

        if (this->getNumberOfRows(line))
            if (this->blockify())
                return true;
    }
    fin.close();
    return false;
}

bool Matrix::getNumberOfRows(string firstLine)
{
    logger.log("Matrix::getNumberOfRows");
    string word;
    stringstream s(firstLine);
    int counter=0;
    while (getline(s, word, ','))
    {
        counter++;
    }
    this->columnCount = counter;
    this->rowCount = counter;
    //total number of blocks/pages
    this->blockCount = ceil((double)this->columnCount/(double)MATRIX_DIM);
    return true;
}



vector<int> simple_tokenizer(string s)
{
	stringstream ss(s);
	string word;
	vector<int>st;
	while (ss >> word) {
		st.push_back(stoi(word));
	}
	return st;
}

string removeComma(string str)
{
    str.erase(remove(str.begin(), str.end(), ','), str.end());
    return str;
}
/**
 * @brief This function splits all the rows and stores them in multiple files of
 * one block size. 
 *
 * @return true if successfully blockified
 * @return false otherwise
 */
bool Matrix::blockify()
{
    logger.log("Matrix::blockify");
    int block_dim=MATRIX_DIM;
    string line;
    ifstream fin(this->sourceFileName, ios::in);
   // cout<<"hi1"<<endl;
    
    vector<int>store_block_ele[this->blockCount];
    // cout<<this->blockCount<<endl;
    int counter_next=0,current_block_number=0,left_ele_count=this->rowCount%block_dim,page_no=0,row_total=0;
  //  cout<<"hi2"<<endl;
    while (getline(fin, line)) {

            string a=removeComma(line);
            vector<int>line_numbers;
            line_numbers = simple_tokenizer(a); //contains numbers in the line
            //    cout<<"hi3"<<endl;
            //adding -1 for leftover places
            int add_left;
            if(left_ele_count!=0)
                add_left=block_dim-left_ele_count;
            else
                add_left=0;
           // cout<<"left over elements"<<add_left<<endl;
            while(add_left--)
                line_numbers.push_back(-1);
            
            for(int x=0;x<line_numbers.size();x++)
            {
                //cout<<"current block no is "<<x/block_dim<<endl;
                store_block_ele[x/block_dim].push_back(line_numbers[x]);
            }
        
            counter_next++;
            
            if(row_total==this->rowCount-1)
            {
                counter_next=0;
             
                for(int block_no=0;block_no< this->blockCount;block_no++)
                 {   
                    vector<vector<int>> page_data (block_dim,vector<int> (block_dim,-1));
                   
                    int flag=0;
                    for(int j=0;j<this->rowCount-page_no*(block_dim);j++)
                    {
                      
                        for(int i=0;i<block_dim;i++)
                        {
                            page_data[j][i]=store_block_ele[block_no][flag++];
                        }
                    }

                    for(int j=this->rowCount-page_no*(block_dim);j<block_dim;j++)
                    {
                        for(int i=0;i<block_dim;i++)
                        {
                            page_data[j][i]=-1;
                        }
                    }
                    
                    int row_start=page_no;
                    int col_start=block_no;
                    bufferManager.writeMatrixPage(this->matrixName, row_start, col_start, row_start, col_start, page_data);

                    page_data.assign(block_dim, vector<int>(this->rowCount-page_no*(block_dim),-1));
                    store_block_ele[block_no].clear();
                }
                page_no++;
            }

            else if(counter_next==block_dim)
            {
                counter_next=0;
             
               
                for(int block_no=0;block_no< this->blockCount;block_no++)
                 {   
                    vector<vector<int>> page_data (block_dim,vector<int> (block_dim,-1));
                    //  cout<<"hi5"<<endl;
                    int flag=0;
                    for(int i=0;i<block_dim;i++)
                    {
                        
                        for(int j=0;j<block_dim;j++)
                        {
                            page_data[i][j]=store_block_ele[block_no][flag++];
                        }
                    }
                    int row_start=page_no;
                    int col_start=block_no;
                    bufferManager.writeMatrixPage(this->matrixName, row_start, col_start, row_start, col_start, page_data);

                    page_data.assign(block_dim, vector<int> (block_dim,-1));
                    store_block_ele[block_no].clear();
                }
                page_no++;
            }
            row_total++;
    }
   // cout<<"hi5"<<endl;
    //add -1 for last rows
    // vector<int>line_numbers;
    // for(int i=0;i<block_dim*(ceil((double)this->columnCount/(double)MATRIX_DIM));i++)
    //     line_numbers.push_back(-1);

    // int add_left=block_dim-left_ele_count;
    // while(add_left--)
    // {
    //         for(int x=0;x<line_numbers.size();x++)
    //         {
    //             store_block_ele[x/block_dim].push_back(line_numbers[x]);
    //         }
            
    // }
      // cout<<"hi4"<<endl;
    fin.close();
    return true;
}


/**
 * @brief Function prints the first few rows of the matrix. If the matrix contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Matrix::print()
{
    logger.log("Matrix::print");
    uint count = min((int)PRINT_COUNT, this->rowCount);
    
    CursorMatrix cursormatrix(this->matrixName, 0, 0, 0, 0);
    PageMatrix page = cursormatrix.getMatrixPage();
    vector<vector<int>> submatrix = page.getSubmatrix();

    for(int i=0; i<count; i++){
        this->writeRow(submatrix[i], count, cout);
    }
    
    printRowCount(this->rowCount);
}


/**
 * @brief called when EXPORT command is invoked to move source file to "data"
 * folder.
 *
 */

void Matrix::makePermanent()
{
    logger.log("Matrix::makePermanent");
    if(!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->matrixName + ".csv";
    ofstream fout(newSourceFile, ios::out);

    int row_start=0, col_start=0, row_end=MATRIX_DIM, col_end=MATRIX_DIM;
    vector<vector<int>>final_matrix(MATRIX_DIM, vector<int> (this->columnCount,-1));

    for (int block_i = 0; block_i < this->blockCount; block_i++) {
        final_matrix.assign(MATRIX_DIM, vector<int> (this->columnCount, -1));
        //cout<<final_matrix.size()<<" "<<final_matrix[0].size()<<endl;
        row_end = min(MATRIX_DIM,this->rowCount-MATRIX_DIM*block_i);
        
        for (int block_j = 0; block_j < this->blockCount; block_j++) {

            col_end = min(MATRIX_DIM,this->columnCount-MATRIX_DIM*block_j);
            //cout<<row_end<<" "<<col_end<<endl;

            CursorMatrix cursormatrix(this->matrixName, block_i,block_j,block_i,block_j);
            PageMatrix page = cursormatrix.getMatrixPage();
            vector<vector<int>> submatrix = page.getSubmatrix();

            for(int i=0; i<row_end; i++){
                for(int j=0; j<col_end; j++){
                    //cout<<j<<" "<<block_j*MATRIX_DIM+i<<" "<<submatrix[i][j]<<endl;
                    final_matrix[i][block_j*MATRIX_DIM+j] = submatrix[i][j];
                }
            }
        }

        for(int i=0; i<row_end; i++){
            this->writeRowWithCommas(final_matrix[i], this->columnCount, fout);
        }
    }
}

/**
 * @brief Function to check if matrix is already exported
 *
 * @return true if exported
 * @return false otherwise
 */
bool Matrix::isPermanent()
{
    logger.log("Matrix::isPermanent");
    if (this->sourceFileName == "../data/" + this->matrixName + ".csv"){
        return true;
    }
    return false;
}

/**
 * @brief Function that returns a cursor that reads rows from this matrix
 * 
 * @return CursorMatrix 
 */
CursorMatrix Matrix::getCursorMatrix(int row_start, int col_start, int row_end, int col_end)
{
    logger.log("Matrix::getCursorMatrix");
    CursorMatrix cursor(this->matrixName, row_start, col_start, row_end, col_end);
    return cursor;
}
