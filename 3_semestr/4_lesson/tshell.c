#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

//maximum length of individual command
#define ARG_MAX 1024
//maximum number of arguments
#define NARG_MAX 10

#define SPLIT_TOKENS " \n"
#define PROMPT "\033[1;32mmyshell>\033[0m "

ssize_t fdread(int fd, void *buf, size_t count);
ssize_t fdreadline(int fd, char *buf, size_t count);
ssize_t fdwrite(int fd, const void *buf, size_t count);
void fdecho(int fd_in, int fd_out, void *buf, size_t size);
	
	
int split_cmd(char *cmd, char **argv) {
	cmd = strtok(cmd, " \n");
	int i = 0;
	while (cmd)
	{
		argv[i++] = cmd;
		cmd = strtok(NULL, " \n");
	}
	if (!strcmp(argv[0], "quit"))
		exit(0);
	argv[i] = NULL;
	return i;
}

int main() {
	char buf[ARG_MAX + 1];
	char *cmd_argv[NARG_MAX];
	do {
		fdwrite(STDOUT_FILENO, PROMPT, strlen(PROMPT));
		int res = fdreadline(STDIN_FILENO, buf, ARG_MAX);
		if (res[buf] != '\n')
			return 0;
		buf[res + 1] = '\0';
		char *cmd = buf;
		char *next_cmd = buf;
		
		int next_in = 0;

		do {
			next_cmd = strchr(next_cmd, '|');
			if (next_cmd)
				(*next_cmd++) = '\0';
			split_cmd(cmd, cmd_argv);

			int fd[2];
			int status = pipe(fd);
			if (status < 0) {
				perror("Pipe error:");
				return errno;
			}

			int pid = fork();
			if (pid < 0) {
				perror("Fork error:");
				return errno;
			}
      if (pid == 0) {
        printf("%s\n", cmd);
        printf("%s\n", cmd_argv[1]);
				status = dup2(next_in, 0);
				if (status < 0) {
					perror("Dup error:");
					return errno;
				}

				status = dup2(fd[1], 1);
				if (status < 0) {
					perror("Dup error:");
					return errno;
				}

				status = close(fd[0]);
				if (status < 0) {
					perror("Close error:");
					return errno;
				}
				execvp(cmd_argv[0], cmd_argv);
				perror("Exec error:");
				return errno;
			}	
			next_in = fd[0];
			
			status = close(fd[1]);
			if (status < 0) {
				perror("Close error:");
				return errno;
			}

			cmd = next_cmd;
		} while (cmd);
		
		fdecho(next_in, STDOUT_FILENO, buf, ARG_MAX + 1);
		int wstatus;
		waitpid(-1, &wstatus, 0);
			
	} while (1);
}

ssize_t fdread(int fd, void *buf, size_t count) {
	ssize_t read_res;
	void *cur = buf;
	while ((read_res = read(fd, cur, count))) {
		if (read_res < 0) {
			perror("Read error:");
			return cur - buf;
		}
		cur += read_res;
		count -= read_res;
	}
	return cur - buf;
}

ssize_t fdreadline(int fd, char *buf, size_t count) {
	ssize_t read_res;
	char *cur = buf;
	while ((read_res = read(fd, cur, count))) {
		if (read_res < 0) {
			perror("Read error:");
			return cur - buf;
		}
		for (int i = 0; i < read_res; i++)
			if (cur[i] == '\n')
				return cur - buf + i;
		cur += read_res;
		count -= read_res;
	}
	return cur - buf;
}

ssize_t fdwrite(int fd, const void *buf, size_t count) {
	ssize_t write_res;
	const void *cur = buf;
	while ((write_res = write(fd, cur, count))) {
		if (write_res < 0) {
			perror("Write error:");
			return cur - buf;
		}
		cur += write_res;
		count -= write_res;
	}
	return cur - buf;
}

void fdecho(int fd_in, int fd_out, void *buf, size_t size) {
	ssize_t res;
	while ((res = fdread(fd_in, buf, size)))
		fdwrite(fd_out, buf, res);
}

