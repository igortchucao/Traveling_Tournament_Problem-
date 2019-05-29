#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define TIMES 20


/*CRIAÇÃO DA MATRIZ DE JOGOS
*   - A matriz é contruida por Linha (mandante) e Coluna(visitante)
*   - A coluna TIMES + 1 representa o erro em caso de muitas viagens para um time determinado
*/
int jogos [TIMES][TIMES+2];
//int jogos;
int custos [TIMES][TIMES];

typedef struct{
    char *Mando;
    char *Vis;
    int ordem;
}Viagem;//[3][(TIMES * TIMES) + 1];

Viagem viagem[(TIMES * TIMES) - TIMES + 1];
//Viagem custo[(TIMES * TIMES) - TIMES + 1];

//Função que zera a matriz em caso de conclusão de rodada
void zerar_matriz()
{
    for(int i = 0; i < TIMES; i++)for(int j = 0; j < TIMES; j++)
    {
        //zera a matriz caso conclua todos os jogos da rodada
        if(jogos[i][j] == -1)jogos[i][j] = 0;

        //elimina os jgoos entre AxA
        if(i == j)jogos[i][j] = -1;
    }
}

//Tira as impossibilidades adicionada pelo Back-Tracking
void zerar_matriz_2()
{
    for(int i = 0; i < TIMES; i++)for(int j = 0; j < TIMES; j++)
    {
        if(jogos[i][j] == -2)jogos[i][j] = 0;
    }
}

/*Esta função mostra para o codigo um jogo antes que ele tentou e errou
*   - Aux é o numero do PRIMEIRO jogo da rodada que foi acusada de erro
*   - B é o primeiro jogo das rodadas de volta
*   - a função apaga os jogos que estao entre Aux e B, ou seja, os jogos adicionados pela rodada com erro
*   - alem disso, marca o primeiro jogo da rodada com "-2" para que a proxima rodada nao tente no mesmo jogo
*/
void back_tracking(int aux, int b)
{
    for(int i = 0; i < TIMES; i++)for(int j = 0; j < TIMES; j++)
    {
        //tira todos os "-1", que são as impossibilidades de uma rodada
        if(jogos[i][j] == -1)jogos[i][j] = 0;

        if(jogos[i][j] == aux)
        {
            jogos[i][j] = -2;
            jogos[j][i] = -2;
        }

        if((jogos[i][j] > aux)&&(jogos[i][j] <= b))
        {
            jogos[i][j] = 0;
            jogos[j][i] = 0;
        }

        if(i == j)jogos[i][j] = -1;
    }
}
/*Função que zera a propria rodada de erro
*   - zera o que foi feito na rodada
*   - noa faz nenhum tipo de marcação
*/
void zerar_rodada(int aux, int b)
{
    for(int i = 0; i < TIMES; i++)for(int j = 0; j < TIMES; j++)
    {
        if(jogos[i][j] == -1)jogos[i][j] = 0;

        if((jogos[i][j] >= aux)&&(jogos[i][j] <= b))
        {
            jogos[i][j] = 0;
            jogos[j][i] = 0;
        }

        if(i == j)jogos[i][j] = -1;
    }
}

