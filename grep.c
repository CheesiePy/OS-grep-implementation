#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024 

void process_file(int fd, const char *filename, const char *search_term) {
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    int line_pos = 0;
    ssize_t bytes_read;
    int match_count = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n' || line_pos >= BUFFER_SIZE - 1) {
                line[line_pos] = '\0';
                if (strstr(line, search_term)) {
                    write(STDOUT_FILENO, line, strlen(line));
                    write(STDOUT_FILENO, "\n", 1);
                    match_count++;
                }
                line_pos = 0;
            } else {
                line[line_pos++] = buffer[i];
            }
        }
    }

    if (bytes_read == -1) {
        perror("read");
        return;
    }

    dprintf(STDOUT_FILENO, "%s: %d lines found\n", filename, match_count); 
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        dprintf(STDERR_FILENO, "Usage: %s <search_term> [file...]\n", argv[0]);
        exit(1);
    }

    const char *search_term = argv[1];

    if (argc == 2) {
        // No files given, use standard input
        process_file(STDIN_FILENO, "stdin", search_term);
    } else {
        // Process each file
        for (int i = 2; i < argc; i++) {
            int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                perror(argv[i]);
                continue;
            }

            process_file(fd, argv[i], search_term);
            close(fd);
        }
    }

    return 0;
}
