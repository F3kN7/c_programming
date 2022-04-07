/* ::: Multiplicacao de Matrizes Quadradas :::
*
* Teste do tempo de execucao de diferentes abordagens de acesso na multiplicacao das matrizes
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*void SalvaMatriz(int Altura, int Largura, int *M, char *Titulo, char *NomeArquivo)
{
    int L, C; // Linha e Coluna

    FILE *fp = fopen(NomeArquivo, "w");
    if (fp == NULL)
    {
        printf("\n::: Erro abrindo arquivo '%s''!\n", NomeArquivo);
        exit(EXIT_FAILURE);
    }

    fprintf(fp, " %s \n", Titulo);
    for (L = 0; L < Altura; L++)
    {
        for (C = 0; C < Largura; C++)
        {
            fprintf(fp, " %2d ", M[L * Altura + C]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}*/

void SalvaMatriz(int Coluna, int Linha, char *Titulo, char *NomeArquivo, double tempoProcessamento)
{

    FILE *fp = fopen(NomeArquivo, "a");
    if (fp == NULL)
    {
        printf("\n::: Erro abrindo arquivo '%s''!\n", NomeArquivo);
        exit(EXIT_FAILURE);
    }

    fprintf(fp, " %s \n", Titulo);
    fprintf(fp, "\n::: Matrix Multiplication \n");
    fprintf(fp, "::: Width: %d | Height: %d | Total Size: %d \n", Coluna, Linha, Coluna * Linha);
    fprintf(fp, "\nAll done! Tempo de processamento: %f segundos.\n", tempoProcessamento);
    fprintf(fp, "___________________________________________________________________");
    fprintf(fp, "\n");

    fclose(fp);
}

void MostraMatrizA(int Altura, int Largura, int *M, char *Titulo)
{
    int L, C; // Linha e Coluna

    printf(" %s \n", Titulo);
    for (L = 0; L < Altura; L++)
    {
        for (C = 0; C < Largura; C++)
        {
            printf(" %2d ", M[L * Largura + C]); // Usando a abordagem A para teste -> M[linha, coluna] -> V[ linha * largura + coluna ]
        }
        printf("\n");
    }
}

void MostraMatrizB(int Altura, int Largura, int *M, char *Titulo)
{
    int L, C; // Linha e Coluna

    printf(" %s \n", Titulo);
    for (L = 0; L < Altura; L++)
    {
        for (C = 0; C < Largura; C++)
        {
            printf(" %2d ", M[C * Altura + L]); // Usando a abordagem B para o teste -> M[linha, coluna] -> V[ coluna * altura + linha ]
        }
        printf("\n");
    }
}

void PreencheMatrizA(int Altura, int Largura, int *M, int Valor)
{
    int L, C; // Linha e Coluna

    for (L = 0; L < Altura; L++)
    {
        for (C = 0; C < Largura; C++)
        {
            M[L * Largura + C] = Valor; // Usando a abordagem A para o teste -> M[linha, coluna] -> V[ linha * largura + coluna ]
        }
    }
}

void PreencheMatrizB(int Altura, int Largura, int *M, int Valor)
{
    int L, C; // Linha e Coluna

    for (L = 0; L < Altura; L++)
    {
        for (C = 0; C < Largura; C++)
        {
            M[C * Altura + L] = Valor; // Usando a abordagem B para o teste -> M[linha, coluna] -> V[ coluna * altura + linha ]
        }
    }
}