/*Função heuristica preenche a matriz com os jogos possiveis:
* 1 - a função distribui jogos por rodadas.
* 2 - cada rodada tem n/2 jogos.
* 3 - até completar os n/2 jogos da rodada, a função nao permite que seja colocado um numero
*     na mesma linha ou coluna de outro jogo da mesma rodada.
* 3.1 - ao marcar um jogo na matriz, a função preenche a linha e a coluna do jogo marcado com "-1"
* 3.2 - a partir disso, a função se torna um Back-Traking até a ultima rodada. Caso caia em uma rodada
*       que nao pode ser completa, ele volta a rodada anterior e altera.
* 4 - Repete a função até que todas os primeiros jogos de IDA tenham sido preenchidos.
* 4.1 -  as rodadas tem o valor de (2n -2). Como a função segue somente os jogos de IDA, entao (2n -2)/2
*/
void heuristica(int rodada, int cont)
{
    int r1, r2, perm = 0;
    int g = 0;
    //função que avalia quantos espaços vazios existem a partir desta rodada
    for(int i = 0; i < TIMES; i++)for(int j = 0; j < TIMES; j++)if(jogos[i][j] == 0)perm++;

    if((rodada <= (TIMES - 1)) && (perm > 0) && (cont <= TIMES/2))
    {
        if(TIMES <= 12)
            {
            do
            {
                r1 = rand()%TIMES;
                r2 = rand()%TIMES;

            //evita que os jogos entre times iguais (AxA)
            }while((r1 == r2)||(jogos[r1][r2] != 0));
        }else
        {
            do
            {
                r1 = rand()%TIMES;
                r2 = rand()%TIMES;
            }while((r1 == r2)||(jogos[r1][r2] > 0));
        }

            //Os jogos seguem este padrão de CONT por RODADA e quantidades de times:
            jogos[r1][r2] = (rodada * cont) + ((rodada - 1) * ((TIMES/2) - cont));
            jogos[r2][r1] = ((rodada * cont) + ((rodada - 1) * ((TIMES/2) - cont))) + (pow(TIMES, 2) - TIMES)/2;
            //print_matriz();

                //preenche a linha e a coluna dos jogos de ida e volta com "-1"
                for(int i = 0; i < TIMES; i++)
                {
                    if(jogos[r1][i] <= 0)jogos[r1][i] = -1;
                    if(jogos[i][r1] <= 0)jogos[i][r1] = -1;
                    if(jogos[r2][i] <= 0)jogos[r2][i] = -1;
                    if(jogos[i][r2] <= 0)jogos[i][r2] = -1;
                }
                //print_matriz();

                //cont é uma variavel que conta quantos jogos tem por rodada. Se cont for igual a ultima rodada de IDA, entao para.
                if((cont == TIMES/2) && (rodada <= (TIMES - 1)))
                {
                    //volta para o cont = 1 da proxima rodada
                    cont = 1;
                    rodada++;
                    zerar_matriz();
                    heuristica(rodada, cont);
                }else{
                    cont++;
                    zerar_matriz_2();
                    heuristica(rodada, cont);
                }


    }else
    {
        if(rodada <= (TIMES - 1))
        {
            //a variavel A é o primeiro jogo da rodada que deu erro. Entao consideranado o CONT como 1, temos:
            int a = rodada + ((rodada - 1) * ((TIMES/2) - 1));

            //a variavel B é o ultimo jogo das rodadas de IDA
            int b = ((TIMES * TIMES) - TIMES)/2;
            zerar_rodada(a, b);

            //volta a rodada anterior
            rodada--;

            //atualiza o valor de A
            a = rodada + ((rodada - 1) * ((TIMES/2) - 1));
            back_tracking(a, b);
            cont = 1;
            heuristica(rodada, cont);
        }
    }
}

