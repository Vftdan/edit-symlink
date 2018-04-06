#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define SILENT 1
#define HELP 2

const size_t bufsize = 512;
unsigned char conf = 0;

int exiterr(char * pre) {
	fprintf(stderr, "%s: %s\n", pre, strerror(errno));
	return errno || -1;
}

int main(int argc, char ** argv) {
	char linktarget[bufsize];
	char * linkname = (char*)NULL;
	int argi;
	for(argi = 1; argi < argc; argi++) {
		char * arg = argv[argi];
		if(arg[0] == '-') {
			if(arg[1] == '-') {
				arg += 2;
				if(!strcmp(arg, "silent")) {
					conf |= SILENT;
				} else if(!strcmp(arg, "help")) {
					conf |= HELP;
				} else {
					fprintf(stderr, "Unknown argument: \"%s\"\n", arg - 2);
					return -1;
				}
			} else {
				while(arg[1] != 0) {
					++arg;
					switch(*arg) {
						case 'h': conf |= HELP; break;
						case 's': conf |= SILENT; break;
						default: fprintf(stderr, "Unknown argument: \"-%c\"\n", *arg);
							 return -1;
					}
				}
			}
		} else {
			linkname = arg;
		}
	}
	if(conf & HELP) {
		printf("Usage: %s [OPTION]... LINK_NAME\n\
  -s, --silent   do not write addition messages to stdout\n\
  -h, --help     display this help and exit\n\
Old and new link targets cannot be longer than %zu bytes.\n\
", argv[0], bufsize);
		return 0;
	} else {
		if(linkname == (char*)NULL) {
			fprintf(stderr, "Link name is not specified\n");
			return -1;
		}
		if(readlink(linkname, linktarget, bufsize) == -1) return exiterr("Unable to read symlink");
		if(!(conf & SILENT)) printf("Current target:\n");
		printf("%s\n", linktarget);
		if(!(conf & SILENT)) printf("Enter new target:\n");
		scanf("%s", linktarget);
		if(unlink(linkname) == -1) return exiterr("Unable to delete old symlink");
		if(symlink(linktarget, linkname) == -1) return exiterr("Unable to read symlink");
		if(!(conf & SILENT)) printf("Link replaced!\n");
	}
}
