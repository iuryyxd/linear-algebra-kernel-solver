
#include <stdio.h>
#include <stdbool.h>

#define real double

int descerLinhas(int numLinhas, int numColunas, real *matriz, int linhaAtual, int colunaAtual)
{
    for (int i = linhaAtual; i < numLinhas; i++)
    {
        if (matriz[i * numColunas + colunaAtual] != 0)
        {
            return i;
        }
    }

    return -1;
}

void trocarLinhas(int numColunas, real *matriz, int linha1, int linha2)
{
    for (int i = 0; i < numColunas; i++)
    {
        real temp = matriz[linha1 * numColunas + i];
        matriz[linha1 * numColunas + i] = matriz[linha2 * numColunas + i];
        matriz[linha2 * numColunas + i] = temp;
    }
}

void escalonar(int numLinhas, int numColunas, real *matriz, int linhaAtual, bool *variaveis)
{
    bool temPivo = false;
    int pivoX = -1;
    int pivoY = -1;

    int j = linhaAtual;
    while (j < numColunas && !temPivo)
    {
        int indice = descerLinhas(numLinhas, numColunas, matriz, linhaAtual, j);
        if (indice != -1)
        {
            if (indice != linhaAtual)
                trocarLinhas(numColunas, matriz, linhaAtual, indice);
            pivoX = linhaAtual;
            pivoY = j;
            variaveis[j] = true;
            temPivo = true;
        }
        else
        {
            j++;
        }
    }

    if (temPivo)
    {
        for (int i = linhaAtual + 1; i < numLinhas; i++)
        {
            real fator = matriz[i * numColunas + j] / matriz[pivoX * numColunas + pivoY];

            for (int k = j; k < numColunas; k++)
            {
                matriz[i * numColunas + k] = matriz[i * numColunas + k] - (fator * matriz[pivoX * numColunas + k]);
            }
        }
    }
}

int Ker(real *matrizEntrada, real *baseKernel, int numLinhas, int numColunas)
{
    int originalNumLinhas = numLinhas;
    if (numLinhas < numColunas)
        numLinhas = numColunas;

    real matriz[numLinhas * numColunas];
    for (int i = 0; i < numLinhas * numColunas; i++)
    {
        matriz[i] = 0.0;
    }

    for (int i = 0; i < originalNumLinhas; i++)
    {
        for (int j = 0; j < numColunas; j++)
        {
            matriz[i * numColunas + j] = matrizEntrada[i * numColunas + j];
        }
    }

    // Array para indicar quais são as variáveis livres ou presas
    bool variaveis[numColunas];
    for (int i = 0; i < numColunas; i++)
    {
        variaveis[i] = false;
    }

    for (int i = 0; i < numLinhas; i++)
    {
        escalonar(numLinhas, numColunas, matriz, i, variaveis);
    }

    // Achar variáveis livres
    int indicesVarLivres[numColunas];
    int countVarLivres = 0;
    for (int i = 0; i < numColunas; i++)
    {
        if (!variaveis[i])
        {
            indicesVarLivres[countVarLivres] = i;
            countVarLivres++;
        }
    }

    // Achar as possiveis bases para o Kernel
    real vetorBase[numColunas * countVarLivres];
    for (int i = 0; i < numColunas * countVarLivres; i++)
    {
        vetorBase[i] = 0.0;
    }

    if (countVarLivres > 0)
    {

        real coeficientes[numColunas];
        for (int j = 0; j < numColunas; j++)
        {
            coeficientes[j] = 0.0;
        }

        for (int i = 0; i < countVarLivres; i++)
        {
            coeficientes[indicesVarLivres[i]] = 1.0;
            for (int j = numLinhas - 1; j >= 0; j--)
            {
                bool temPivo = false;
                int pivoY = -1;

                for (int k = 0; k < numColunas; k++)
                {
                    if (matriz[j * numColunas + k] != 0)
                    {
                        temPivo = true;
                        pivoY = k;
                        break;
                    }
                }

                if (temPivo)
                {
                    real soma = 0.0;
                    for (int k = pivoY + 1; k < numColunas; k++)
                    {
                        soma += matriz[j * numColunas + k] * coeficientes[k];
                    }
                    coeficientes[pivoY] = -soma / matriz[j * numColunas + pivoY];
                }
            }
            for (int j = 0; j < numColunas; j++)
            {
                vetorBase[i * numColunas + j] = coeficientes[j];
            }
            coeficientes[indicesVarLivres[i]] = 0.0;
        }
    }

    // Transformar a matriz para um array e coloca na variável baseKernel
    int vetorBaseColuna = countVarLivres == 0 ? 1 : countVarLivres;
    for (int i = 0; i < vetorBaseColuna; i++)
    {
        for (int j = 0; j < numColunas; j++)
        {
            baseKernel[i * numColunas + j] = vetorBase[i * numColunas + j];
        }
    }

    return countVarLivres; // Dimensão do Kernel é o número de variáveis livres
}

int main()
{
    int numLinhas, numColunas;
    printf("Digite o numero de linhas da matriz:\n");
    scanf("%d", &numLinhas);
    printf("Digite o numero de colunas da matriz:\n");
    scanf("%d", &numColunas);

    real matrizEntrada[numLinhas * numColunas];

    printf("Digite a matriz (%d linhas x %d colunas),\n", numLinhas, numColunas);
    printf("informando os %d valores em ordem, separados por espacos/linhas:\n", numLinhas * numColunas);
    for (int i = 0; i < numLinhas; i++)
    {
        for (int j = 0; j < numColunas; j++)
        {
            scanf("%lf", &matrizEntrada[i * numColunas + j]);
        }
    }

    real baseKernel[numColunas * numColunas];
    int dim = Ker(matrizEntrada, baseKernel, numLinhas, numColunas);

    printf("Dimensao do Kernel: %d\n", dim);
    printf("Base(s) do Kernel:\n");
    for (int i = 0; i < (dim == 0 ? 1 : dim) * numColunas; i++)
    {
        real valor = baseKernel[i];
        if (valor > -0.000001 && valor < 0.000001)
            valor = 0.000000;

        if (i % numColunas == 0)
            printf("( ");
        printf("%f ", valor);
        if ((i + 1) % numColunas == 0)
            printf(")\n");
    }

    return 0;
}