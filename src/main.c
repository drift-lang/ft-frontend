/* Drift
 *
 * 	- https://drift-lang.fun/
 *
 * GPL 3.0 License - bingxio <3106740988@qq.com> */
#include "lexer.h"
#include "compiler.h"
#include "vm.h"

#define COMPILER_VERSION "Drift 0.0.1 (MADE AT Jul 29 15:40:45)"

bool show_tokens; /* Exec arguments */
bool show_bytes;
bool show_tb;

/* Run source code */
void run(char *source, int fsize) {
    /* Lexical analysis */
    list *tokens = lexer(source, fsize);

    if (show_tokens) {
        for (int i = 0; i < tokens->len; i ++) {
            token *t = tokens->data[i];
            printf("[%3d]\t%-5d %-20s %-20d %d\n", 
                i, t->kind, t->literal, t->line, t->off);
        }
    }

    /* Compiler */
    list *codes = compile(tokens);
    if (show_bytes) dissemble(codes->data[0]);

    /* Virtual machine */
    vm_state state = evaluate(codes->data[0]);
    if (show_tb) {
        frame *main = (frame *)state.frame->data[0];
        dissemble_table(main->tb,
            main->code->description);
    }

    free(source); /* Release memory */
    free_list(tokens);
    free_list(codes);
}

/* Print license*/
void license() {
    printf("1. GNU Public License v3.0 @ bingxio 2021.\n");
}

/* Print usage information */
void usage() {
    printf("usage: <option> FILE(program file with .ft suffix)\n \
\n\
  -t  print token list\n\
  -b  print bytecode list\n\
  -s  print table\n\n\
  -v  print version\n\
  -l  print license\n");
    exit(EXIT_FAILURE);
}

/* ? */
int main(int argc, char **argv) {
    if (argc < 2) usage();
    if (argc == 2) {
        if (strcmp(argv[1], "-v") == 0) {
            printf("%s\n", COMPILER_VERSION);
            exit(EXIT_SUCCESS);
        }
        if (strcmp(argv[1], "-l") == 0) {
            license();
            exit(EXIT_SUCCESS);
        }
    }
    if (argc == 3) {
        if (strcmp(argv[1], "-t") == 0) show_tokens = true;
        if (strcmp(argv[1], "-b") == 0) show_bytes = true;
        if (strcmp(argv[1], "-s") == 0) show_tb = true;
    }
    const char *path = argc == 3 ? argv[2] : argv[1];
    int len = strlen(path) - 1;
    if (path[len] != 't'  || 
        path[len - 1] != 'f' ||
        path[len - 2] != '.') {
            fprintf(stderr, 
                "\033[1;31merror:\033[0m please load the source file with .ft sufix.\n");
            exit(EXIT_FAILURE);
    }
    FILE *fp = fopen(path, "r"); /* Open file of path */
    if (fp == NULL) {
        printf("\033[1;31merror:\033[0m failed to read buffer of file: %s.\n", path);
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    int fsize = ftell(fp); /* Returns the size of file */
    fseek(fp, 0, SEEK_SET);
    char *buf = (char *) malloc(fsize * sizeof(char));

    fread(buf, fsize, sizeof(char), fp); /* Read file to buffer*/
    buf[fsize] = '\0';

    run(buf, fsize);
    
    fclose(fp); /* Close file */
    return 0;
}