# ParallelMatrixMultiplication
Parallel Matrix Multiplication using Processes or Threads in C

Calculates the multiplication sum of a matrix using either proccesses or threads.


COMPILING & EXECUTING
---------------------------

Processes:
<ol>
    <li>type "make" in the terminal under the subdirectory processes/src to compile, Note: make should be typed without quotes</li>
    <li>./pmms matrix_A matrix_B 3 2 4 where matrix_A is 3x2 and matrix_B is 2x4</li>  
    <li>other test cases can also be run similarly, i.e substitute test case files instead of matrix_A and matrix_B along with the new row          and column values for the test files provided. e.g ./pmms Test1_A Test1_B 2 2 2</li>
   <li>make clean to remove executable and object files</li>
   </ol>

Threads:
<ol>
    <li>type "make" in the terminal under the subdirectory threads/src to compile, Note: make should be typed without quotes</li>
    <li>./pmms matrix_A matrix_B 3 2 4 where matrix_A is 3x2 and matrix_B is 2x4</li> 
    <li>other test cases can also be run similarly, i.e substitute test case files instead of matrix_A and matrix_B along with the new row and column values for the test files provided. e.g ./pmms Test1_A Test1_B 2 2 2</li>
   <li>make clean to remove executable and object files</li>
   </ol>
      
##### Tested with Valgrind, no memory leaks present.


###### Copyright © Nikhil Gudhka. All Rights Reserved.
