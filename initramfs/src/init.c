#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define len(_arr) ((int)((&_arr)[1] - _arr))

void __attribute__((noreturn)) panic(const char *msg)
{
	fprintf(stderr, "%s: %s (errno = %d)\n", msg, strerror(errno), errno);
	exit(-1);
}

void __attribute__((noreturn)) exec_cmd(char *path)
{
	char *argv[20];
	*argv = path;
	int i = 0;

	// Search spaces and save the next character as an argument
	for (char *arg = path; (arg = strchr(arg, ' ')); )
		if (++i < len(argv) - 1) {
			*arg = 0;
			argv[i] = ++arg;
		} else {
			fprintf(stderr, "/etc/inittab command %s has too many arguments (%d supported)\n",
			path, len(argv));
			exit(-1);
		}

	argv[++i] = NULL;

	execv(path, argv);  // should never return
	panic("execv");
}

pid_t start(char *path)
{
	pid_t pid = fork();
	if (pid == -1) {
		panic("fork");
	} else if (pid) {
		printf("Starting %s (pid = %d)\n", path, pid);
		return pid;
	} else {
		exec_cmd(path);
	}
}

int wait_for_children(int count)
{
	int error = 0;
	while (count) {
		int wstatus;
		pid_t pid = wait(&wstatus);
		if (WIFEXITED(wstatus)) {
			printf("pid %d exited (%d)\n", pid, WEXITSTATUS(wstatus));
			error |= WEXITSTATUS(wstatus);
		} else if (WIFSIGNALED(wstatus)) {
			printf("pid %d killed (signal %d)\n", pid, WTERMSIG(wstatus));
		} else {
			continue;
		}
		count--;
	}
	return error;
}

/*
 * buf - 512-byte buffer. Will receive the next entry.
 */
int get_initcmd_entry(FILE *f, char *buf)
{
	for (;;) {
		if (!fgets(buf, 512, f) || buf[0] == '\n')
			return 0;
		if (buf[0] == '#')  // ignore comments
			continue;
		size_t len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = 0;
		return 1;
	}

}

int main()
{
	printf("Init starting\n");

	char initcmd_entry[512];
	FILE *initcmd = fopen("/etc/initcmd", "r");
	if (!initcmd)
		panic("Opening /etc/initcmd failed");

	printf("Running early initialization programs\n");
	while (get_initcmd_entry(initcmd, initcmd_entry)) {
		start(initcmd_entry);
		if (wait_for_children(1)) {
			fprintf(stderr, "%s returned non-zero status, init exiting\n", initcmd_entry);
			exit(-1);
		}
	}

	printf("Running programs\n");
	int program_count = 0;
	for (program_count = 0; get_initcmd_entry(initcmd, initcmd_entry); program_count++)
		start(initcmd_entry);
	wait_for_children(program_count);

	printf("init finished\n");
	for (;;)
		sleep(1000);
	return 0;
}
