# Group 14: Using multithreading in a 2D Physics Engine in the context of Games

The main premise of our project is to utilize single and multi threading within a physics engine and show the differences in performance.
In order to do this we created two identical raylib projects, rain and particle, to show the difference between single and multi threaded program.
We are using C++ and logging the performance data in csv's which we parsed using python in order to create our graphs.
If you want to know how to use the python scripts refer to the readme in scripts.


## How to run the Single or Multi raylib project?

- Step 1: Download the entire GitHub project and extract the files.
- Step 2: Open Single or Multi folder and double click the "main.code-workspace" file to open in VSCode
- Step 3: Inside VSCode find and click on the "main.cpp" to open the main source code
- Step 4: To run the code either press "F5" while inside "main.cpp file or press the play button in the top right corner and click "Debug" or "Run"

The steps are the same for both version of the code. At the moment we have the code running for 30 seconds then stopping 
and logging the frame time into a csv file which will be parsed later in our python script files.



## If you would like more info check out the main raylib page or a separate tutorial on how to set up a raylib project.

Main Page: https://www.raylib.com/

Video Tutorial: https://www.youtube.com/watch?v=PaAcVk5jUd8
