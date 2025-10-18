#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SH_RL_BUFSIZE 1024 
#define SH_TOK_BUFSIZE 64 
#define SH_TOK_DELIM " \t\r\n\a"

void sh_loop(void){

	char *line; 
	char **args; 
	int status; 

	do{
		printf("> "); 
		line = sh_read_line(); 
		args = sh_split_line(line);
		status = sh_execute; 

		free(line); 
		free(args); 
	} while(status); 
		
}

char** sh_split_line(char* line){

	int bufsize = SH_TOK_BUFSIZE , position = 0 ; 
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, SH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += SH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, SH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
} 


char *sh_read_line(void){

	int bufsize = SH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize); 
	int c; 
	
	if(!buffer){
		fprintf(stderr, "sh: allocation error\n"); 
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
			bufsize += SH_RL_BUFSIZE; 
			buffer = realloc(buffer, bufsize); 
			if(!buffer){
				fprintf(stderr, "sh: allocation error\n"); 
				exit(EXIT_FAILURE); 
			}
		}
	} 
 
}



int main(int argc, char **argv){
	// config file loading

	// command loop
	sh_loop();

	// shutdown/cleanup
	return EXIT_SUCCESS; 
	
}
 
