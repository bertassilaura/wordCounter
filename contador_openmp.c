#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <sys/time.h>

int num_words = 30;
int qtd_word[30];
char keywords[50][100] = {
    "jesus",
    "deus",
    "temor",
    "senhor",
    "pastor",
    "não tema",
    "amigo",
    "montanha",
    "rei",
    "desculpa",
    "dia",
    "pecado",
    "tempo",
    "mora",
    "amo",
    "serpente",
    "homem",
    "ele",
    "vai",
    "decepcionado",
    "eu",
    "vida",
    "rosa",
    "livro",
    "interessante",
    "batismo",
    "mundo",
    "flor",
    "estrela",
    "noite",
};

int counter(char *line, char *word)
{

    int qtd = 0;
    int equals = 0;

    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == word[equals])
            ++equals;
        else
            equals = 0;
        if (equals == strlen(word))
        {
            ++qtd;
            equals = 0;
        }
    }
    return qtd;
}

void main()
{
    char *buffer = 0;
    long size;

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    FILE *file = fopen("/home/gbl/Projects/paralela/biblia-em-txt-lowercase.txt", "rb");

    if (file)
    {
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = (char *)malloc(size);
        if (buffer)
        {
            fread(buffer, 1, size, file);
        }
        fclose(file);
    }
    printf("%s\n", "1");

    if (buffer)
    {
        omp_set_num_threads(2);

#pragma omp parallel for

        for (int i = 0; i < num_words; i++)
            qtd_word[i] = counter(buffer, keywords[i]);

        for (int j = 0; j < num_words; j++)
            printf("%s - %d vezes.\n\n", keywords[j], qtd_word[j]);

        gettimeofday(&t2, NULL);
    }

    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);
    int min = t_total / 60;
    int h, m, s;

    h = (t_total / 3600);

    m = (t_total - (3600 * h)) / 60;

    s = (t_total - (3600 * h) - (m * 60));

    printf("H:M:S - %d:%d:%d\n", h, m, s);

    printf("Tempo em segundos: %f ms\n", t_total);
    printf("Tempo em minutos: %d min\n\n", min);
}