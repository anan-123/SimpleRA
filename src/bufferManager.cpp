#include "global.h"

BufferManager::BufferManager()
{
    logger.log("BufferManager::BufferManager");
}

/**
 * @brief Function called to read a page from the buffer manager. If the page is
 * not present in the pool, the page is read and then inserted into the pool.
 * @param matrixName
 * @param tableName 
 * @param pageIndex 
 * @return Page 
 */
Page BufferManager::getPage(string tableName, int pageIndex)
{
    logger.log("BufferManager::getPage");
    string pageName = "../data/temp/"+tableName + "_Page" + to_string(pageIndex);
    if (this->inPool(pageName))
        return this->getFromPool(pageName);
    else
        return this->insertIntoPool(tableName, pageIndex);
}

PageMatrix BufferManager::getMatrixPage(string matrixName, int row_start, int col_start, int row_end, int col_end)
{
    logger.log("BufferManager::getMatrixPage");
    string pageName = "../data/temp/" + matrixName + "_Page_" + to_string(row_start) + "_" + to_string(col_start) + "_" + to_string(row_end) + "_" + to_string(col_end);
    if (this->inMatrixPool(pageName)){
        return this->getFromMatrixPool(pageName);
    }
    else
        return this->insertIntoMatrixPool(matrixName, row_start, col_start, row_end, col_end);
}
/**
 * @brief Checks to see if a page exists in the pool
 *
 * @param pageName 
 * @return true 
 * @return false 
 */
bool BufferManager::inPool(string pageName)
{
    logger.log("BufferManager::inPool");
    for (auto page : this->pages)
    {
        if (pageName == page.pageName)
            return true;
    }
    return false;
}

bool BufferManager::inMatrixPool(string pageName)
{
    logger.log("BufferManager::inMatrixPool");
    for (auto page : this->matrixpages)
    {
        if (pageName == page.pageName)
            return true;
    }
    return false;
}

/**
 * @brief If the page is present in the pool, then this function returns the
 * page. Note that this function will fail if the page is not present in the
 * pool.
 *
 * @param pageName 
 * @return Page 
 */
Page BufferManager::getFromPool(string pageName)
{
    logger.log("BufferManager::getFromPool");
    for (auto page : this->pages)
        if (pageName == page.pageName)
            return page;
}

PageMatrix BufferManager::getFromMatrixPool(string pageName)
{
    logger.log("BufferManager::getFromMatrixPool");
    for (auto page : this->matrixpages)
        if (pageName == page.pageName)
            return page;
}

/**
 * @brief Inserts page indicated by tableName and pageIndex into pool. If the
 * pool is full, the pool ejects the oldest inserted page from the pool and adds
 * the current page at the end. It naturally follows a queue data structure. 
 * @param matrixName
 * @param tableName 
 * @param pageIndex 
 * @return Page 
 */
Page BufferManager::insertIntoPool(string tableName, int pageIndex)
{
    logger.log("BufferManager::insertIntoPool");
    Page page(tableName, pageIndex);
    if (this->pages.size() >= BLOCK_COUNT)
        pages.pop_front();
    pages.push_back(page);
    return page;
}

PageMatrix BufferManager::insertIntoMatrixPool(string matrixName, int row_start, int col_start, int row_end, int col_end)
{
    logger.log("BufferManager::insertIntoMatrixPool");
    PageMatrix page(matrixName, row_start, col_start, row_end, col_end);
    if (this->matrixpages.size() >= BLOCK_COUNT)
        matrixpages.pop_front();
    matrixpages.push_back(page);
    return page;
}
/**
 * @brief The buffer manager is also responsible for writing pages. This is
 * called when new tables are created using assignment statements.
 * @param matrixName
 * @param tableName 
 * @param pageIndex 
 * @param rows 
 * @param rowCount 
 */
void BufferManager::writePage(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("BufferManager::writePage");
    Page page(tableName, pageIndex, rows, rowCount);
    page.writePage();
}

void BufferManager::writeMatrixPage(string matrixName, int row_start, int col_start, int row_end, int col_end, vector<vector<int>>& data)
{
    logger.log("BufferManager::writeMatrixPage");
    PageMatrix page(matrixName, row_start, col_start, row_end, col_end, data);

    for (auto &p : this->matrixpages) {
        if (p.pageName == page.pageName) {
            p = page;
            break;
        }
    }

    page.writePage();
}
/**
 * @brief Deletes file names fileName
 *
 * @param fileName 
 */
void BufferManager::deleteFile(string fileName)
{
    
    if (remove(fileName.c_str()))
        logger.log("BufferManager::deleteFile: Err");
        else logger.log("BufferManager::deleteFile: Success");
}

/**
 * @brief Overloaded function that calls deleteFile(fileName) by constructing
 * the fileName from the tableName and pageIndex.
 * @param matrixName
 * @param tableName 
 * @param pageIndex 
 */
void BufferManager::deleteFile(string tableName, int pageIndex)
{
    logger.log("BufferManager::deleteFile");
    string fileName = "../data/temp/"+tableName + "_Page" + to_string(pageIndex);
    this->deleteFile(fileName);
}
void BufferManager::deleteMatrixFile(string matrixName, int row_start, int col_start, int row_end, int col_end)
{
    logger.log("BufferManager::deleteMatrixFile");
    string fileName = "../data/temp/" + matrixName + "_Page_" + to_string(row_start) + "_" + to_string(col_start) + "_" + to_string(row_end) + "_" + to_string(col_end);
    this->deleteFile(fileName);
}