int main()
{
    int N;
    int inicial;
    int final;
    int intervalo;
    char opcao;

    printf("\nQual a dimensao inicial da matriz quadrada? ");
    scanf("%d", &inicial);
    printf("\nQual a dimensao final da matriz quadrada? ");
    scanf("%d", &final);
    printf("\nInforme o intervalo: ");
    scanf("%d", &intervalo);

    fflush(stdin);
    printf("\nQual abordagem deseja testar?\n\n (A) = M[linha, coluna] -> V[ linha * largura + coluna ]\n (B) = M[linha, coluna] -> V[ coluna * altura + linha ] : ");
    scanf("%c", &opcao);

    switch (opcao)
    {
    case 'A':

        for (N = inicial; N <= final; N += intervalo)
        {

            int *Ma = (int *)malloc(sizeof(int) * N * N);
            if (Ma == NULL)
            {
                printf("\nErro alocando memoria! \n");
                exit(EXIT_FAILURE);
            }

            int *Mb = (int *)malloc(sizeof(int) * N * N);
            if (Mb == NULL)
            {
                printf("\nErro alocando memoria! \n");
                exit(EXIT_FAILURE);
            }

            int *Mc = (int *)malloc(sizeof(int) * N * N);
            if (Mc == NULL)
            {
             printf("\nErro alocando memoria! \n");
             exit(EXIT_FAILURE);
            }

            /*int *Mc = (int *)calloc(N * N, sizeof(int));
            if (Mc == NULL)
            {
                printf("\nErro alocando memoria! \n");  //>>> usando a funcao calloc para alocar a matriz C ja inicializada com 0
                exit(EXIT_FAILURE);
            }*/

            printf("\n::: Matrix Multiplication \n");
            printf("::: Width: %d | Height: %d | Total Size: %d \n", N, N, N * N);
            printf("::: Preenchendo as matrizes... ");

            PreencheMatrizA(N, N, Ma, 1);
            PreencheMatrizA(N, N, Mb, 3);
            PreencheMatrizB(N, N, Mc, 0);

            printf("Done! Now calculating...\n");
            // MostraMatrizA(N, N, Ma, " Matriz A ");
            // MostraMatrizA(N, N, Mb, " Matriz B ");
            // MostraMatrizA(N, N, Mc, " Matriz C ");

            int L, C, K;

            // conta o tempo de processamento
            clock_t tempo;
            tempo = clock();

            // C = A . B
            for (L = 0; L < N; L++)
            {
                for (C = 0; C < N; C++)
                {
                    for (K = 0; K < N; K++)
                    {
                        Mc[L * N + C] += Ma[L * N + K] * Mb[K * N + C]; // Usando a abordagem A para o teste -> M[linha, coluna] -> V[ linha * largura + coluna ]
                    }
                }
            }

            clock_t final = clock();
            double tempoProcessamento = ((double)(final - tempo)) / (double)(CLOCKS_PER_SEC);

            // printf("\nResultado da multiplicacao:\n");
            // MostraMatrizA(N, N, Mc, " Matriz C ");

            printf("\nAll done! Tempo de processamento: %f segundos.\n", tempoProcessamento);
            printf("___________________________________________________________________");

            SalvaMatriz(N, N, "Abordagem A", "MultMatriz.txt", tempoProcessamento);
        }

        break;

    case 'B':

        for (N = inicial; N <= final; N += intervalo)
        {

            int *Ma = (int *)malloc(sizeof(int) * N * N);
            if (Ma == NULL)
            {
                printf("\nErro alocando memoria! \n");
                exit(EXIT_FAILURE);
            }

            int *Mb = (int *)malloc(sizeof(int) * N * N);
            if (Mb == NULL)
            {
                printf("\nErro alocando memoria! \n");
                exit(EXIT_FAILURE);
            }

            int *Mc = (int *)malloc(sizeof(int) * N * N);
            if (Mc == NULL)
            {
                printf("\nErro alocando memoria! \n");
                exit(EXIT_FAILURE);
            }

            /*int *Mc = (int *)calloc(N * N, sizeof(int));
            if (Mc == NULL)
            {
                printf("\nErro alocando memoria! \n");  //>>> usando a funcao calloc para alocar a matriz C ja inicializada com 0
                exit(EXIT_FAILURE);
            }*/

            printf("\n::: Matrix Multiplication \n");
            printf("::: Width: %d | Height: %d | Total Size: %d \n", N, N, N * N);
            printf("::: Preenchendo as matrizes... ");

            PreencheMatrizB(N, N, Ma, 1);
            PreencheMatrizB(N, N, Mb, 3);
            PreencheMatrizB(N, N, Mc, 0);

            printf("Done! Now calculating...\n");
            // MostraMatrizB(N, N, Ma, " Matriz A ");
            // MostraMatrizB(N, N, Mb, " Matriz B ");
            // MostraMatrizB(N, N, Mc, " Matriz C ");

            int L, C, K;

            // conta o tempo de processamento
            clock_t tempo;
            tempo = clock();

            // C = A . B
            for (L = 0; L < N; L++)
            {
                for (C = 0; C < N; C++)
                {
                    for (K = 0; K < N; K++)
                    {
                        Mc[C * N + L] += Ma[C * N + K] * Mb[K * N + L]; //Usando a abordagem B para o teste -> M[linha, coluna] -> V[ coluna * altura + linha ]
                    }
                }
            }

            clock_t final = clock();
            double tempoProcessamento = ((double)(final - tempo)) / (double)(CLOCKS_PER_SEC);

            printf("\nResultado da multiplicacao:\n");
            // MostraMatrizB(N, N, Mc, " Matriz C ");

            printf("\nAll done! Tempo de processamento: %f segundos.\n", tempoProcessamento);
            printf("___________________________________________________________________");

            SalvaMatriz(N, N, "Abordagem B", "MultMatriz.txt", tempoProcessamento);
        }

        break;

    default:
        printf("\n###- Digite uma letra valida -###");
    }

    return 0;
}