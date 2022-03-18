```diff
- Works only with white themes
```
Because of github markdown features:
<img src="https://render.githubusercontent.com/render/math?math=\pm"> is +
<img src="https://render.githubusercontent.com/render/math?math=\mp"> is -

# MPI laboratory

## Theory
The goal of this program is to solve differential equation:

<img src="https://render.githubusercontent.com/render/math?math=\frac{\partial u(t, x)}{\partial t} \pm a\cdot \frac{\partial u(t, x)}{\partial x} = f(t,x)">
<img src="https://render.githubusercontent.com/render/math?math=u(0,x) = \varphi (x), 0 \leqslant x \leqslant X">
<img src="https://render.githubusercontent.com/render/math?math=u(t,o) = \psi (t), 0 \leqslant t \leqslant T">


## Method of soultion

Function u(t,x) is considered in:
<img src="https://render.githubusercontent.com/render/math?math=t = k\tau, 0 \leqslant k \leqslant K">
<img src="https://render.githubusercontent.com/render/math?math=x = mh, 0 \leqslant m \leqslant M">

In order to calculate function we will use difference schemes

1. <img src="https://render.githubusercontent.com/render/math?math=u_m^{k \pm 1}">
