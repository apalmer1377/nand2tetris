#include "jack_compiler.h"

int main(int argc, char **argv) {
    struct dirent * rfile;
    DIR * argdir;

    char * fname = argv[1];

    char * ext = get_filename_ext(argv[1]);
    char * tokens = (char *) malloc(INT_MAX);
    char * xml = (char *) malloc(INT_MAX);
    if (strlen(ext) == 0) {
        argdir = opendir(argv[1]);
        char * fullname = (char *) malloc(strlen(argv[1]) + 51);
        strcpy(fullname,argv[1]);
        while ((rfile = readdir(argdir)) != NULL) {
            if (strcmp(get_filename_ext(rfile->d_name),"jack") == 0) {
                tokens[0] = '\0';
                xml[0] = '\0';
                strcat(fullname,rfile->d_name);
                printf("%s\n",fullname);
                parseFile(fullname,tokens,xml);
            }
            strcpy(fullname,argv[1]);
        }
        return 0;
    }

    printf("%s\n",fname);
    parseFile(fname,tokens,xml);
    return 0;
}

void parseFile(char *file, char * tokens, char * xml) {
    FILE * f = fopen(file,"r");
    if (f == NULL)
        exit(1);

    char * token = malloc(MAX_LEN);
    char c;
    int i = 0;
    struct classDec ** newClass = (struct classDec **) malloc(10*sizeof(struct classDec *));
    //char * xml = (char *) malloc(INT_MAX);
    while ((c = parseToken(f , token)) != EOF) {

        if (strcmp(token,"class") == 0) {
            parseClass(f,token,newClass+i);
            newClass[i]->next = NULL;
            if (i > 0)
                newClass[i-1]->next = newClass[i];
            i++;
        }

    }


    struct classDec * temp = newClass[0];
    while (temp != NULL) {
        printClass(temp);
        tokenizeClass(tokens,temp);
        outputClass(xml,temp);
        temp = temp->next;
    }

    char * out = strip_extension(file);
    char * tout = (char *) malloc(strlen(out) + 6);
    char * xout = (char *) malloc(strlen(out) + 5);
    strcpy(tout,out);
    strcpy(xout,out);
    strcat(tout,"T.xml");
    strcat(xout,".xml");
    printf("%s\n",xout);

    FILE * w = fopen(tout,"w+");
    if (w == NULL) {
        printf("damn\n");
        exit(1);
    }

    fputs(tokens,w);
    fclose(w);

    FILE * x = fopen(xout,"w+");
    if (x == NULL)
        exit(1);

    fputs(xml,x);
    fclose(x);

    return;
}
