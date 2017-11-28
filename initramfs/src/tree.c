#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define RESET "\e[0m"
#define BLUE  "\e[34m"

static int recursive;

void print_tree(int depth)
{
	if (!depth)
		return;

	printf("├─");
	for (depth--; depth; depth--)
		printf("──");
}

void list(int depth)
{
	DIR *dir = opendir(".");
	if (!dir) {
		perror("opendir");
		return;
	}

	struct dirent *entry;
	while ((entry = readdir(dir))) {
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
			continue;

		print_tree(depth);
		if (entry->d_type != DT_DIR) {
			puts(entry->d_name);
		} else {
			printf(BLUE "%s\n" RESET, entry->d_name);
			if (recursive) {
				if (chdir(entry->d_name)) {
					perror("chdir");
					continue;
				}
				list(depth + 1);
				chdir("..");
			}
		}
	}
	closedir(dir);
}

int main(int argc, char **argv)
{
	int c;
	while ((c = getopt(argc, argv, "r")) != -1) {
		switch (c) {
			case 'r':
			recursive = 1;
			break;

			case '?':
			fprintf(stderr, "Usage: %s [-r] [dir1 [dir2] ...]\n", argv[0]);
			return -1;
		}
	}

	if (optind >= argc) {
		// No name parameters, list current directory
		list(0);
		return 0;
	}

	static char cwd[256];
	if (!getcwd(cwd, sizeof(cwd))) {
		perror("getcwd");
		return -1;
	}
	for (int i = optind; i < argc; i++) {
		printf(BLUE "%s:\n" RESET, argv[i]);
		if (chdir(argv[i])) {
			perror("chdir");
			continue;
		}
		list(1);
		if (chdir(cwd)) {
			perror("chdir");
			return -1;
		}
	}

	return 0;
}
