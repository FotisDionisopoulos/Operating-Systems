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
int start_procs (int in, int out, struct command *cmd) ;
int start_pipes (int n, struct command *cmd);
char *read_input();


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
    if(tok_num>1){ 
      cmd [i+1].argv =  NULL ;
      start_pipes (tok_num, cmd);
    }



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
          ;
        } 
      else 
        {
            if (chdir(args[1]) != 0) 
              {
                ;
              }
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
                ;
              }
              exit(EXIT_FAILURE);
          }else if (pid < 0) 
            {
              // Error at forking
              ;
            }
          else do // Parent
            {
                   wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                
    }
  return;
}




int start_procs (int in, int out, struct command *cmd) 
{
  pid_t pid,wpid;
  int status;

  if ((pid = fork ()) == 0)
    {
      if (in != 0)
        {
          dup2 (in, 0);
          close (in);
        }

      if (out != 1)
        {
          dup2 (out, 1);
          close (out);
        }
      
      execvp (cmd->argv [0], (char **)cmd->argv);
      exit(EXIT_FAILURE);
    }
    else do 
    {// Parent
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        
  return 1;
}



int start_pipes (int n, struct command *cmd)
{

  int i;
  pid_t pid,wpid;;
  int in, fd [2],status;
  in = 0;
  for (i = 0; i < n - 1; ++i)
    {
      pipe (fd);
      start_procs (in, fd [1], cmd + i);

      close (fd [1]);
      in = fd [0];
    }
  if (pid=fork() == 0) 
     {
       // Child 
        if (in != 0)
        dup2 (in, 0);
        execvp (cmd [i].argv [0], (char **)cmd [i].argv) ;
        exit(EXIT_FAILURE);
      }else if (pid < 0) 
      {
       ;
      }
      else do 
      { // Parent
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        
 close (fd [1]);
 close (fd [0]);
return 1;

}

