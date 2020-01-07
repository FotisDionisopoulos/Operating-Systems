#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define token_del " \t\r\n\a"

//Function Declarations
void shell();
void execute(char **args);

//Main
int main(int argc, char **argv) 
{
  shell();
  return 0;
}

//Shell: Reading,Parsing and executing
void shell() 
{
  
  char *cur_tok, **args, *input;
  int tok_num, pos, bufsize1;
  ssize_t bufsize;

  while(1)
  {
    pos=0;
    tok_num = 0;
    args = malloc(80*sizeof(char*));
    cur_tok = malloc (sizeof(char) * 80);

    //read input
    printf("$");
    input = NULL;
    bufsize = 0;
    getline(&input, &bufsize, stdin);

    //parsing
    bufsize1 = 0;
    cur_tok = strtok(input, token_del);
    if(cur_tok != NULL){//IF1
    while (cur_tok != NULL) 
       {
          args[tok_num] = cur_tok;
          tok_num++;
         
          if (tok_num >= 64) 
            {
              bufsize1 += 64;
              args = realloc(args, bufsize1 * sizeof(char*));
            if (!args) 
             {
               fprintf(stderr, "Error: allocation error\n");
                 exit(EXIT_FAILURE);
              }
            }

          cur_tok = strtok(NULL, token_del);
        }//END WHILE

    args[tok_num] = NULL;

    if (!(strcmp("exit",args[0]))) return ;

    //executing
    execute(args);

    }//END IF1

  if(input!=NULL) free(input);
  if(args!=NULL) free(args);
  if(cur_tok!=NULL) free(cur_tok);

  }//END WHILE
  return;
}




void execute(char **args)
{
  pid_t pid, wpid;
  int status;
 if(!(strcmp("cd",args[0]))) 
   {
      if (args[1] == NULL) 
        {
          fprintf(stderr, "Error : expected argument to \"cd\"\n");
        } 
      else 
        {
            if (chdir(args[1]) != 0) perror("Error :");
            return;
        }
  } else
    {
        //Executing(fork)
        pid = fork();
        if (pid == 0) 
          {
            // Child 
            if (execvp(args[0], args) == -1) 
              {
                perror("Exit :");
              }
              exit(EXIT_FAILURE);
          }else if (pid < 0) 
            {
              // Error at forking
              perror("Error: ");
            }
          else do // Parent
            {
                   wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                
    }
  return;
}






