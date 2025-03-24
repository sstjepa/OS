#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define N 20

int main()
{
    srand(time(NULL));

    int paranZbir[2];
    if (pipe(paranZbir) < 0)
        return 1;
    int neparanZbir[2];
    if (pipe(neparanZbir) < 0)
        return 1;

    int pid1, pid2;

    if (pid1 = fork() != 0)
    {
        /* roditelj */

        close(paranZbir[0]);
        close(neparanZbir[0]);

        char niz[N];
        int suma = 0;
        for (int i = 0; i < N; i++)
        {
            niz[i] = rand() % 95 + 32;
            suma += niz[i];
        }
        printf("%d\n", suma);
        if (suma % 2 == 0)
            write(paranZbir[1], niz, sizeof(char) * N);
        else
            write(neparanZbir[1], niz, sizeof(char) * N);

        close(paranZbir[1]);
        close(neparanZbir[1]);
    }
    else if (pid2 = fork() != 0)
    {
        /* proces 2 */

        close(neparanZbir[0]);
        close(neparanZbir[1]);
        close(paranZbir[1]);

        char niz[N];
        if (read(paranZbir[0], niz, sizeof(char) * N))
        {
            FILE *f = fopen("KARAKTERI.txt", "w");
            for (int i = 0; i < N; i++)
                fprintf(f, "%c", niz[i]);
            fprintf(f, "\n");
            fclose(f);

            close(paranZbir[0]);
        }
        exit(0);
    }
    else
    {
        /* proces 3 */

        close(paranZbir[0]);
        close(paranZbir[1]);
        close(neparanZbir[1]);

        char niz[N];
        if (read(neparanZbir[0], niz, sizeof(char) * N))
        {
            for (int i = 0; i < N; i++)
                printf("%c", niz[i]);
            printf("\n");

            close(neparanZbir[0]);
        }
        exit(0);
    }
    return 0;
}