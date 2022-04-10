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
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

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
      int permanecem, pos, t_aux;
      int qnt_processos;
      int tempo = 0, tempo_sjf = 0;
      int flag = 0;
      int tempo_quantum;
      int tempo_espera = 0;
      int tempo_retorno = 0;
      int tempo_chegada[20], tempo_surto[20], temp_aux[20];
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

      permanecem = qnt_processos;

      for (i = 0; i < qnt_processos; i++)
      {
            printf("\nProcesso: [%s]\t", ent[i].tarefa);

            printf("\nChegada:\t"); // Arrival Time
            tempo_chegada[i] = ent[i].inicio;
            printf("%d", ent[i].inicio);

            printf("\nTempo Surto:\t"); // Burst Time
            tempo_surto[i] = ent[i].duracao;
            printf("%d\n", ent[i].duracao);

            temp_aux[i] = tempo_surto[i];
      }

      // ==================================================================================================
      // Escalonamento ROUND ROBIN

      printf("\nInforme qual o tempo de execucao para cada processo (Time Quantum):\t");
      scanf("%d", &tempo_quantum);

      printf("---------------------------------------------------------------------------------------------\n");
      printf("***** ROUND ROBIN *****\n");

      printf("\nProcesso ID\t\tTempo de surto\t\tTempo de Retorno\tTempo de Espera\n"); // Burst Time // Turnaround Time // Waiting Time

      for (tempo = 0, i = 0; permanecem != 0;)
      {
            if (temp_aux[i] <= tempo_quantum && temp_aux[i] > 0)
            {
                  tempo += temp_aux[i];
                  temp_aux[i] = 0;
                  
                  flag = 1;
            }
            else if (temp_aux[i] > 0)
            {
                  temp_aux[i] = temp_aux[i] - tempo_quantum;
                  tempo += tempo_quantum;
            }
            if (temp_aux[i] == 0 && flag == 1)
            {
                  permanecem--;

                  printf("\nProcesso[%s]\t\t\t%d\t\t\t%d\t\t\t%d", ent[i].tarefa, tempo_surto[i], tempo - tempo_chegada[i], tempo - tempo_chegada[i] - tempo_surto[i]);

                  tempo_espera += tempo - tempo_chegada[i] - tempo_surto[i];
                  tempo_retorno += tempo - tempo_chegada[i];
                  
                  flag = 0;
            }
            if (i == qnt_processos - 1)
            {
                  i = 0;
            }
            else if (tempo_chegada[i + 1] <= tempo)
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

      printf("\n\nTempo Medio de Espera:\t%f", media_tempo_espera);
      printf("\nTempo Medio de Retorno:\t%f\n", media_tempo_retorno); 

      // ==================================================================================================
      // Escalonamento Shortest Job First | Trabalho mais curto primeiro | Non-Preemptive

      printf("---------------------------------------------------------------------------------------------\n");
      printf("***** SHORTEST JOB FIRST *****\n");

      for (i = 0; i < qnt_processos; i++)
      {
            pos = i;
            for (j = i + 1; j < qnt_processos; j++)
            {
                  if (tempo_surto[j] < tempo_surto[pos])
                        pos = j;
            }

            t_aux = tempo_surto[i];
            tempo_surto[i] = tempo_surto[pos];
            tempo_surto[pos] = t_aux;

            t_aux = posicao[i];
            posicao[i] = posicao[pos];
            posicao[pos] = t_aux;
      }

      t_espera_sjf[0] = 0;

      for (i = 1; i < qnt_processos; i++)
      {
            t_espera_sjf[i] = 0;
            for (j = 0; j < i; j++)
                  t_espera_sjf[i] += tempo_surto[j];

            tempo_sjf += t_espera_sjf[i];
      }

      m_tempo_espera_sjf = (float)tempo_sjf / qnt_processos;

      tempo_sjf = 0;

      printf("\nProcesso ID\t\tTempo de surto\t\tTempo de Espera\t\tTempo de Retorno\n");

      for (i = 0; i < qnt_processos; i++)
      {
            t_retorno_sjf[i] = tempo_surto[i] + t_espera_sjf[i];
            tempo_sjf += t_retorno_sjf[i];

            printf("\nProcesso[%s]\t\t\t%d\t\t\t%d\t\t\t%d", ent[i].tarefa, tempo_surto[i], t_espera_sjf[i], t_retorno_sjf[i]);
      }

      m_tempo_retorno_sjf = (float)tempo_sjf / qnt_processos;
      
      printf("\n\nTempo Medio de Espera:\t%f", m_tempo_espera_sjf);
      printf("\nTempo Medio de Retorno:\t%f\n", m_tempo_retorno_sjf);

      return 0;
}