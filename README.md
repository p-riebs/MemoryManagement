# Memory Management

## Author(s):

Parker Riebs

## Date:

4/26/2016

## Description:

Shows the amount of page faults that will occur in memory managment.
The given file is the data read/written to memory, and output
is shown for First-In-First-Out (FIFO), Optimal (Opt.),
Least-Recently-Used (LRU), Second-Chance LRU (SC),
Enhanced Second-Chance LRU (ESC) algorithms that handle pages in memory.
How many frames can also be specified for the algorthims, the default is
to show all frames (1-7).

## How to build the software

Use the make file to build the package, then run the package by:
$ ./pagefault <argument1> <argument2> <argument3>

## How to use the software

The software requires supplying a file of newline seperated page information.
For example, give a test file to the program:
$ ./pagefault given-tests/test1.txt
An optional command of "-f" followed by a number between 1-7 can also be
given. This specifies what the exact count of frames that must be computed
by this program. For example:
$ ./pagefault -f 3 given-tests/test1.txt
The above command will tell this program to only show computations of memory
management with three frames.

## How the software was tested

The software was tested through debugging with test cases of files, and 
an overall understanding of memory management algorithms.

## Test Suite

Additional testing files are located in "/tests/".
Test files are:
test1.txt - Simple test to check if only one page fault occurs across all tests.
test2.txt - Multiple character integer pages are checked
test3.txt - Checks interweaving read/write pages with multiple character integers.
test4.txt - Checks large amount of pages with interweaving read/write actions.
test5.txt - Checks for top line total of tests to be properly functional.

## Known bugs and problem areas

None that I know of.
