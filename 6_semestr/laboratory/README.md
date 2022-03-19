```diff
- Works only with white themes
```

# Parallel calculations laboratory

## Task
The goal of this program is to solve differential equation:

<img src="https://render.githubusercontent.com/render/math?math=\frac{\partial u(t, x)}{\partial t} \dotplus a\cdot \frac{\partial u(t, x)}{\partial x} = f(t,x)">
<img src="https://render.githubusercontent.com/render/math?math=u(0,x) = \varphi (x), 0 \leqslant x \leqslant X">
<img src="https://render.githubusercontent.com/render/math?math=u(t,0) = \psi (t), 0 \leqslant t \leqslant T">

## Method of soultion

Function u(t,x) is considered in:
<img src="https://render.githubusercontent.com/render/math?math=t = k\tau, 0 \leqslant k \leqslant K">
<img src="https://render.githubusercontent.com/render/math?math=x = mh, 0 \leqslant m \leqslant M">

In order to calculate function we will use difference schemes

1. Explicit left corner

![1](images/1.png)

Formula: <img src="https://render.githubusercontent.com/render/math?math=\frac{u_m^{k \dotplus 1} - u_m^{k}}{\tau} \dotplus \frac{u_m^{k} - u_{m - 1}^{k}}{h} = f_m^k, k = 0, ..., K - 1, m = 0, ..., M">

## Programming
This equation will be solved using MPI library.

Firstly, let's calculate the time is take to send a message from one processor to another:
```
make dopusk
make dopusk_run
```
For my computer it is: ```Time: 0.000175648```

Secondly, let's look at the matrix that the program calculates:
```
make 
make run np=1 K=5 M=5
```
It will output this matrix:
```
            0     1     2     3     4     5
      ------------------------------------------
0     |     0     4     2e+01 4e+01 6e+01 1e+02
      |
1     |     -2    0     4     2e+01 4e+01 6e+01
      |
2     |     2     -3    -3    8     2e+01 3e+01
      |
3     |     -1    -1    -8    5     7     9
      |
4     |     -0.6  -6    -9    3     3     -3
      |
5     |     2     -7    -2e+016     0.8   -1e+01
      |


Time: 0.000115267
```
- np - Number of process 
- K - Height of the matrix
- M - Widht of the matrix

Let's look at this matrix with one more processor:
```
make run np=2 K=5 M=5
```
Output:
```
            0     1     2     3     4     5
      ------------------------------------------
0     |     0     4     2e+01 4e+01 6e+01 1e+02

1     |     -2    0     4     2e+01 4e+01 6e+01
      |
2     |     2     -3    -3    8     2e+01 3e+01
      |
3     |     -1    -1    -8    5     7     9
      |
4     |     -0.6  -6    -9    3     3     -3
      |
5     |     2     -7    -2e+016     0.8   -1e+01
      |


Time: 0.00134599
      |                                          
```
Output is the same except for the sequence of style characters: | 

In order to disable output we have to comment ```#define PRINT``` line in __main.cpp__. After commenting this line we can measure time needed to calculate big matrices.

## Effectiveness

- <img src="https://render.githubusercontent.com/render/math?math=T_1"> time it takes for one processor to complete a job
- <img src="https://render.githubusercontent.com/render/math?math=T_p"> time it takes for <img src="https://render.githubusercontent.com/render/math?math=p"> processors to complete a job





