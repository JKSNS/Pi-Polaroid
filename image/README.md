# Image

## Experience Summary

### How many hours did it take to complete assignment?

<br><br>
2.5
<br><br>

### What did you enjoy about this lab?

<br><br>
At the very end of the lab, I enjoyed running the program and watching all the files be output to my lab folder. Very satisfying. 
<br><br>

### What were the major challenges you had with this lab? Try to be as detailed as possible.

<br><br>
The major challenge that I had with this lab was understanding the what the bitwise OR operation was doing with the image at the very end. For some reason, I couldn't find the words 
to describe what this function was doing specific to the pixels in the image. 
<br><br>

## Lab Specific Tasks
<br><br>

### Remove Color Channel
1. Save the GREEN masked image as `green_mask.bmp`
<br><br>

![green](./green_mask.bmp)
<br><br>

2. Save the BLUE masked image as `blue_mask.bmp`
<br><br>

![blue](./blue_mask.bmp)
<br><br>

3. Save the RED masked image as `red_mask.bmp`
<br><br>

![red](./red_mask.bmp)
<br><br>

4. What was your method for masking out/turning off the appropriate channel values?
<br><br>

For this lab, while masking out specific color channels, I itereated over every bitmap image pixel. Considering it needed to be removed to match the current image requirements, 
I set the odd-ball pixel's value to zero. 

<br><br>

5. Why did you choose to do it this way?
<br><br>

I chose this method because it made the most sense to me. For this lab, you need to filter images based off particular pixel colors. With my method, considering the pixel needed to be 
filtered out, the value would be set to zero. In my mind, this is the fastest, most efficient way to complete this task.  
<br><br>

### Grayscale
6. Save the grayscaled image as `grayscale.bmp`
<br><br>

![grayscale](./grayscale.bmp)
<br><br>

### OR Filter
7. Save the OR blurred image as `or_filter.bmp`
<br><br>

![or_filter](./or_filter.bmp)
<br><br>

8. Explain why you think the image looks the way it does after applying the or filter.
<br><br>

To pass off the "or filtered" image (or_filter.bmp) and explain its visual behavior, you can describe the effects of the OR filter on the image as follows:
<br><br>

The bitwise OR filter applies brightness and contrast to the image by combining the color values of vertically adjacent pixels. This creates a distorted, ominous, staticy 
filter to the image. Because the value of the red, green, and blue hues can only increase vertically, there's a higher concentration of those partiuclar values displayed 
throughout the image. 


<br><br>