/*Função que analisa os mandos de Jogos
* 1 - Apos copiar a matriz, a função ordena por ordem dos jogos
* 2 - Se o jogo tem uma sequencia de 3 jogos em casa ou fora, gera um erro
*/
int verificar()
{
    int key1, j, d = 0;
    int k;
    //Insertion Sort
    for(int i = 0; i < TIMES; i++)
    {
        for (int k = 0; k < TIMES; k++)
        {
            key1 = jogos[i][k];
            j = k-1;

            while (j >= 0 && jogos[i][j] > key1)
            {
               jogos[i][j+1] = jogos[i][j];
               j--;
            }
            jogos[i][j+1] = key1;
        }
    }

    /*Com o vetor de jogos copiados para o vetor "viagem", podemos ordenar a matriz jogos
    * e analisar ela de formar ordenada:
    *   - Se temos mais de 3 jogos tanto fora quanto dentro de casa, fica acusado o erro "4/-4"
    *   - A analise de "Fora/Dentro" é feita se o numero for maior ou menor que ((TIMES * TIMES) - TIMES)/2
    *   - Essa conta é o valor do ultimo jogo da rodada de IDA
    */
    for(int i = 0; i < TIMES; i++)
    {
        jogos[i][TIMES] = 0;
        for (k = 1; k < TIMES; k++)
            if(i == k)k++;
        {
            if(jogos[i][TIMES] >= -3 && jogos[i][TIMES] <= 3)
            {
                if(jogos[i][k] > ((TIMES * TIMES) - TIMES)/2)
                {
                    if(jogos[i][TIMES] > 0)jogos[i][TIMES]++;
                    else jogos[i][TIMES] = 1;
                }else
                {
                    if(jogos[i][TIMES] < 0)jogos[i][TIMES]--;
                    else jogos[i][TIMES] = -1;
                }
            }else {
                /*int aux = jogos[i][k];
                jogos[i][k] = jogos[k][i];
                jogos[k][i] = aux;*/
                d++;
            }
        }
    }
    if(d > 0)return 0;
    else return 1;
}

//Função que faz o calculo da viagem dos times individualmente
void custo()
{
     //jogos[i][TIMES+1] = 0;
     for(int i = 0; i < TIMES; i++)
    {
        jogos[i][TIMES+1] = 0;
        for (int k = 1; k < TIMES; k++)
        {
            //estou em casa?
            //SIM
            if(jogos[i][k] <= ((TIMES * TIMES) - TIMES)/2)
            {
                //é meu primeiro jogo?
                //NAO
                if(jogos[i][TIMES + 1] > 0)
                {
                    //jogo passado é em casa ou nao?
                    //NAO
                    if(jogos[i][k-1] > ((TIMES * TIMES) - TIMES)/2)jogos[i][TIMES + 1] = jogos[i][TIMES + 1] + custos[i][k-1];
                }
            }else{
            //NAO
                //é meu primeiro jogo?
                //SIM
                if(jogos[i][TIMES + 1] == 0)jogos[i][TIMES + 1] = jogos[i][TIMES + 1] + custos[i][k];

                //NAO
                if(jogos[i][TIMES + 1] > 0)
                {
                    //jogo passado é em casa ou nao?
                    //SIM
                    if(jogos[i][k-1] <= ((TIMES * TIMES) - TIMES)/2)jogos[i][TIMES + 1] = jogos[i][TIMES + 1] + custos[i][k];
                    //NAO
                    else jogos[i][TIMES + 1] = jogos[i][TIMES + 1] + custos[k][k-1];
                }
            }
        }
    }
    for(int i = 0; i < TIMES; i++)
    {
        for (int k = 1; k < TIMES; k++)
        {
            //estou em casa?
            //SIM
            if(jogos[i][k] > ((TIMES * TIMES) - TIMES)/2)
            {
                //é meu primeiro jogo da volta?
                //SIM
                if(k == 1)
                {
                    //jogo da rodada passada é em casa ou nao?
                    //NAO
                    if(jogos[i][TIMES-1] > ((TIMES * TIMES) - TIMES)/2)jogos[i][TIMES + 1] = jogos[i][TIMES + 1] + custos[k][TIMES-1];
                }
                //NAO
                else
                {
                    //jogo passado é em casa ou nao?
                    //NAO
                    if(jogos[i][k-1] <= ((TIMES * TIMES) - TIMES)/2)jogos[i][TIMES + 1] = jogos[i][TIMES + 1] = jogos[i][TIMES + 1] + custos[k][k-1];
                }
            //NAO
            }else{
                //é meu primeiro jogo da volta?
                //SIM
                if(k == 1)
                {
                    //jogo da rodada passada é em casa ou nao?
                    //SIM
                    if(jogos[i][TIMES-1] <= ((TIMES * TIMES) - TIMES)/2)jogos[i][TIMES + 1] = jogos[i][TIMES + 1] + custos[i][k];
                    //NAO
                    else jogos[i][TIMES + 1] = jogos[i][TIMES + 1] + custos[k][TIMES-1];
                }
                //NAO
                else
                {
                    //jogo passado é em casa ou nao?
                    //SIM
                    if(jogos[i][k-1] > ((TIMES * TIMES) - TIMES)/2)jogos[i][TIMES + 1] = jogos[i][TIMES + 1] + custos[i][k];
                    //NAO
                    else jogos[i][TIMES + 1] = jogos[i][TIMES + 1] + custos[k][k-1];
                }
            }
        }
    }
}

