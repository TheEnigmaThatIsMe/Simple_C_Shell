#include "../include/commands.h"

int main(){

	//initialize buffer	for command inputs
	char buffer[100];
	int nbytes = 100;
	
	//Display welcome so you know when inside the program
	printf("--------------------\n");
	printf("Welcome to the Shell\n");
	printf("--------------------\n");
	printf("Please enter a command: ");
	
	//get the command and subsequent string
	fgets(buffer, nbytes, stdin);
	
	//initialize various variables used in command checks
	char command[100];
	char exit[4];
	char pwd[3];
	char cd[2];
	char ls[2];
	char grep[4];
	char exec[2];
	char help[4];
	char If[2];
	char* argv[100];
	char* token = NULL;
	char* commandToken = NULL;
	pid_t pid;
    int status;
	
	//create invalid characters to search for in commands
	const char *invalid_characters = "./";
	const char *pipeCharacter = "|";
	
	//copy command value into initialized variables
	strcpy(command, buffer);
   	strcpy(exit, "exit");
   	strcpy(pwd, "pwd");
   	strcpy(cd, "cd");
   	strcpy(ls, "ls");
   	strcpy(grep, "grep");
   	strcpy(help, "help");
   	strcpy(exec, "./");
   	strcpy(If, "If");
   	
   	//tokenize command looking for new line or executable command
   	commandToken = strtok(command, "./ \n");
   	
   	//if buffer contains invalid characters set commandToken to executable command
   	if (strchr(invalid_characters, *buffer)){
   		commandToken = "./";
   	}
   	
   	//error check for a NULL command
   	while(commandToken == NULL){
		printf("ERROR: Command is NULL\n");
		printf("\nPlease enter a command: ");
		fgets(buffer, nbytes, stdin);
		strcpy(command, buffer);
		commandToken = strtok(command, "./ \n");
	}
	//check for pipe character
	if (strchr(pipeCharacter, *buffer) == NULL){
		//while commandToken is not exit
		while (strcmp(commandToken, exit) != 0){
		
			//tokenize buffer for the first time to get past command
			token = strtok(buffer, "./ \n");
	
			//PRINT WORKING DIRECTORY COMMAND
			if (strcmp(commandToken, pwd) == 0)
			{
				if ((pid = fork()) == -1) {
					  perror("fork error");
					  _exit(EXIT_FAILURE);
				}
				else if (pid == 0) { /* start of child process      */
					if(getDirectory() != 0){
						printf("PRINT WORKING DIRECTORY FAILED!\n");
						return -1;
					}
					return 0;
				}
				else { /* start of parent process     */
					if ((pid = wait(&status)) == -1)
												 /* Wait for child process.*/
					   perror("wait error\n");

					else {                       /* Check status.                */
					   if (WIFSIGNALED(status) != 0)
						  printf("Child process ended because of signal %d\n",
								  WTERMSIG(status));
					   else if (WIFEXITED(status) != 0){}
				  
					   else
						  printf("Child process did not end normally\n");
					}
				}
			}
			//CHANGE DIRECTORY COMMAND
			/***********************************
			NOTE: MR. ENGLAND SAID THAT 
			FORKING THIS COMMAND WAS NOT NEEDED
			************************************/
			else if (strcmp(commandToken, cd) == 0)
			{
				//tokenize the file path from buffer
				token = strtok(NULL, " \n");
				if(moveDirectories(token) != 0){}
			}
	
			//LIST DIRECTORY COMMAND
			else if (strcmp(commandToken, ls) == 0)
			{
				//tokenize the filename from buffer
				token = strtok(NULL, " \n");
				if ((pid = fork()) == -1) {
					  perror("fork error");
					  _exit(EXIT_FAILURE);
				}
				else if (pid == 0) { /* start of child process      */
					if(readDirectory(token) != 0){
						printf("LIST DIRECTORY FAILED!\n");
						return -1;
					}
					return 0;
				}
				else { /* start of parent process     */
					if ((pid = wait(&status)) == -1)
												 /* Wait for child process.*/
					   perror("wait error\n");

					else {                       /* Check status.                */
					   if (WIFSIGNALED(status) != 0)
						  printf("Child process ended because of signal %d\n",
								  WTERMSIG(status));
					   else if (WIFEXITED(status) != 0){}
	
					   else
						  printf("Child process did not end normally\n");
					}
				}
			}
			//GREP COMMAND
			else if (strcmp(commandToken, grep) == 0){
				//intialize substring and filename params
				char* substring = NULL;
				char* filename = NULL;
		
				//get substring and filename to grep
				substring = strtok(NULL, " \n");
				filename = strtok(NULL, " \n");
		
				if ((pid = fork()) == -1) {
					  perror("fork error");
					  _exit(EXIT_FAILURE);
				}
				else if (pid == 0) { /* start of child process      */
					if(grepCommand(substring, filename) != 0){
						return -1;
					}
					return 0;
				}
				else { /* start of parent process     */
					if ((pid = wait(&status)) == -1)
												 /* Wait for child process.*/
					   perror("wait error\n");

					else {                       /* Check status.                */
					   if (WIFSIGNALED(status) != 0)
						  printf("Child process ended because of signal %d\n",
								  WTERMSIG(status));
					   else if (WIFEXITED(status) != 0){}
		
					   else
						  printf("Child process did not end normally\n");
					}
				}	
			}
			//EXECUTE COMMAND
			else if (strcmp(commandToken, exec) == 0){
				struct stat sb;
				/* Check to see if executable */
				if (stat(token, &sb) == 0 && sb.st_mode & S_IXUSR){
					*argv = token; 
					execute(argv);
				}
				/* non-executable */ 
				else{
					printf("NOT EXECUTABLE!\n");
				}
			}
			else if (strcmp(commandToken, help) == 0){
				printf("\n-------------------------------------------------------------------------\n");
				printf("Valid commands:\n");
				printf("pwd (print working directory)\n");
				printf("cd <file path> (change directory)\n");
				printf("ls OR ls <directory name> (displays list of directories or sub directories)\n");
				printf("grep <substring> <file name> (finds substring within a file)\n");
				printf("./<executable file name> (executes a file)\n");
				printf("exit (exits shell program)\n");
				printf("-------------------------------------------------------------------------\n");
			}
			//read the README for this one
			else if (strcmp(commandToken, If)==0){
				printf("\nDoes a bear shit in the woods?\n");
			}
			else{
				printf("Not a valid command! Input help to see a list of valid commands\n");
			}

			/*bottom of while loop to have user enter another command after
				a previous command either executes or fails*/
			printf("\nPlease enter a command: ");
			fgets(buffer, nbytes, stdin);
			strcpy(command, buffer);
			commandToken = strtok(command, "./ \n");
			if (strchr(invalid_characters, *buffer)){
				commandToken = "./";
			}
			if (strchr(pipeCharacter, *buffer) != NULL) {
				break;
			}
			while(commandToken == NULL){
				printf("ERROR: Command is NULL\n");
				printf("\nPlease enter a command: ");
				fgets(buffer, nbytes, stdin);
				strcpy(command, buffer);
				commandToken = strtok(command, "./ \n");
			}
		}
	}
	//begin laying pipe
	else if (strchr(pipeCharacter, *buffer) != NULL){
		//take first command string and make it str1
		//take second command string after | and set it = str2
		//parse out actual command from str1 (i.e. ls, cd, etc)
		//parse out actual command from str2 (i.e. ls, cd, etc)
	
		//fork process and execute str1 command as child process
		//return value of first command and send through pipe for use in second command, effectively ending child process
		//Use pipe passed value to fork and execute str2 command
	}
	//User command is exit
	printf("\nEXITING\n");
	return 0;
}

