#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void obradi(char *direktorijum)
{
    DIR *dp;
    if ((dp = opendir(direktorijum)) == NULL)
    {
        printf("Greska prilikom otvaranja direktorijuma.\n");
        exit(1);
    }

    struct dirent *dirp;
    struct stat statbuf;
    char path[256];

    while ((dirp = readdir(dp)) != NULL)
    {
        // Preskoci ., ..
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(path, direktorijum);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        int result;
        if ((result = stat(path, &statbuf)) == -1)
        {
            printf("Neuspesno citanje podataka o objektu: %s\n", dirp->d_name);
            continue;
        }

        if (S_ISDIR(statbuf.st_mode))
        {
            obradi(path);
        }
        else if (S_ISREG(statbuf.st_mode))
        {
            char *ime = dirp->d_name;
            int n = strlen(ime);

            // Proveri da li se zavrsava sa '.sh' (glup nacin al radi)
            if (n > 3 && ime[n - 1] == 'h' && ime[n - 2] == 's' && ime[n - 3] == '.')
            {
                int pid = fork();
                if (pid == 0)
                {
                    if (execlp("sh", "sh", path, NULL) < 0)
                        exit(1);
                }
                printf("Izvrsava se: %s\n", ime);
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Neodgovarajuci broj argumenata.\n");
        exit(1);
    }

    obradi(argv[1]);

    return 0;
}