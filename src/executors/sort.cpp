#include"global.h"
int idx=0;
int sort_flag=0;
int leftover=0;
int groups=0;
int maxrows=0;
int B=(BLOCK_COUNT-1);
string tname="brunch_itr_";
/**
 * @brief File contains method to process SORT commands.
 * 
 * syntax:
 * R <- SORT relation_name BY column_name IN sorting_order
 * 
 * sorting_order = ASC | DESC 
 */
bool syntacticParseSORT(){
    logger.log("syntacticParseSORT");
    if(tokenizedQuery.size()< 8 || tokenizedQuery[4] != "BY" || tokenizedQuery[6] != "IN" || tokenizedQuery.size() > 10){
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
    parsedQuery.queryType = SORT;
    parsedQuery.sortResultRelationName = tokenizedQuery[0];
    parsedQuery.sortRelationName = tokenizedQuery[3];
    parsedQuery.sortColumnName = tokenizedQuery[5];
    string sortingStrategy = tokenizedQuery[7];
    if(sortingStrategy == "ASC")
        parsedQuery.sortingStrategy = ASC;
    else if(sortingStrategy == "DESC")
        parsedQuery.sortingStrategy = DESC;
    else{
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
     if (tokenizedQuery.size() > 8) {
        BLOCK_COUNT = stoi(tokenizedQuery[9]);
        B=BLOCK_COUNT-1;
    }
    return true;
}

bool semanticParseSORT(){
    logger.log("semanticParseSORT");

    if(tableCatalogue.isTable(parsedQuery.sortResultRelationName)){
        cout<<"SEMANTIC ERROR: Resultant relation already exists"<<endl;
        return false;
    }

    if(!tableCatalogue.isTable(parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Relation doesn't exist"<<endl;
        return false;
    }

    if(!tableCatalogue.isColumnFromTable(parsedQuery.sortColumnName, parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
        return false;
    }

    return true;
}
bool sortcol1(const vector<int>& v1, const vector<int>& v2)
{
    return v1[idx] < v2[idx];
    
}
bool sortcol2(const vector<int>& v1, const vector<int>& v2)
{
    return v1[idx] > v2[idx];
    
}
int ceilfn(int a,int b)
{
    return (a / b) + ((a % b) != 0) ;
}
void sortpages_DESC(int iterations,int sort_flag)
{
    vector <Cursor> p;
    for(int it=1;it<=iterations;it++)
    {
        Table table = *tableCatalogue.getTable(tname+to_string(it-1));
        Cursor ct = table.getCursor();
        int s=0;
        int x=table.blockCount/(B*it);
        int y=table.blockCount%(B*it);

        //write to new table
        Table* endtable = new Table(tname+to_string(it),table.columns);
        //get cursor to all pages in p
        for(int i=0;i<table.blockCount;i++)
        {
            ct.nextPage(i);
            p.push_back(ct);
        }
        //initialize
        vector<Cursor> tmp;
        vector<int>page_c;
        vector<int>size_group;
        vector<int>current_size_group;
        vector<int>flag_group;

        for(int i=0;i<x;i++)
        {
            size_group.push_back(B*it*table.maxRowsPerBlock);
         //   cout<<s<<" ";
            tmp.push_back(p[s]);
            page_c.push_back(s);
            flag_group.push_back(0);
            current_size_group.push_back(0);
            s+=B*it;
        }
        if(y==0)
        {
            size_group[x-1]=table.rowCount-(x-1)*B*it*table.maxRowsPerBlock;
          
        }
        if(y>0){
        size_group.push_back(table.rowCount-(x)*B*it*table.maxRowsPerBlock);
        //cout<<s<<endl;
        tmp.push_back(p[s]);
        page_c.push_back(s);
        flag_group.push_back(0);
        current_size_group.push_back(0);
        }
        int no_of_group=x+(y!=0); //or tmp.size()
      // cout<<" no of groups "<<x+(y!=0)<<"NO OF PAGE POINTERS" <<tmp.size()<<endl;
        // for(int i=0;i<size_group.size();i++)
        // {
        //    cout<<size_group[i]<<endl;
        // }
        int g=0;
        vector<int>row;
        vector<int>max_row;
        int max_row_val=-1,max_row_idx,exitflag=0;
        while(g<no_of_group)
        {
            max_row_val=-100;
            max_row_idx=0;
            for(int i=0;i<tmp.size();i++)
            {
                if(!flag_group[i])
                {
                    // if(current_size_group[i]%table.maxRowsPerBlock==1 && current_size_group[i]>table.maxRowsPerBlock)
                    // {
                    //     cout<<"new page no "<<page_c[i]+(current_size_group[i]/table.maxRowsPerBlock)<<endl;
                    //       tmp[i]=p[page_c[i]+(current_size_group[i]/table.maxRowsPerBlock)];
                    // }
                      
                    row=tmp[i].getRow();
                    if(row.empty())
                    {
                      //  cout<<"new page no "<<page_c[i]+(current_size_group[i]/table.maxRowsPerBlock)<<endl;
                        tmp[i]=p[page_c[i]+(current_size_group[i]/table.maxRowsPerBlock)];
                        row=tmp[i].getRow();
                    }
                        // row=tmp[i].getNext();
                    // row=tmp[i].getRow();
                    if(!row.empty() && row[idx]>max_row_val)
                    {
                        max_row_val=row[idx];
                        max_row_idx=i;
                        max_row=row;
                    }
                    // for(int k=0;k<row.size();k++)
                    //         cout<<row[k]<<" ";
                    // cout<<endl;
                }
            }
            endtable->writeRow<int>(max_row);
           // cout<<"total elements written "<<exitflag<<endl;
            exitflag++;
           
            current_size_group[max_row_idx]++;
          //  cout<<" current group "<<max_row_idx<<" size" <<current_size_group[max_row_idx]<<endl;
            if(current_size_group[max_row_idx]==(size_group[max_row_idx]))
            {
                flag_group[max_row_idx]=1;
                g++;
                
            }
            else
                row=tmp[max_row_idx].getNext();
            
        }
      
       // cout<<"no of elements written "<<exitflag<<endl;
        if(endtable->blockify())
            tableCatalogue.insertTable(endtable);
        p.clear();
        // cout<<" no of groups "<<groups<<endl;
        // groups=ceilfn(table.rowCount,B*it*table.maxRowsPerBlock);
        //break;
        tmp.clear();
    }
    //WRITE TO NEW TABLE
    Table table = *tableCatalogue.getTable(tname+to_string(iterations));
    Cursor cnew = table.getCursor();
    Table* Table_n = new Table(parsedQuery.sortResultRelationName, table.columns);
    vector<int> row = cnew.getNext();
    while (!row.empty())
    {
        Table_n->writeRow<int>(row);
        row=cnew.getNext();
    }
    if(Table_n->blockify())
        tableCatalogue.insertTable(Table_n);

     //delete in between tables
     for (int it=0;it<=iterations;it++) {
        tableCatalogue.deleteTable(tname+to_string(it));
    }
    return;

}
void sortpages_ASC(int iterations,int sort_flag)
{
    vector <Cursor> p;
    for(int it=1;it<=iterations;it++)
    {
        Table table = *tableCatalogue.getTable(tname+to_string(it-1));
        Cursor ct = table.getCursor();
        int s=0;
        int x=table.blockCount/(B*it);
        int y=table.blockCount%(B*it);

        //write to new table
        Table* endtable = new Table(tname+to_string(it),table.columns);
        //get cursor to all pages in p
        for(int i=0;i<table.blockCount;i++)
        {
            ct.nextPage(i);
            p.push_back(ct);
        }
        //initialize
        vector<Cursor> tmp;
        vector<int>page_c;
        vector<int>size_group;
        vector<int>current_size_group;
        vector<int>flag_group;

        for(int i=0;i<x;i++)
        {
            size_group.push_back(B*it*table.maxRowsPerBlock);
         //   cout<<s<<" ";
            tmp.push_back(p[s]);
            page_c.push_back(s);
            flag_group.push_back(0);
            current_size_group.push_back(0);
            s+=B*it;
        }
        if(y==0)
        {
            size_group[x-1]=table.rowCount-(x-1)*B*it*table.maxRowsPerBlock;
          
        }
        if(y>0){
        size_group.push_back(table.rowCount-(x)*B*it*table.maxRowsPerBlock);
        //cout<<s<<endl;
        tmp.push_back(p[s]);
        page_c.push_back(s);
        flag_group.push_back(0);
        current_size_group.push_back(0);
        }
        int no_of_group=x+(y!=0); //or tmp.size()
     //  cout<<" no of groups "<<x+(y!=0)<<"NO OF PAGE POINTERS" <<tmp.size()<<endl;
        // for(int i=0;i<size_group.size();i++)
        // {
        //    cout<<size_group[i]<<endl;
        // }
        int g=0;
        vector<int>row;
        vector<int>max_row;
        long int max_row_val=999999999999999999;
        int max_row_idx,exitflag=0;
        while(g<no_of_group)
        {
            max_row_val=999999999999999999;
            max_row_idx=0;
            for(int i=0;i<tmp.size();i++)
            {
                if(!flag_group[i])
                {
                    // if(current_size_group[i]%table.maxRowsPerBlock==1 && current_size_group[i]>table.maxRowsPerBlock)
                    // {
                    //     cout<<"new page no "<<page_c[i]+(current_size_group[i]/table.maxRowsPerBlock)<<endl;
                    //       tmp[i]=p[page_c[i]+(current_size_group[i]/table.maxRowsPerBlock)];
                    // }
                      
                    row=tmp[i].getRow();
                    if(row.empty())
                    {
                       // cout<<"new page no "<<page_c[i]+(current_size_group[i]/table.maxRowsPerBlock)<<endl;
                        tmp[i]=p[page_c[i]+(current_size_group[i]/table.maxRowsPerBlock)];
                        row=tmp[i].getRow();
                    }
                        // row=tmp[i].getNext();
                    // row=tmp[i].getRow();
                    if(!row.empty() && row[idx]<max_row_val)
                    {
                        max_row_val=row[idx];
                        max_row_idx=i;
                        max_row=row;
                    }
                    // for(int k=0;k<row.size();k++)
                    //         cout<<row[k]<<" ";
                    // cout<<endl;
                }
            }
            endtable->writeRow<int>(max_row);
          //  cout<<"total elements written "<<exitflag<<endl;
            exitflag++;
           
            current_size_group[max_row_idx]++;
          //  cout<<" current group "<<max_row_idx<<" size" <<current_size_group[max_row_idx]<<endl;
            if(current_size_group[max_row_idx]==(size_group[max_row_idx]))
            {
                flag_group[max_row_idx]=1;
                g++;
                
            }
            else
                row=tmp[max_row_idx].getNext();
            
        }
      
      //  cout<<"no of elements written "<<exitflag<<endl;
        if(endtable->blockify())
            tableCatalogue.insertTable(endtable);
        p.clear();
        // cout<<" no of groups "<<groups<<endl;
        // groups=ceilfn(table.rowCount,B*it*table.maxRowsPerBlock);
        //break;
        tmp.clear();
    }
     //WRITE TO NEW TABLE
    Table table = *tableCatalogue.getTable(tname+to_string(iterations));
    Cursor cnew = table.getCursor();
    Table* Table_n = new Table(parsedQuery.sortResultRelationName, table.columns);
    vector<int> row = cnew.getNext();
    while (!row.empty())
    {
        Table_n->writeRow<int>(row);
        row=cnew.getNext();
    }
    if(Table_n->blockify())
        tableCatalogue.insertTable(Table_n);
    
    //delete in between tables
     for (int it=0;it<=iterations;it++) {
        tableCatalogue.deleteTable(tname+to_string(it));
    }
    return;

}

void executeSORT(){
    logger.log("executeSORT");
    //flag for ASC or DESC sort
    int sort_flag=0;
    (parsedQuery.sortingStrategy==ASC)?sort_flag=0:sort_flag=1;

    Table table = *tableCatalogue.getTable(parsedQuery.sortRelationName);
    int colIndex = table.getColumnIndex(parsedQuery.sortColumnName);
    idx=colIndex;
   // cout<<colIndex<<endl;
    tname="brunch_itr_"+parsedQuery.sortRelationName;
    Table* Table_1 = new Table("brunch_itr_"+parsedQuery.sortRelationName+"0", table.columns);
    Cursor cursor = table.getCursor();
    
    vector<vector<int>> page_row;
    vector<int> row = cursor.getNext();

    //sort page by page and write back
    int count=0;
    maxrows=table.maxRowsPerBlock*(B);
    while (!row.empty())
    {
        if(count==maxrows)
        {
            (sort_flag==0)?sort(page_row.begin(), page_row.end(), sortcol1):sort(page_row.begin(), page_row.end(), sortcol2);

            for (vector<int> row : page_row)
            {
                 Table_1->writeRow<int>(row);
            }
            count=0;
            page_row.clear();
        }
        page_row.push_back(row);
        row = cursor.getNext();
        count++;
    }
   
    //write back remaining elements
    (sort_flag==0)?sort(page_row.begin(), page_row.end(), sortcol1):sort(page_row.begin(), page_row.end(), sortcol2);
    
    for (vector<int> row : page_row)
    { 
        Table_1->writeRow<int>(row);
    }
    count=0;
    page_row.clear();
   
    if(Table_1->blockify())
        tableCatalogue.insertTable(Table_1);
    else{
        cout<<"Empty Table"<<endl;
        Table_1->unload();
        delete Table_1;
        return;
    }
    int iterations = ceil(log(table.blockCount)/log(BLOCK_COUNT-1))-1;

    //left over rows update
    groups = ceilfn(table.blockCount,B);
   // cout<<"block count"<<table.blockCount<<" B is :"<<B << "groups are " <<groups<<endl;
    Cursor cursor_left = table.getCursor();
    cursor_left.nextPage(table.blockCount-1);
    vector<int> ro = cursor_left.getNext();

    while (!ro.empty())
    {
        leftover++;
        ro=cursor_left.getNext();
    }
 //   cout<<BLOCK_COUNT<<B<<endl;
    if(iterations>0){
    (sort_flag==1)?sortpages_DESC(iterations,sort_flag):sortpages_ASC(iterations,sort_flag);
    }
    else
    {
       // cout<<"hi"<<endl;
        Table table = *tableCatalogue.getTable(tname+to_string(0));
        Cursor cnew = table.getCursor();
        Table* Table_n = new Table(parsedQuery.sortResultRelationName, table.columns);
        vector<int> row = cnew.getNext();
        while (!row.empty())
        {
            Table_n->writeRow<int>(row);
            row=cnew.getNext();
        }
        if(Table_n->blockify())
            tableCatalogue.insertTable(Table_n);
        
        
        tableCatalogue.deleteTable(tname+to_string(0));
        

    }


    return;
}
 
       