// compilar com gcc -fopenmp contador_openmp.c -o count
// executar com ./count
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include <omp.h>
#define OMP_NUM_THREADS 16

int ocorrencias_palavras_chave[19];
char palavras_chave[50][100] = {"death", "that", "never", "soul", "divine", "time", "god", "inferno", "make", "escape"};
int qtd_palavras = 10;

char *texto(char *arquivo)
{
    FILE *file = fopen(arquivo, "rb");                                                // abre o arquivo
    fseek(file, 0, SEEK_END);                                                         // cursor avança 0, começa no final.
    int tam_texto = ftell(file);                                                      // tamanho do inicio do file até o cursor
    fseek(file, 0, SEEK_SET);                                                         // volta o cursor para o inicio do file
    unsigned char *data = (unsigned char *)malloc(tam_texto * sizeof(unsigned char)); // cria buffer para armazenar o conteúdo do arquivo
    fread(data, sizeof(unsigned char), tam_texto, file);                              // le o arquivo para data
    fclose(file);
    return data;
}

int num_ocorrencias(char *linha, char *palavra)
{
    int cont = 0;
    int igual = 0;

    for (int i = 0; i < strlen(linha); i++)
    {
        if (linha[i] == palavra[igual]) // começa na linha, posição 0 e na posição 0 da palavra
            ++igual; // se é igual, incrementa
        else
            igual = 0;
        if (igual == strlen(palavra)) // se igual for igual ao tamanho da palavra, incrementa contador
        {
            ++cont;
            igual = 0;
        }
    }
    return cont;
}

int main()
{

    struct timeval t1, t2;

    gettimeofday(&t1, NULL);

    char *text = texto("divinacomedia.txt");

#pragma omp parallel for num_threads(OMP_NUM_THREADS)
    for (int i = 0; i < qtd_palavras; i++)
    {
        ocorrencias_palavras_chave[i] = num_ocorrencias(text, palavras_chave[i]); // qtd de ocorrencias da palavra chave procurada
    }

    gettimeofday(&t2, NULL);
    for (int j = 0; j < qtd_palavras; j++)
    {
        printf("%s - %d vez(es).\n\n", palavras_chave[j], ocorrencias_palavras_chave[j]);
    }

    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);
    printf("tempo total = %f\n", t_total);

    return 0;
}