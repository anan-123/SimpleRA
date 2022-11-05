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
1. LOAD <table_name>
To successfully load a table, there should be a csv file names <table_name>.csv consisiting of comma-seperated integers in the data directory. 
2. LIST TABLES
3. PRINT <table_name>
4. RENAME <toColumnName> TO <fromColumnName> FROM <table_name>
5. EXPORT <table_name>
6. CLEAR <table_name>
7. QUIT 
8. <new_table_name> <- CROSS <table_name1> <table_name2>
9. <new_table_name> <- SELECT <condition> FROM <table_name>
10. <new_table_name> <- PROJECT <column1>(,<columnN>)* FROM <table_name>

### MATRICES
1. LOAD MATRIX <matrix name>
2. PRINT MATRIX <matrix name>
3. CROSS TRANSPOSE <matrix1 name> <matrix2 name>
4. EXPORT MATRIX <matrix name>
Exports matrix to <matrix_name.csv> in data directory.