/*Faz a tradução dos numeros para os char's dos Times
* 1 - Função composta juntamente com a função "tradutor"
* 1.1 - Esta função representa os times mandantes dos jogos
* 1.2 - Função "Tradutor" são os times visitantes
* 2 - Tabela feita pelas abreviações dos nomes dos times
*/
void vetor_print()
{
    for(int i = 0; i < TIMES; i++)for(int j = i + 1; j < TIMES; j++)
    {
        viagem[jogos[i][j]].ordem = jogos[i][j];
        viagem[jogos[j][i]].ordem = jogos[j][i];
        switch(i)
            {
            case 0:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "CAM";
                tradutor(j, i);
                break;
            case 1:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "CRU";
                tradutor(j, i);
                break;
            case 2:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "AME";
                tradutor(j, i);
                break;
            case 3:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "BOT";
                tradutor(j, i);
                break;
            case 4:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "CAP";
                tradutor(j, i);
                break;
            case 5:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "COR";
                tradutor(j, i);
                break;
            case 6:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "CEA";
                tradutor(j, i);
                break;
            case 7:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "FLA";
                tradutor(j, i);
                break;
            case 8:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "FLU";
                tradutor(j, i);
                break;
            case 9:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "GRE";
                tradutor(j, i);
                break;
            case 10:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "INT";
                tradutor(j, i);
                break;
            case 11:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "PAL";
                tradutor(j, i);
                break;
            case 12:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "PAR";
                tradutor(j, i);
                break;
            case 13:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "SAN";
                tradutor(j, i);
                break;
            case 14:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "SAO";
                tradutor(j, i);
                break;
            case 15:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "VAS";
                tradutor(j, i);
                break;
            case 16:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "VIT";
                tradutor(j, i);
                break;
            case 17:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "SPO";
                tradutor(j, i);
                break;
            case 18:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "CHA";
                tradutor(j, i);
                break;
            case 19:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "BAH";
                tradutor(j, i);
                break;
            }
    }
    //for(int i = 1; i <= (TIMES * TIMES) - TIMES; i++)if(viagem[i].ordem > 0)printf("\t %s X %s = %i\n", viagem[i].Mando, viagem[i].Vis, viagem[i].ordem);
}

//Função que compoe a função "vetor_print"
void tradutor(int i, int j)
{
    switch(i)
            {
            case 0:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "CAM";
                break;
            case 1:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "CRU";
                break;
            case 2:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "AME";
                break;
            case 3:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "BOT";
                break;
            case 4:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "CAP";
                break;
            case 5:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "COR";
                break;
            case 6:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "CEA";
                break;
            case 7:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "FLA";
                break;
            case 8:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "FLU";
                break;
            case 9:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "GRE";
                break;
            case 10:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "INT";
                break;
            case 11:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "PAL";
                break;
            case 12:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "PAR";
                break;
            case 13:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "SAN";
                break;
            case 14:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "SAO";
                break;
            case 15:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "VAS";
                break;
            case 16:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "VIT";
                break;
            case 17:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "SPO";
                break;
            case 18:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "CHA";
                break;
            case 19:
                viagem[jogos[i][j]].Mando = viagem[jogos[j][i]].Vis = "BAH";
                break;
            }
}

