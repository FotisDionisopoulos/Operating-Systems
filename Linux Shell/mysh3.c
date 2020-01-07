#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define token_del "|"
#define token_del2 " \t\r\n\a\'\""

struct command
{
  char **argv;
};

void one_com(char **);
char *read_input();
void run(struct command *cmd);

int main ()
{

  struct command cmd[10];
  char *cur_tok, *input;
  int tok_num ,tok_num2, bufsize1, i, j;
  char **args, **args1 ;  

  input = NULL;
  while(1)
  {

    fflush(stdin);
    //fflush(stdout);
    args = malloc(64*sizeof(char*));
    tok_num = 0;
    bufsize1 = 0;

    //read input
    printf("$");
    input=read_input();

    //parsing
    cur_tok = strtok(input, token_del);
    if( cur_tok==NULL) continue;
    if(!(strcmp("exit",cur_tok))) break;

    if(cur_tok != NULL)
    {
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



    for(i=0;i<tok_num;i++)
    {

      tok_num2  = bufsize1 = 0;

      args1 = malloc(64*sizeof(char*));
    
      cur_tok = strtok(args[i], token_del2);
      while (cur_tok != NULL) 
        {
          args1[tok_num2] = cur_tok;
          tok_num2++;
          if (tok_num2 >= 64) 
            {
              bufsize1 += 64;
              args1 = realloc(args1, bufsize1 * sizeof(char*));
            if (!args1) 
              {
                fprintf(stderr, "Error: allocation error\n");
                exit(EXIT_FAILURE);
              }
      
            }

         cur_tok = strtok(NULL, token_del2);

        }//END WHILE

      if(tok_num==1) 
        {
          one_com(args1);
          break;
        }else
        {
          args1[tok_num2] = NULL;  
          cmd [i].argv =  args1 ;
        }

        }
         
    
      if(!(strcmp("exit",args[0]))) break;

      cmd [i+1].argv =  NULL ;
      run(cmd);


      if(args1!=NULL)  free(args1);
      if(input!=NULL)  free(input);
      if(args!=NULL)  free(args);
    }//END IF == NULL
  }//END WHILE 1

 for(i=0;i<tok_num;i++) 
    if(cmd[i].argv!=NULL) free(cmd[i].argv);
    
  return 0;
}



char *read_input()
{
  int buf_size = 512,pos = 0,B;
  char *buf = malloc(sizeof(char)*buf_size);

  if(!buf) fprintf(stderr,"MySh :Allocation error\n");

  while (1) 
  {
    // Read a character
    B = getchar();

    if (B == EOF || B == '\n') 
    {
      buf[pos] = '\0';
      return buf;
    } else 
    {
      buf[pos] = B;
    }

    pos++;

    if (pos >= buf_size) 
    {
      buf_size += 512;
      buf = realloc(buf, buf_size);
      if (!buf) 
      {
        fprintf(stderr, "MySh: Allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}





void one_com(char **args)
{
  pid_t pid, wpid;
  int status;
  if(!(strcmp("exit",args[0]))) exit(0);

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




void run(struct command *cmd)
{

  int pipes[2],status,i;
  pid_t pid1,pid2,wpid;
  pipe(pipes); // sets up pipe

  // fork the first child 
  if (pid1=fork() == 0)
    {
      // replace stdout with write part of 1st pipe
      dup2(pipes[1], 1);

      // close all pipes 
      close(pipes[0]);
      close(pipes[1]);

      execvp (cmd [0].argv [0], (char **)cmd [0].argv);
      exit(1);
    }
  else
    {
      // fork second child
      if (pid2=fork() == 0)
      {
        // replace grep's stdin with read end of 1st pipe  
        dup2(pipes[0], 0);

        // close all ends of pipes

        close(pipes[0]);
        close(pipes[1]);

        execvp (cmd [1].argv [0], (char **)cmd [1].argv);
        exit(1);
      }
	   else do {
                wpid = waitpid(pid2, &status, WUNTRACED);
              } while (!WIFEXITED(status) && !WIFSIGNALED(status));
	  
    }
      
  close(pipes[0]);
  close(pipes[1]);

  do {
       wpid = waitpid(pid2, &status, WUNTRACED);
  } while (!WIFEXITED(status) && !WIFSIGNALED(status));


}



