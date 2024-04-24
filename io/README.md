# I/O

## Experience Summary

### How many hours did it take to complete assignment?
5 hours. 

### What did you enjoy about this lab?
I enjoyed debugging and fixing the code related to user input specific to the buttons. 

### What were the major challenges you had with this lab? Try to be as detailed as possible.
Similarly, the biggest challege that I faced in this lab was getting the program to appropriately 
recognize and operate given user input specific to the buttons. 

## Lab Specific Tasks

1. What are the file operation functions we use in this lab and what do they do?

The file operation functions that we used in this lab were: 
<br>
'fopen()': opens the file and returns the subsequent file pointer. 
'fclose()': closes the file that is associated with said file pointer. 
'fread()': reads the file into a buffer. 
'fwrite()': writes the data from the buffer to the file. 

2. What are the directory operation functions we use in this lab and what do they do?

The directory operation functions that we used in this lab were: 
<br>
'opendir()': opens the directory and reutnrs the stream pointer. 
'readdir()': reads the next entry from the directory stream. 
'closedir()': Closes the directory stream. 

3. What are the FILE and DIR pointers and how do they behave?

The 'FILE' pointer is specific to a structure that represents the related open file. This is essential while reading, writing, opening and closing files. 
The 'DIR' pointer is specific to a structure that represents an open directory. This is essential while opening, reading, and closing directories. 

4. How does the FILE pointer differ from regular pointers in C?
|
The 'FILE' pointer differs from regular C pointers because it is specific to file operations compared to being able to point at any data type. 
Therefore, 'FILE' pointers are used with the file operation functions like 'fopen()', 'fclose()', 'fread()', and 'fwrite()' to perform the very file-related operations that are specific to the 'FILE' pointer. 
On the other hand, regular pointers in C are used for regular memory manipulation and related tasks. 

5. What are the risks associated with not closing a file after operations? Describe any experience you had with this during the lab.

The risks associated with not closing a file after operations comes in that it could lead to memory or resource leaks, potentially making you lose or corrupt data. 
Specific to this lab, if you didn't close a file, it could result in running out of the available file descriptors, causing our program to fail when trying to open new files. 

6. What would happen if you try to open a non-existent directory using the DIR pointer?

If you try to open a non-existent directory using the 'DIR' pointer, it would return NULL. In other words, it would fail to open said directory. 

