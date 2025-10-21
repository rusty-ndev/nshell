#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define nsh_RL_BUFSIZE 1024 
#define nsh_TOK_BUFSIZE 64 
#define nsh_TOK_DELIM " \t\r\n\a"

void nsh_loop(void);
char *nsh_read_line(void);
char **nsh_split_line(char *line);
int nsh_execute(char **args);
int nsh_launch(char **args);

int nsh_cd(char **args);
int nsh_help(char **args);
int nsh_exit(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &nsh_cd,
  &nsh_help,
  &nsh_exit
};

int nsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int nsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "nsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("nsh");
    }
  }
  return 1;
}


int nsh_help(char **args)
{
  int i;
  printf("Naeem's nsh\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < nsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}


int nsh_exit(char **args)
{
  return 0;
}

void nsh_loop(void){

	char *line; 
	char **args; 
	int status; 

	do{
		printf("> "); 
		line = nsh_read_line(); 
		args = nsh_split_line(line);
		status = nsh_execute(args); 

		free(line); 
		free(args); 
	} while(status); 
		
}

char** nsh_split_line(char* line){

	int bufsize = nsh_TOK_BUFSIZE , position = 0 ; 
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

  if (!tokens) {
    fprintf(stderr, "nsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, nsh_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += nsh_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "nsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, nsh_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
} 


char* nsh_read_line(void){

	int bufsize = nsh_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize); 
	int c; 
	
	if(!buffer){
		fprintf(stderr, "nsh: allocation error\n"); 
		exit(EXIT_FAILURE); 
	}

	while(1){
		// read character 
		c = getchar(); 

		// if we hit EOF, replace it with null character and return
		if(c == EOF|| c== '\n')
		{
			buffer[position] = '\0';
			return buffer; 
		}else{
			buffer[position] = c; 
		}
		position ++; 

		//if we have exceeded the buffer, reallocate
		if(position >= bufsize ){
			bufsize += nsh_RL_BUFSIZE; 
			buffer = realloc(buffer, bufsize); 
			if(!buffer){
				fprintf(stderr, "nsh: allocation error\n"); 
				exit(EXIT_FAILURE); 
			}
		}
	} 
 
}

int nsh_launch(char **args){
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("nsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("nsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int nsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < nsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return nsh_launch(args);
}



int main(int argc, char **argv){
	// config file loading

	// command loop
	nsh_loop();

	// nshutdown/cleanup
	return EXIT_SUCCESS; 
	
}
 
