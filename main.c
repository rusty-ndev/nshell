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


int main(int argc, char **argv){
	// config file loading

	// command loop
	sh_loop();

	// shutdown/cleanup
	return EXIT_SUCCESS; 
	
}
