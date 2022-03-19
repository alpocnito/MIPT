```diff
- Works only with white themes
```

# Parallel calculations laboratory

## Theory
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

