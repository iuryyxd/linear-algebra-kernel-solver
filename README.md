# Linear Algebra Kernel Solver

Uma implementação em C para encontrar a base do Kernel de qualquer matriz $m \times n$.

## Sobre o Projeto

Este algoritmo utiliza **Eliminação Gaussiana** para transformar a matriz em sua forma escalonada e aplica **Back-Substitution** para identificar as variáveis livres e gerar os vetores da base do Kernel.

O projeto foi desenvolvido para lidar com:

- Matrizes quadradas e retangulares.
- Casos de solução trivial (Kernel = {0}).

## Como Compilar e Rodar

Você precisará de um compilador C (como GCC).

```bash
# Compilar
gcc main.c -o kernel_solver

# Rodar
./kernel_solver
```
