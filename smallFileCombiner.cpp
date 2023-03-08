#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static int outFd = -1;
static const int maxsize = 1024;
static unsigned char buffer[maxsize];

void printUsage(const char *progname) {
    printf("%s outputfile inputfile-1 inputfile-2 inputfile-3 ...\n", progname);
}

bool append(const char *outputfile, const char *inputfile) {
    if (outFd < 0) {
        outFd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outFd < 0) {
            printf("open outfile %s failed\n", outputfile);
            return false;
        }
    }
    int inFd = open(inputfile, O_RDONLY);
    if (inFd < 0) {
        printf("open inputfile %s failed\n", inputfile);
        return false;
    }

    int readSize = 0, writeSize = 0;

    int got;
    while ((got = read(inFd, buffer, maxsize)) > 0) {
        readSize += got;
        writeSize += write(outFd, buffer, got);
    }

    printf("summary file: %s, read: %d byte(s), write: %d byte(s)\n", inputfile, readSize, writeSize);

    close(inFd);
    return true;
}

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *outputfile = argv[1];

    for (int i = 2; i < argc; i++) {
        const char *inputfile = argv[i];
        if ( !append(outputfile, inputfile) ) {
            printf("append failed, exiting...\n");
            break;
        }
    }

    if (outFd > 0) {
        close(outFd);
    }

    return 0;
}
