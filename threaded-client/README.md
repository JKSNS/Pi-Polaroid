# Threaded Client

## Experience Summary

### How many hours did it take to complete assignment?
Four hours. 
### What did you enjoy about this lab?
I enjoyed seeing the evolution of these labs lead us to this very spot. It's interesting to see how each lab before this has played a critical role in where we are today. It was very satisfying seeing my image be sent to the ECEn server, uploaded online, all the while I was still using my doorbell. 

### What were the major challenges you had with this lab? Try to be as detailed as possible.
One of the major challenges that I had with this lab was making the user general functionality work while the image is being sent to the server. That was the very last thing I finished in order to complete this lab. 

## Lab Specific Tasks

### Creating a Thread

1. Why do we use threads?

We use threads to allow our programs to perform multiple tasks at once. 

2. What does pthread stand for?

'Pthread' stands for POSIX thread: Portable Operating System Interface, allowing syncronization amongst threads. 

3. What function spawns a thread?

'Pthread_create' spaws a thread. 

### Passing in Arguments to a Thread

4. What is the generic pointer called in C?

The generic pointer called in C is 'void *', which can point to any data type. 


### Return Values from Threads

5. What function do we use to get return values from threads?

To return values from threads, we use the 'pthread_join()' function. 

### Technical Debt

6. What is technical debt?

Technical debt is the phenomenon when you're coding something for the first time and it's not very polished. Therefore, when you go back to improve the functionality of said code, you have to do some refactoring. 

7. Did you have any technical debt in this lab? What did you do to fix it?

I had a great deal of technical debt in this lab: I had never written a multi-threaded program before. During the last week, I rewrote this lab several times in an attempt to make it more clear, concise, and efficient. I found that everytime I went back to rewrite this lab, I was able to better understand the project as a whole. Therefore, everytime I refactored this lab, it became better. 
