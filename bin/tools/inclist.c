#include <stdio.h>
#include <io.h>
#include <malloc.h>
#include <string.h>

#define MAX_SIZE 260

enum{
    STATE_IDLE = 0,
    STATE_INC = 1,
    STATE_REC = 2
};

const char* text_help = "Usage: inclist <options...> [file]"
                        "   -h show help"
                        "   -o show include files at front only"
                        "   -e check file's existence";

typedef unsigned char ubool;

int main(int argc, char** argv){
    char file[MAX_SIZE + 1];
    char buf[MAX_SIZE + 1];
    int buf_ptr = 0;
    FILE* fp;
    int len;
    char* text;

    ubool front_only = 0;
    ubool check = 0;

    file[0] = '\0';
    file[MAX_SIZE] = '\0';
    for(int i = 1; i < argc; i++){
        if(!strcmp(argv[i], "-h")){
            puts(text_help);
            return 0;
        }else if(!strcmp(argv[i], "-o")){
            front_only = 1;
            continue;
        }else if(!strcmp(argv[i], "-e")){
            check = 1;
            continue;
        }else{
            if(file[0]){
                puts("Error: multiple input files");
                return 1;
            }
            strncpy(file, argv[i], MAX_SIZE);
        }
    }

    if(!file[0]){
        puts("Error: no input file");
        puts(text_help);
        return 1;
    }
    fp = fopen(file, "r");
    if(!fp){
        printf("Error: cannnot open file '%s'\n", file);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    text = malloc(len + 1);
    if(!text){
        puts("Runtime Error: cannot allocate memory");
        return 1;
    }
    fread(text, 1, len, fp);
    text[len] = '\0';
    fclose(fp);

    int state = STATE_IDLE;

    for(int i = 0; i < len; i++){
        switch(state){
        case STATE_IDLE:
            if(!strncmp(text + i, "#include", 8)){
                state = STATE_INC;
                i += 7;
                break;
            }
            if(front_only && !strchr(" \t\n\r\b", text[i])){
                free(text);
                return 0;
            }
            break;
        case STATE_INC:
            if(text[i] == '<'){
                state = STATE_REC;
                buf_ptr = 0;
            }
            break;
        case STATE_REC:
            if(text[i] == '>'){
                buf[buf_ptr] = '\0';
                if(!check || access(buf, F_OK) == 0)
                    puts(buf);
                state = STATE_IDLE;
                break;
            }
            if(buf_ptr < MAX_SIZE)
                buf[buf_ptr++] = text[i];
            break;
        default:
            printf("Runtime Error: unknown state %d\n", state);
            return 1;
        }
    }

    free(text);

    return 0;
}