//Escreve o nome completo dos times para a representação do custo de viagem
void nomes_completos(int i)
{
    switch(i)
            {
            case 0:
                printf("Atletico Mineiro");
                break;
            case 1:
                 printf("Cruzeiro");
                break;
            case 2:
                printf("America Mineiro");
                break;
            case 3:
                 printf("Bota Fogo");
                break;
            case 4:
                 printf("Atletico Paranaese");
                break;
            case 5:
                 printf("Corinthians");
                break;
            case 6:
                 printf("Ceara");
                break;
            case 7:
                 printf("Flamengo");
                break;
            case 8:
                 printf("Fluminense");
                break;
            case 9:
                 printf("Gremio");
                break;
            case 10:
                 printf("Internacional");
                break;
            case 11:
                 printf("Palmeiras");
                break;
            case 12:
                 printf("Parana");
                break;
            case 13:
                 printf("Santos");
                break;
            case 14:
                 printf("Sao Paulo");
                break;
            case 15:
                 printf("Vasco");
                break;
            case 16:
                 printf("Vitoria");
                break;
            case 17:
                 printf("Sport");
                break;
            case 18:
                 printf("Chapecoense");
                break;
            case 19:
                 printf("Bahia");
                break;
            }
}

//Preenche a Matriz "custos[][]" com os valores de viagens
void preencher_matriz()
{
    custos[0][1] = custos[1][0] = 0;
    custos[0][2] = custos[2][0] = 0;
    custos[0][3] = custos[3][0] = 440;
    custos[0][4] = custos[4][0] = 440;
    custos[0][5] = custos[5][0] = 440;
    custos[0][6] = custos[6][0] = 440;
    custos[0][7] = custos[7][0] = 1000;
    custos[0][8] = custos[8][0] = 1000;
    custos[0][9] = custos[9][0] = 1719;
    custos[0][10] = custos[10][0] = 1719;
    custos[0][11] = custos[11][0] = 585;

    custos[1][2] = custos[2][1] = 0;
    custos[1][3] = custos[3][1] = 440;
    custos[1][4] = custos[4][1] = 440;
    custos[1][5] = custos[5][1] = 440;
    custos[1][6] = custos[6][1] = 440;
    custos[1][7] = custos[7][1] = 1000;
    custos[1][8] = custos[8][1] = 1000;
    custos[1][9] = custos[9][1] = 1719;
    custos[1][10] = custos[10][1] = 1719;
    custos[1][11] = custos[11][1] = 585;

    custos[2][3] = custos[3][2] = 440;
    custos[2][4] = custos[4][2] = 440;
    custos[2][5] = custos[5][2] = 440;
    custos[2][6] = custos[6][2] = 440;
    custos[2][7] = custos[7][2] = 1000;
    custos[2][8] = custos[8][2] = 1000;
    custos[2][9] = custos[9][2] = 1719;
    custos[2][10] = custos[10][2] = 1719;
    custos[2][11] = custos[11][2] = 585;

    custos[3][4] = custos[4][3] = 0;
    custos[3][5] = custos[5][3] = 0;
    custos[3][6] = custos[6][3] = 0;
    custos[3][7] = custos[7][3] = 849;
    custos[3][8] = custos[8][3] = 849;
    custos[3][9] = custos[9][3] = 1569;
    custos[3][10] = custos[10][3] = 1569;
    custos[3][11] = custos[11][3] = 434;

    custos[4][5] = custos[5][4] = 0;
    custos[4][6] = custos[6][4] = 0;
    custos[4][7] = custos[7][4] = 849;
    custos[4][8] = custos[8][4] = 849;
    custos[4][9] = custos[9][4] = 1569;
    custos[4][10] = custos[10][4] = 1569;
    custos[4][11] = custos[11][4] = 434;

    custos[5][6] = custos[6][5] = 0;
    custos[5][7] = custos[7][5] = 849;
    custos[5][8] = custos[8][5] = 849;
    custos[5][9] = custos[9][5] = 1569;
    custos[5][10] = custos[10][5] = 1569;
    custos[5][11] = custos[11][5] = 434;

    custos[6][7] = custos[7][6] = 849;
    custos[6][8] = custos[8][6] = 849;
    custos[6][9] = custos[9][6] = 1569;
    custos[6][10] = custos[10][6] = 1569;
    custos[6][11] = custos[11][6] = 434;

    custos[7][8] = custos[8][7] = 0;
    custos[7][9] = custos[9][7] = 744;
    custos[7][10] = custos[10][7] = 744;
    custos[7][11] = custos[11][7] = 424;

    custos[8][9] = custos[9][8] = 744;
    custos[8][10] = custos[10][8] = 744;
    custos[8][11] = custos[11][8] = 424;

    custos[9][10] = custos[10][9] = 0;
    custos[9][11] = custos[11][9] = 1143;

    custos[10][11] = custos[11][10] = 1143;
    if(TIMES > 12)
    {
    custos[0][12] = custos[12][0] = 585;
    custos[0][13] = custos[13][0] = 585;
    custos[0][14] = custos[14][0] = 643;
    custos[0][15] = custos[15][0] = 2423;
    custos[0][16] = custos[16][0] = 2423;
    custos[0][17] = custos[17][0] = 2136;
    custos[0][18] = custos[18][0] = 1469;
    custos[0][19] = custos[19][0] = 1471;

    custos[1][12] = custos[12][1] = 585;
    custos[1][13] = custos[13][1] = 585;
    custos[1][14] = custos[14][1] = 643;
    custos[1][15] = custos[15][1] = 2423;
    custos[1][16] = custos[16][1] = 2423;
    custos[1][17] = custos[17][1] = 2136;
    custos[1][18] = custos[18][1] = 1469;
    custos[1][19] = custos[19][1] = 1471;

    custos[2][12] = custos[12][2] = 585;
    custos[2][13] = custos[13][2] = 585;
    custos[2][14] = custos[14][2] = 643;
    custos[2][15] = custos[15][2] = 2423;
    custos[2][16] = custos[16][2] = 2423;
    custos[2][17] = custos[17][2] = 2136;
    custos[2][18] = custos[18][2] = 1469;
    custos[2][19] = custos[19][2] = 1471;

    custos[3][12] = custos[12][3] = 434;
    custos[3][13] = custos[13][3] = 434;
    custos[3][14] = custos[14][3] = 495;
    custos[3][15] = custos[15][3] = 2586;
    custos[3][16] = custos[16][3] = 2586;
    custos[3][17] = custos[17][3] = 2306;
    custos[3][18] = custos[18][3] = 1318;
    custos[3][19] = custos[19][3] = 1631;

    custos[4][12] = custos[12][4] = 434;
    custos[4][13] = custos[13][4] = 434;
    custos[4][14] = custos[14][4] = 495;
    custos[4][15] = custos[15][4] = 2586;
    custos[4][16] = custos[16][4] = 2586;
    custos[4][17] = custos[17][4] = 2306;
    custos[4][18] = custos[18][4] = 1318;
    custos[4][19] = custos[19][4] = 1631;

    custos[5][12] = custos[12][5] = 434;
    custos[5][13] = custos[13][5] = 434;
    custos[5][14] = custos[14][5] = 495;
    custos[5][15] = custos[15][5] = 2586;
    custos[5][16] = custos[16][5] = 2586;
    custos[5][17] = custos[17][5] = 2306;
    custos[5][18] = custos[18][5] = 1318;
    custos[5][19] = custos[19][5] = 1631;

    custos[6][12] = custos[12][6] = 434;
    custos[6][13] = custos[13][6] = 434;
    custos[6][14] = custos[14][6] = 495;
    custos[6][15] = custos[15][6] = 2586;
    custos[6][16] = custos[16][6] = 2586;
    custos[6][17] = custos[17][6] = 2306;
    custos[6][18] = custos[18][6] = 1318;
    custos[6][19] = custos[19][6] = 1631;

    custos[7][12] = custos[12][7] = 424;
    custos[7][13] = custos[13][7] = 424;
    custos[7][14] = custos[14][7] = 408;
    custos[7][15] = custos[15][7] = 3477;
    custos[7][16] = custos[16][7] = 3477;
    custos[7][17] = custos[17][7] = 3119;
    custos[7][18] = custos[18][7] = 468;
    custos[7][19] = custos[19][7] = 2443;

    custos[8][12] = custos[12][8] = 424;
    custos[8][13] = custos[13][8] = 424;
    custos[8][14] = custos[14][8] = 408;
    custos[8][15] = custos[15][8] = 3477;
    custos[8][16] = custos[16][8] = 3477;
    custos[8][17] = custos[17][8] = 3119;
    custos[8][18] = custos[18][8] = 468;
    custos[8][19] = custos[19][8] = 2443;

    custos[9][12] = custos[12][9] = 1143;
    custos[9][13] = custos[13][9] = 1143;
    custos[9][14] = custos[14][9] = 1128;
    custos[9][15] = custos[15][9] = 4196;
    custos[9][16] = custos[16][9] = 4196;
    custos[9][17] = custos[17][9] = 3838;
    custos[9][18] = custos[18][9] = 452;
    custos[9][19] = custos[19][9] = 3163;

    custos[10][12] = custos[12][10] = 1143;
    custos[10][13] = custos[13][10] = 1143;
    custos[10][14] = custos[14][10] = 1128;
    custos[10][15] = custos[15][10] = 4196;
    custos[10][16] = custos[16][10] = 4196;
    custos[10][17] = custos[17][10] = 3838;
    custos[10][18] = custos[18][10] = 452;
    custos[10][19] = custos[19][10] = 3163;

    custos[11][12] = custos[12][11] = 0;
    custos[11][13] = custos[13][11] = 0;
    custos[11][14] = custos[14][11] = 66;
    custos[11][15] = custos[15][11] = 3090;
    custos[11][16] = custos[16][11] = 3090;
    custos[11][17] = custos[17][11] = 2703;
    custos[11][18] = custos[18][11] = 884;
    custos[11][19] = custos[19][11] = 2028;

    custos[12][13] = custos[13][12] = 0;
    custos[12][14] = custos[14][12] = 66;
    custos[12][15] = custos[15][12] = 3090;
    custos[12][16] = custos[16][12] = 3090;
    custos[12][17] = custos[17][12] = 2703;
    custos[12][18] = custos[18][12] = 884;
    custos[12][19] = custos[19][12] = 2028;

    custos[13][14] = custos[14][13] = 66;
    custos[13][15] = custos[15][13] = 3090;
    custos[13][16] = custos[16][13] = 3090;
    custos[13][17] = custos[17][13] = 2703;
    custos[13][18] = custos[18][13] = 884;
    custos[13][19] = custos[19][13] = 2028;

    custos[14][15] = custos[15][14] = 3159;
    custos[14][16] = custos[16][14] = 3159;
    custos[14][17] = custos[17][14] = 2762;
    custos[14][18] = custos[18][14] = 884;
    custos[14][19] = custos[19][14] = 2087;

    custos[15][16] = custos[16][15] = 0;
    custos[15][17] = custos[17][15] = 782;
    custos[15][18] = custos[18][15] = 3872;
    custos[15][19] = custos[19][15] = 1189;

    custos[16][17] = custos[17][16] = 782;
    custos[16][18] = custos[18][16] = 3872;
    custos[16][19] = custos[19][16] = 1189;

    custos[17][18] = custos[18][17] = 3580;
    custos[17][19] = custos[19][17] = 806;

    custos[18][19] = custos[19][18] = 2912;
    }
}

