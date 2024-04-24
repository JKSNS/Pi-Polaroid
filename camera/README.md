# Camera

## Experience Summary

### How many hours did it take to complete assignment?
6 hours. 

### What did you enjoy about this lab?
I enjoyed troubleshooting and making the image filters work. 

### What were the major challenges you had with this lab? Try to be as detailed as possible.
Similar to what I enjoyed, the biggest challenge that I had with this lab was making those filters work. 
I was trying to apply the filters to the actual pixel data of the image I just captured rather than a 
bitmap copy of said image data. After running through the UML diagram available through the ECEN github 
I was able to better understand the flow of data and adjust accordingly. 

## Lab Specific Tasks

### Creating Large Buffers

1. What command is used to allocate memory?

Malloc was used to allocate memory, then followed by the free(); command. 


2. What library needs to be included in order to use this command?

stdlib.h.

3. Where on the computer is the memory allocated? (Stack or Heap). Why is it stored there and not in the other place?

Memory is allocated to the heap in this scenario because it allows for much more space for larger variables as such. 

4. What command is used to deallocate memory from the program? Why is it important to use this function?

The command used to deallocate memory from the program, as stated previously, is the 'free();' command. This being imporant to use 
because it returns the memory back to the system for future use in addition to preventing memory leaks. 

5. Look at the **Memory Allocation in C** link in the **Explore More!** section at the bottom of the lab. List the other memory allocation functions discussed in the article and what they do differently than the commands described above.

The other memory allocation functions discussed in the article are: 
<br>
Calloc- allocates memory by initializing all bits to zero. 
<br> 
Realloc- resizes an allocated memory block without losing the old data. 

### Writing to a File

6. What do you need to do a file before you can write to it in C? What function is used for this?

Before you can write to a file in C, you must 'fopen' said file. 

7. What function writes to a file in C?

To write to a file in C, you use the 'fwrite' function. 

8. What file mode allows for adding text to the end of the file?

To add text to the end of a file, you can use the 'a' command that stands for appending to the end of a file. 

9. What mode indicates that you are writing to a binary file?

The 'b' character used with 'w' in 'fopen' to form 'wb' is indicitive of a binary mode being in use, specifically compatible with Windows. Conversely, on Unix and Linux systems, all files are treated as binary. In other words, the 'b' flag does not change the behavior of file I/O operations, but it is still good practice to use it for portable code.
