//Server Code
#include "global.h"

using namespace std;

float BLOCK_SIZE = 8;
uint BLOCK_COUNT = 2;
uint PRINT_COUNT = 20;
int MATRIX_DIM = (int)sqrt((BLOCK_SIZE * 1024) / sizeof(int));
//int MATRIX_DIM = 5;
Logger logger;
vector<string> tokenizedQuery;
ParsedQuery parsedQuery;
TableCatalogue tableCatalogue;
MatrixCatalogue matrixCatalogue;
BufferManager bufferManager;

void doCommand()
{
    logger.log("doCommand");
    if (syntacticParse() && semanticParse())
        executeCommand();
    return;
}

int main(void)
{

    regex delim("[^\\s,]+");
    string command;
    system("rm -rf ../data/temp");
    system("mkdir ../data/temp");

    while(!cin.eof())
    {
        cout << "\n> ";
        tokenizedQuery.clear();
        parsedQuery.clear();
        logger.log("\nReading New Command: ");
        getline(cin, command);
        logger.log(command);


        auto words_begin = std::sregex_iterator(command.begin(), command.end(), delim);
        auto words_end = std::sregex_iterator();
        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
            tokenizedQuery.emplace_back((*i).str());
       
        if (tokenizedQuery.size() == 1 && tokenizedQuery.front() == "QUIT")
        {
            break;
        }
        if (tokenizedQuery.size() == 3 && tokenizedQuery[0] == "LOAD" && tokenizedQuery[1] == "MATRIX")
        {
            tokenizedQuery[0]="LOAD_MATRIX";
            tokenizedQuery[1]=tokenizedQuery[2];
            tokenizedQuery.pop_back();
        }
        if (tokenizedQuery.size() == 3 && tokenizedQuery[0] == "EXPORT" && tokenizedQuery[1] == "MATRIX")
        {
            tokenizedQuery[0]="EXPORT_MATRIX";
            tokenizedQuery[1]=tokenizedQuery[2];
            tokenizedQuery.pop_back();
        }
        if (tokenizedQuery.size() == 3 && tokenizedQuery[0] == "PRINT" && tokenizedQuery[1] == "MATRIX")
        {
            tokenizedQuery[0]="PRINT_MATRIX";
            tokenizedQuery[1]=tokenizedQuery[2];
            tokenizedQuery.pop_back();
        }

        if (tokenizedQuery.empty())
        {
            continue;
        }

        if (tokenizedQuery.size() == 1)
        {
            cout << "SYNTAX ERROR" << endl;
            continue;
        }

        doCommand();
    }
}