int main()
{
    int custo_times[TIMES];
    srand(time(NULL));
    Viagem v[(TIMES * TIMES) - TIMES + 1];
    int a;
    int custototal = 100000000000000;
    int parcial;

    preencher_matriz();

    for (int i = 0; i < 1; i++)
    {
        /*Função "Do/While" faz com que as matrizes com erros de tabela sejam eliminadas e sorteadas novamente*/
        do{
            a = 1;
            heuristica(1, 1);
            vetor_print();
            a = verificar();
            custo();

                //zera a matriz "jogos[][]" em caso de erros
                if(a == 0)for(int i = 0; i < TIMES; i++)for(int j = 0; j < TIMES; j++)if(jogos[i][j] > 0)jogos[i][j] = 0;

        }while(a == 0);

        parcial = 0;

        for(int i = 0; i < TIMES; i++)
        {
            //calcula o valor de custo total
            parcial = parcial + jogos[i][TIMES + 1];
            for(int j = 0; j < TIMES; j++)if(jogos[i][j] > 0)jogos[i][j] = 0;
        }

        /*Caso a nova tabela seja melhor, ela substitui a tabela antiga*/
        if(parcial < custototal)
        {
            custototal = parcial;
            for(int i = 0; i < TIMES; i++)custo_times[i] = jogos[i][TIMES + 1];
            for(int i = 1; i <= (TIMES * TIMES) - TIMES; i++)if(viagem[i].ordem > 0)v[i] = viagem[i];
        }

    }
    printf("\t\t TABELA DE JOGOS\n");
    printf("--------------------------------------------------------------------\n");
    for(int i = 1; i <= (TIMES * TIMES) - TIMES; i = i + 4)if(v[i].ordem > 0){
            /*if(i == (((TIMES * TIMES) - TIMES)/2) + 1)printf("\n\t\t---JOGOS DA VOLTA---\n\n");
            printf("\t\t%s\t| X |\t%s\n", v[i].Mando, v[i].Vis);
            printf("\t\t\t-----\n");*/
            //for(int j = 0; j < 4; j++)if(i == (((TIMES * TIMES) - TIMES)/2) -2 || i == (((TIMES * TIMES) - TIMES)/2) + 2 || i == (((TIMES * TIMES) - TIMES)/2) -1 || i == (((TIMES * TIMES) - TIMES)/2) + 4)printf("\n\t\t---JOGOS DA VOLTA---\n\n");
            for(int j = 0; j < 4; j++)
            {
                if(i + j < 10)printf("%i   - |%s | X | %s|     ", (i + j), v[i + j].Mando, v[i + j].Vis);//printf("%s\t| X |\t%s\t\t%s\t| X |\t%s\t\t%s\t| X |  \t%s\t\t%s\t   | X |\t%s\n", v[i].Mando, v[i].Vis, v[i + 1].Mando, v[i + 1].Vis, v[i + 2].Mando, v[i + 2].Vis, v[i + 3].Mando, v[i + 3].Vis);
                else if(i + j < 100)printf("%i  - |%s | X | %s|     ", (i + j), v[i + j].Mando, v[i + j].Vis);//printf("%s\t| X |\t%s\t\t%s\t| X |\t%s\t\t%s\t| X |  \t%s\t\t%s\t   | X |\t%s\n", v[i].Mando, v[i].Vis, v[i + 1].Mando, v[i + 1].Vis, v[i + 2].Mando, v[i + 2].Vis, v[i + 3].Mando, v[i + 3].Vis);
                else if(i + j >= 100)printf("%i - |%s | X | %s|     ", (i + j), v[i + j].Mando, v[i + j].Vis);//printf("%s\t| X |\t%s\t\t%s\t| X |\t%s\t\t%s\t| X |  \t%s\t\t%s\t   | X |\t%s\n", v[i].Mando, v[i].Vis, v[i + 1].Mando, v[i + 1].Vis, v[i + 2].Mando, v[i + 2].Vis, v[i + 3].Mando, v[i + 3].Vis);

                //printf("|");
            }
            printf("\n");
            //printf("\t\t\t-----\n");
    }
    for(int i = 0; i < TIMES; i++)
    {
        printf("Viagem de ");
        nomes_completos(i);
        printf(" = %iKm\n", custo_times[i]);
    }
    printf("\nCUSTO TOTAL %iKm", custototal);
    return 0;
}
