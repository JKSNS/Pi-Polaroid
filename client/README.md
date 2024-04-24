# Client

## Experience Summary

### How many hours did it take to complete assignment?
1 hour. 

### What did you enjoy about this lab?
I enjoyed seeing the images being uploaded to the http website/ ECEn server. 

### What were the major challenges you had with this lab? Try to be as detailed as possible.
I didn't have any major challenges while finishing this lab. 

## Lab Specific Tasks

### Config Struct
1. What is the purpose of a `struct` in C? How do we use the struct in this lab?

The purpose of a 'struct' in C is to group various different data types together under one single name, in a block of memory, specifically 
for the sake of organization. 

2. Look at the implementation of the `client_connect()` function. What structs are used there? While you are not expected to know the particulars of how the socket creation works, what purpose `addrinfo` struct serve?

The structs used in the 'client_connect()' function are the 'addrinfo' and 'getaddrinfo()' structs. The 'addrinfo' struct 
serves the purpose of abstracting socket details, allowing for IPv4 or IPv6 protocols to be used which simplifies the 
setting up of a connection between the camera and ECEn server. 

### Network Socket
3. What data type is used to represent a socket in C?

The data type that is used to represent a socket in C is an 'int'. 

4. What does a file descriptor do?

A file descriptor is another abstraction used to manage access to files or I/O resources, like sockets. 


5. List some similarities and differences between writing to a file and writing to a socket?

Similarities: 
Both use file descriptors. 
Reading, writing, and closing are all similar in conventions. 

Differences: 
Writing to a socket requires network instructions which adds a level of complexity to the process: introducing the possibility for latency, data loss, and other network errors. 
When you're writing to a file on your local machine, it's specific to your local system. Whereas when you write to a socket, you're sending that data across a network. 

### Sending Data

6. What command is used to send data over a network socket? What parameters does it take and what do they mean?

To send data over a network socket, you use the following commands: 

'sockfd': File descriptor. 
'buf': Pointer to the buffer beign sent. 
'len': Length of data. 
'flags': Influencing the operation of the send. 

Overall, these functions telling the 'send()' function which socket to use, what data should be sent, how long, and any other special instructions. 

