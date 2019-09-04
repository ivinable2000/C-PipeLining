#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int num = argc/5;
    pid_t p[num];
    int pipes[num - 1][2];
    char x;
    if(num > 1){
        for(int i = 0; i < num; i++){
             if (i < num - 1) {
                 if(pipe(pipes[i]) == -1){
                     perror("ERROR: Pipe could not be made");
                     return 1;
                 }
            }
            if ((p[i] = fork()) < 0)
            {
                perror("ERROR: Could not fork");
            }
            else if (p[i] == 0)
            {
                if(i == num - 1){
                    dup2(pipes[i-1][0], 0);
                    close(pipes[i-1][0]);
                    execlp("./biquad", "biquad", argv[(5*i) + 1], argv[(5*i) + 2], argv[(5*i) + 3], argv[(5*i) + 4], argv[(5*i) + 5], (char *)NULL);
                }
                else if(i == 0){
                    close(pipes[i][0]);
                    dup2(pipes[i][1], 1);
                    close(pipes[i][1]);
                    execlp("./biquad", "biquad", argv[(5*i) + 1], argv[(5*i) + 2], argv[(5*i) + 3], argv[(5*i) + 4], argv[(5*i) + 5], (char *)NULL);
                }
                else
                {
                    dup2(pipes[i-1][0], 0);
                    close(pipes[i-1][0]);
                    close(pipes[i][0]);

                    dup2(pipes[i][1], 1);
                    close(pipes[i][1]);
                    execlp("./biquad", "biquad", argv[(5*i) + 1], argv[(5*i) + 2], argv[(5*i) + 3], argv[(5*i) + 4], argv[(5*i) + 5], (char *)NULL);
                }
            }

            else if(p[i] > 0){
                if(i == 0){
                    close(pipes[i][1]);
                }
                if(i == num - 1){
                    close(pipes[i-1][0]);
                }

                if(i > 0){
                    close(pipes[i-1][0]);
                    close(pipes[i][1]);
                }
            }
        }
    }
    else{
        p[0] = fork();
        if(p[0] == 0){
            execlp("./biquad", "biquad", argv[1], argv[2], argv[3], argv[4], argv[5], (char *)NULL);
        }
    }

    for (int j = 0; j < num; j ++){
        waitpid(p[j], NULL, 0);
    }
    return 1;
}

