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
```LOAD <table_name>```:

To successfully load a table, there should be a csv file names <table_name>.csv consisiting of comma-seperated integers in the data directory. None of the columns in the data file should have the same name and every cell in the table should have a value. 

```LIST TABLES```

This command lists all tables that have been loaded or created using assignment statements

 ```PRINT <table_name>```
 
 Displays the first PRINT_COUNT (global variable) rows of the table.
 
```RENAME <toColumnName> TO <fromColumnName> FROM <table_name>```

<table_name> should be a loaded table in the system and <fromColumnName> should be an exsiting column in the table. <toColumnName> should not be another column in the table
  
```EXPORT <table_name>```
  
All changes made and new tables created, exist only within the system and will be deleted once execution ends (temp file). To keep changes made (RENAME and new tables), you have to export the table (data).
  
```CLEAR <table_name>```
  
Removes table from system. 
  
```QUIT ```
  
Clears all tables present in the system (WITHOUT EXPORTING THEM) (temp file - empty)
  
```<new_table_name> <- CROSS <table_name1> <table_name2>```
  
```<new_table_name> <- SELECT <condition> FROM <table_name>```
  
```<new_table_name> <- PROJECT <column1>(,<columnN>)* FROM <table_name>```

### MATRICES

```LOAD MATRIX <matrix name>```
  
```PRINT MATRIX <matrix name>```
  
```CROSS TRANSPOSE <matrix1 name> <matrix2 name>```
  
```EXPORT MATRIX <matrix name>```
 
 Exports matrix to <matrix_name.csv> in data directory.
