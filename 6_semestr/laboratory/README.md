```diff
- Works only with white themes
```

# MPI laboratory

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

<img src="https://render.githubusercontent.com/render/math?math=\logleftrightarrow \uparrow">

<img src="https://render.githubusercontent.com/render/math?math=\frac{u_m^{k \dotplus 1} - u_m^{k}}{\tau} \dotplus \frac{u_m^{k} - u_{m - 1}^{k}}{h} = f_m^k, k = 0, ..., K - 1, m = 0, ..., M">
