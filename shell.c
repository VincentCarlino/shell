#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
 
#include "list.h"
#include "parse.h"
#include "tokens.h"

static char temp_line[128];

void
execute(tree* line) {

	char** args = arg_array(line);
	
	if(streq(line->cmnd,"exit")) {
		exit(0);
	} 
	else if(streq(line->cmnd, "cd")) {
		tree* dir = line->arg1;
		chdir(dir->cmnd);
		printDir();
	} 
	else if(streq(line->cmnd, ">") || streq(line->cmnd, "<")) {
		handleRedirect(line);
	} 
	else if(streq(line->cmnd, ";")) {
		handleSemiColon(line);
	} 
	else if(streq(line->cmnd, "|")) {
		handlePipe(line);
	} 
	else if(streq(line->cmnd, "&")) {
		handleBG(line);
	} 
	else if(streq(line->cmnd, "&&") || streq(line->cmnd, "||")) {
		handleLogical(line);
	}
	
	else {
		execute_args(args);
	}
	
	
}

void handleRedirect(tree* line) {
	printf("Handling redirect\n");
}

void handleSemiColon(tree* line) {
	printf("Handling semicolon\n");
}

void handlePipe(tree* line) {
	printf("Handling Pipe\n");
	int pipes[2];
	int cpid, rv;
	rv = pipe(pipes);
	
	char* la;
	char* ra;
	
	
	if(rv < 0) {
		perror("pipe failed\n");
	}		
	
	//pipes[0] = read
	//pipes[1] = write
	
	if(cpid = fork()) {
		printf("before closing write\n");
		close(pipes[1]);

		int status;
		waitpid(cpid, &status, 0);
	}
	else {
		close(pipes[0]);
		
		rv = dup(pipes[1]);
		if(rv < 1) {
			perror("dup failed\n");
		}
		
		la = arg_array(line->arg1);
		printf("got to the execution of side 1\n");
		execvp(la[0], la);
	}

	char tmp[256];
	int n;
		
	do {
		n = read(pipes[0], tmp, 256);
		if(n < 0) {
			
		}
	}

	

	
}

void handleBG(tree* line) {
	printf("Handling background\n");
}

void handleLogical(tree* line) {
	printf("Handling logical operator\n");
}

void
execute_args(char** args) {

	int cpid;
	if((cpid = fork())) {
		// parent

		int status;
		waitpid(cpid, &status, 0);
	}	
	else {
		// child
		if(execvp(args[0], args) == -1) {
			perror("error running command\n");
		}
	}

}

void 
printDir()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	//printf("%s\n", cwd);
}

char*
read_line(int fd)
{
    int ii;
    for (ii = 0; ii < 100; ++ii) {
        int rv = read(fd, temp_line + ii, 1);
        if (rv == 0) {
            return 0;
        }
        if (temp_line[ii] == '\n') {
            temp_line[ii] = 0;
            break;
        }
    }
    return temp_line;
}

int
main(int argc, char* argv[])
{
	
    
	if (argc != 2) {
        fprintf(stderr, "Usage: ./shell input.sexp\n");
        return 1;
    }
	
    int input = open(argv[1], O_RDONLY);
	    
	if (input == -1) {
        perror("open failed");
        return 1;
    }
	
	
    char* line;
    int rv = 0;
	
	cell* token;
	cell** tokenized;
	tree* parsed;	

    while ((line = read_line(input))) {
	
		token = tokens(line);
		tokenized = &token;
		parsed = parse(tokenized);
		execute(parsed);
	}
	

}

//(">" ("echo" "one") "myfile.txt")
