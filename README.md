# SimpleRA
A Relational Database Management System which supports database operations like load, print, store and transpose for integer-only tables and operates efficiently for large-sized matrices.
## Compilation Instructions


```cd``` into the SimpleRA directory
```
cd SimpleRA
```
```cd``` into the soure directory (called ```src```)
```
cd src
```
To compile
```
make clean
make
```

## To run

Post compilation, an executable names ```server``` will be created in the ```src``` directory
```
./server
```
## QUERIES SYNTAX 

### TABLES
#### load
```LOAD <table_name>```:

To successfully load a table, there should be a csv file names <table_name>.csv consisiting of comma-seperated integers in the data directory. None of the columns in the data file should have the same name and every cell in the table should have a value. 
#### list 
```LIST TABLES```

This command lists all tables that have been loaded or created using assignment statements
#### Print
 ```PRINT <table_name>```
 
 Displays the first PRINT_COUNT (global variable) rows of the table.
 #### Rename
```RENAME <toColumnName> TO <fromColumnName> FROM <table_name>```

<table_name> should be a loaded table in the system and <fromColumnName> should be an exsiting column in the table. <toColumnName> should not be another column in the table
#### Export
```EXPORT <table_name>```
 
All changes made and new tables created, exist only within the system and will be deleted once execution ends (temp file). To keep changes made (RENAME and new tables), you have to export the table (data).
 
#### CLEAR
```CLEAR <table_name>```
  
Removes table from system. 

#### SORT 
```<new_table_name> <- SORT <table_name> BY <column_name> IN ASC | DESC```
Here <table_name> represents the table that has to be sorted and is the
column in the table that the sort order is based on. ASC or DESC are used
to denote ascending or descending orders.  
```<new_table_name> <- SORT <table_name> BY <column_name> IN ASC | DESC BUFFER <buffer_size>```
Here, the optional parameter <buffer_size> denotes the number of main
memory buffer blocks you are allowed to use to carry out the sorting
operation. If no buffer option is provided, the default buffer size is 10.
#### CROSS PRODUCT
```<new_table_name> <- CROSS <table_name1> <table_name2>```
#### SELECT 
```<new_table_name> <- SELECT <condition> FROM <table_name>```
#### PROJECT
```<new_table_name> <- PROJECT <column1>(,<columnN>)* FROM <table_name>```
#### QUIT
  
```QUIT ```
  
Clears all tables present in the system (WITHOUT EXPORTING THEM) (temp file - empty)


### MATRICES

```LOAD MATRIX <matrix name>```
  
```PRINT MATRIX <matrix name>```
  
```CROSS TRANSPOSE <matrix1 name> <matrix2 name>```
  
```EXPORT MATRIX <matrix name>```
 
 Exports matrix to <matrix_name.csv> in data directory.
