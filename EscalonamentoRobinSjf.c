#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

/* ESCALONAMENTO DE PROCESSOS 
*
*Para leitura do arquivo e armazenamento dos valores corretos o arquivo entrada.txt precisa estar com a seguinte estrutura:
*
*  4; 
*  T1,0,8;
*  T2,1,4;
*  T3,2,9;
*  T4,3,5;
*
*/

struct entrada // struct entrada
{
      char tarefa[3];
      int inicio;
      int duracao;
};

int main()
{
      // ==================================================================================================
      // Variaveis

      struct entrada *ent; // declarando vetor de struct
      int i = 0, j = 0;
      int x, pos, aux2;
      int qnt_processos;
      int total = 0, total_sjf = 0;
      int contador = 0;
      int tempo_quantum;
      int tempo_espera = 0;
      int tempo_retorno = 0;
      int tempo_chegada[20], tempo_execucao[20], aux[20];
      int t_espera_sjf[20], t_retorno_sjf[20], posicao[20];
      float media_tempo_espera;
      float media_tempo_retorno;
      float m_tempo_espera_sjf;
      float m_tempo_retorno_sjf;

      // ==================================================================================================
      // Entrada dos valores

      FILE *arquivo;

      if ((arquivo = fopen("entrada.txt", "r")) == NULL) // abrindo e lendo arquivo entrada.txt
      {
            printf("Erro ao abrir o arquivo.\n");
      }
      else
      {
            fscanf(arquivo, "%d;", &qnt_processos); // lendo e armazenando valor

            ent = (struct entrada *)malloc(qnt_processos * sizeof(struct entrada)); // Alocando dinamicamente o tamanho do vetor da struct

            while (!feof(arquivo))
            {

                  fscanf(arquivo, "\n%[^,],%d,%d;\n", ent[i].tarefa, &ent[i].inicio, &ent[i].duracao); // lendo e armazenando valores

                  i++;
            }

            fclose(arquivo);
      }

      // ==================================================================================================
      // Passando e exibindo os valores

      printf("\nEntrada do numero total de processos:\t");
      printf("%d\n", qnt_processos);

      x = qnt_processos;

      for (i = 0; i < qnt_processos; i++)
      {
            printf("\nProcesso: [%s]\t", ent[i].tarefa);

            printf("\nTempo de Chegada:\t"); // Arrival Time
            tempo_chegada[i] = ent[i].inicio;
            printf("%d", ent[i].inicio);

            printf("\nTempo de Execucao:\t"); // Burst Time
            tempo_execucao[i] = ent[i].duracao;
            printf("%d\n", ent[i].duracao);

            aux[i] = tempo_execucao[i];
      }

      // ==================================================================================================
      // Escalonamento ROUND ROBIN

      printf("\nInforme qual o tempo de execucao para cada processo (Time Quantum):\t");
      scanf("%d", &tempo_quantum);

      printf("---------------------------------------------------------------------------------------------\n");
      printf("***** ROUND ROBIN *****\n");

      printf("\nProcesso ID\t\tTempo de Execucao\tTempo de Retorno\tTempo de Espera\n"); // Burst Time // Turnaround Time // Waiting Time

      for (total = 0, i = 0; x != 0;)
      {
            if (aux[i] <= tempo_quantum && aux[i] > 0)
            {
                  total = total + aux[i];
                  aux[i] = 0;
                  contador = 1;
            }
            else if (aux[i] > 0)
            {
                  aux[i] = aux[i] - tempo_quantum;
                  total = total + tempo_quantum;
            }
            if (aux[i] == 0 && contador == 1)
            {
                  x--;

                  printf("\nProcesso[%s]\t\t\t%d\t\t\t%d\t\t\t%d", ent[i].tarefa, tempo_execucao[i], total - tempo_chegada[i], total - tempo_chegada[i] - tempo_execucao[i]);

                  tempo_espera = tempo_espera + total - tempo_chegada[i] - tempo_execucao[i];
                  tempo_retorno = tempo_retorno + total - tempo_chegada[i];
                  contador = 0;
            }
            if (i == qnt_processos - 1)
            {
                  i = 0;
            }
            else if (tempo_chegada[i + 1] <= total)
            {
                  i++;
            }
            else
            {
                  i = 0;
            }
      }

      media_tempo_espera = (float)tempo_espera / qnt_processos;
      media_tempo_retorno = (float)tempo_retorno / qnt_processos;

      printf("\n\nMedia do Tempo de Espera:\t%f", media_tempo_espera);
      printf("\nMedia do Tempo de Retorno:\t%f\n", media_tempo_retorno); 

      // ==================================================================================================
      // Escalonamento Shortest Job First | Trabalho mais curto primeiro | Non-Preemptive

      printf("---------------------------------------------------------------------------------------------\n");
      printf("***** SHORTEST JOB FIRST *****\n");

      for (i = 0; i < qnt_processos; i++)
      {
            pos = i;
            for (j = i + 1; j < qnt_processos; j++)
            {
                  if (tempo_execucao[j] < tempo_execucao[pos])
                        pos = j;
            }

            aux2 = tempo_execucao[i];
            tempo_execucao[i] = tempo_execucao[pos];
            tempo_execucao[pos] = aux2;

            aux2 = posicao[i];
            posicao[i] = posicao[pos];
            posicao[pos] = aux2;
      }

      t_espera_sjf[0] = 0;

      for (i = 1; i < qnt_processos; i++)
      {
            t_espera_sjf[i] = 0;
            for (j = 0; j < i; j++)
                  t_espera_sjf[i] += tempo_execucao[j];

            total_sjf += t_espera_sjf[i];
      }

      m_tempo_espera_sjf = (float)total_sjf / (float)qnt_processos;
      total_sjf = 0;

      printf("\nProcesso ID\t\tTempo de Execucao\tTempo de Espera\t\tTempo de Retorno\n");

      for (i = 0; i < qnt_processos; i++)
      {
            t_retorno_sjf[i] = tempo_execucao[i] + t_espera_sjf[i];
            total_sjf += t_retorno_sjf[i];

            printf("\nProcesso[%s]\t\t\t%d\t\t\t%d\t\t\t%d", ent[i].tarefa, tempo_execucao[i], t_espera_sjf[i], t_retorno_sjf[i]);
      }

      m_tempo_retorno_sjf = (float)total_sjf / (float)qnt_processos;
      printf("\n\nMedia do Tempo de Espera:\t%f", m_tempo_espera_sjf);
      printf("\nMedia do Tempo de Retorno:\t%f\n", m_tempo_retorno_sjf);

      return 0;
}