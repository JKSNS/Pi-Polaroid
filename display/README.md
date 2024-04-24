# Display

## Experience Summary

### How many hours did it take to complete assignment?
4 hours. 
<br>

### What did you enjoy about this lab?
I enjoyed creating the hashtag: learned a lot about C based programming during the process. 
<br>

### What were the major challenges you had with this lab? Try to be as detailed as possible.
Even though I enjoyed it, I had some trouble hard coding the hashtag. I initially tried to make everything relative to one another which ended up causing more problems than solutions. Because of this realization, 
I pivoted to hard coding the hashtag and it was significantly easier. 

## Lab Specific Tasks

### BCM2835 Library
1. What does the BCM2835 library do?

<br> 
The BCM2835 library provides a GPIO (General Purpose Input/Output) specific to my Raspberry Pi's hardware interface. 
<br>
<br>

2. How does the BCM2835 library differ from other GPIO libraries you might have come across?
   
<br> 
The BCM2835 library differs from other GPIO libraries because BCM2835 provides more direct, lower level access to the GPIO 
hardware associated. BCM2835 works more directly with hardware manipulation and customization, requiring the user to have a more 
in depth knowledge of what they're doing and how they're configuring their system. 
<br>


### Drawing to Screen

3. Where is the origin of the LCD screen?

<br> 
The origin of the LCD screen is in the top left corner, considering you’re holding the device with the button closest to the ground. 
<br>
<br>
4. How are the fonts passed into the `display_draw_string` function?
<br>
<br> 
The fonts are passed into the 'display_draw_string' function by referencing the 'fonts.h' library, specific to the address of the font 
that you want to use present in that file. 
<br>
<br>
5. What do you have to call before you can start drawing to the screen?
<br>
<br> 
Before you can start drawing to the screen, you have to call the 'display_init()' function. 
<br>

### Interacting with Buttons

6. What do you have to call before you can read from the buttons?

<br> 
Before you can read from the buttons, you have to call the 'buttons_init()' function. Interesting to note, the 'display_init()' function 
needs to be called before the 'buttons_init()' function or else there will be a segmentation fault. 
<br>

### Device Delay

8. Why do we add delays in our code? Which function accomplishes this and what library is it from?

<br> 
We add delays to our code because they control the overall flow and timing. Without delays, there's potential for 
the CPU to be used excessively, resulting in longer runtimes or increase wear on the computer hardware. The function 'delay_ms()' from 
the 'device.h' library is used  to introduce said delays. 
<br>
<br>
9. Can you think of any potential issues if the delay is set too long or too short in certain applications?
<br>
<br> 
Some potential issues is the delay is set too long or too short in certain applications include: excessive runtimes causing an increase 
in CPU load, ineffective management of tasks, or increase lag between the user and the application. This could be particularly detrimental 
in the case of high octane aviation or similar machinery where time is of the essence. Similarly, this could also be said about 
softwares or applications used in the healthcare industry where every second counts. The timeliness of your software is incredibly important, 
something that I learned how to configure and manage in this lab. 
<br>

### Miscellaneous

10. What is a `.gitignore` and how is it used?

<br> 
The '.gitignore' is a file you include considering you want to keep it personal to your own device. This is generally used to store 
sensitive configuration information specific to keys, passwords, or similar things of that sort. 
<br>
<br>
11. What command did you use to compile your code?
<br>
<br> 
To compile my code, I used 'sudo make' which was then ran by the command 'sudo ./main'. 
<br>


