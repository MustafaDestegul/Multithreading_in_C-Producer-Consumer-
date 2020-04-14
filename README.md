# Multithreading_in_C-Producer-Consumer-
Generating random numbers by producer threads and storing in Queue, Consumer Threads take numbers from Queue and find the divisors of the number. 

In this project, a multithreading example is implemented and tested on Linux ubuntu 18.04. As the development software "C" is used.

Generator Thread
The program will create one generator thread. This thread will generate random positive integers and insert them to the queue. The quantity of the numbers and their possible range is be decided by the command-line arguments. For example , ./EE442_HW1 -t 4 -q 3 -r 15 -m 1000. t,q,r,m will be described below.

Worker Threads 
These threads remove a number from the queue. They will sleep for a random amount of time (0.1 to 1 second with increments of 0.1 seconds). Finally, they will find the divisors of the numbers they removed and print the result. Then, they will start over until the generator finishes working. A result is as follows: 
Thread ID: 3059108672, Number: 96, Divisors: 1 2 3 4 6 8 12 16 24 32 48 96

Command-line arguments 
The program should use four optional arguments to change parameters: 
-t: Number of worker threads (default 5)  -q: The maximum size of the queue, which is shared by the threads (default 10)  -r: The amount of the random numbers (default 20)  -m: The possible range of the random numbers (default 100)

Example Linux commands to compile and run the software.

Compile   : gcc EE442_HW1.c -o EE442_HW1 -pthread

Run       : ./EE442_HW1 -t 4 -q 3 -r 15 -m 1000


Unfortunatelly, the codes will be uploaded after May. , which is the due date of the hw :)