//get the current working directory (COMMAND: pwd)
int getDirectory(void){
	//initialize parameters
    char* cwd;
    char buff[PATH_MAX + 1];
    
    //run the command
	
	cwd = getcwd( buff, PATH_MAX + 1 );
	if( cwd != NULL ) {
		printf("\nCurrent working directory is: %s\n", cwd );
	}
	else{
		return EXIT_FAILURE;
	}

    //program executes successfully
    return EXIT_SUCCESS;
}

//change into a different directory (COMMAND: cd <file path>)
int moveDirectories(char* filepath){
	
    //run command
	if(chdir(filepath) == 0) {
		printf("\nDirectory changed to %s\n", filepath);
	} 
	//could not successfully change file path
	else{
		perror(filepath);
		return EXIT_FAILURE;
	}
     //program exited successfully
    return EXIT_SUCCESS;
}

//list the directories out to user (COMMAND: ls OR ls <file name>)
int readDirectory(char* file_name){
	
	//initialize parameters
	DIR *dir;
    struct dirent *dp;
    
	 //run command
	if(file_name == NULL) {
		dir = opendir(".");
		while ((dp=readdir(dir)) != NULL) {
			if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") )
			{
				// do nothing (straight logic)
			} 
			else {
				file_name = dp->d_name; // use it
				printf("\nfile_name: \"%s\"", file_name);
			}
		}
	}
		//same as above only thing that changes is directory
	else{
		dir = opendir(file_name);
		while ((dp=readdir(dir)) != NULL) {
			if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") )
			{
				// do nothing (straight logic)
			} 
			else {
				file_name = dp->d_name; // use it
				printf("file_name: \"%s\"\n", file_name);
			}
		}
	}
    //close the directory and exit successfully
    closedir(dir);
	return EXIT_SUCCESS;
}

//find a substring within a file (COMMAND: grep <substring> <file name>)
int grepCommand(char* substring, char* filename){
	//initialize parameters
	FILE * fp = NULL;
	char line[256];
	
	//open the file
	fp = fopen(filename, "r");
	
    //run command
	if (fp == NULL || substring == NULL){
		printf("Substring or file pointer is NULL!\nPlease enter in format: grep <substring> <file name>\n");
		return EXIT_FAILURE;
	}
	//while we get every line of the file
	while(fgets(line, sizeof(line), fp)){
		//check if line in file has substring
		if(strstr(line, substring) != NULL){
			printf("LINE: %s", line);
		}
	}
    //close file and exit successfully
	fclose(fp);
	return EXIT_SUCCESS;
}

//execute a program file (COMMAND: ./<executable>)
void  execute(char **argv){
	//initialize params
     pid_t  pid;
     int    status;
     const char *path = *argv;
	//fork process
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     //execute command
     else if (pid == 0) {          /* for the child process:         */
          if (execl(path, *argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     //wait for child to finish
     else {                                  /* for the parent:      */
          while (wait(&status) != pid);      /* wait for completion  */
     }
}