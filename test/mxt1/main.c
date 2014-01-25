/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "record.h"
#include "field.h"

#define FILE_OPEN_FLAGS 

/*
 * Conditional fill for file size.
 */
void cfill(int fd, size_t count, uint8_t value){
    struct stat st;
    if (0 == fstat(fd,&st)){
        if (0 == st.st_size){

            size_t cc;
            uint8_t b[count];
            size_t acc = count;
            ssize_t del;

            while (0 < acc){
                del = write(fd,b,count);
                acc -= del;
            }
        }
        else if (count > st.st_size){

            lseek(fd,0,SEEK_END);

            size_t cc;
            uint8_t b[count];
            size_t acc = count;
            ssize_t del;

            while (0 < acc){
                del = write(fd,b,count);
                acc -= del;
            }
        }
        lseek(fd,0,SEEK_SET);
    }
}

int opt(int argc, char** argv, int argx, char test, int def){
    if (argx < argc)

        return (0 != index(argv[argx],test));
    else
        return def;
}

int main(int argc, char** argv){
    char* prog = argv[0];
    if (1 < argc){
        char* file = argv[1];
        int fd = open(file,(O_RDWR|O_CREAT),(S_IRUSR|S_IWUSR));
        if (-1 != fd){
            int count = 1024;
            uint8_t dc = 3;
            size_t dp = record_sizeof(3);
            size_t plen = (count*dp);

            fprintf(stdout,"N: %d\n",count);
            fprintf(stdout,"C: %d\n",dc);
            fprintf(stdout,"P: %d\n",dp);
            fprintf(stdout,"Z: %d\n",plen);

            cfill(fd,plen,0);


            void* top = mmap(0,plen,(PROT_READ|PROT_WRITE),MAP_SHARED,fd,0);

            close(fd);

            if (MAP_FAILED != top){
                void* p;
                int cc;
                /*
                 * Conditional Review
                 */
                if (opt(argc,argv,2,'l',0)){
                    uint8_t *c, *z;
                    int col = 0;
                    for (c = top, z = (top+plen); c < z; c++){
                        switch(*c){
                        case FS:
                            col = 0;
                            fprintf(stdout,"\n<FS>");
                            break;
                        case GS:
                            col = 0;
                            if (top < c)
                                fprintf(stdout,"\n<GS>");
                            else
                                fprintf(stdout,"<GS>");
                            break;
                        case RS:
                            col = 0;
                            fprintf(stdout,"<RS>");
                            break;
                        default:
                            if (0 == (col % 4))
                                fprintf(stdout," <%02x>",*c);
                            else
                                fprintf(stdout,"<%02x>",*c);
                            col += 1;
                            break;
                        }
                    }
                    return 0;
                }
                /*
                 * Write Test
                 */
                if (opt(argc,argv,2,'w',1)){

                    for (cc = 0, p = top; cc < count; cc++, p += dp){

                        record* r = (record*)p;

                        if (0 != record_init(r,dc)){

                            fprintf(stderr, "%s: error in write test %d on '%s'.\n", prog,cc,file);
                            return 1;
                        }
                    }

                    fprintf(stderr, "%s: passed write test for '%s'.\n", prog,file);
                }
                /*
                 * Read Test
                 */
                if (opt(argc,argv,2,'r',1)){

                    for (cc = 0, p = top; cc < count; cc++, p += dp){

                        record* r = (record*)p;

                        if (0 != record_check(r,dc)){

                            fprintf(stderr, "%s: error in read test %d on '%s'.\n", prog,cc,file);
                            return 1;
                        }
                    }

                    fprintf(stderr, "%s: passed read test for '%s'.\n", prog,file);
                }
                return 0;
            }
            else {
                fprintf(stderr, "%s: error mapping file '%s'.\n", prog,file);
                return 1;
            }
        }
        else {
            fprintf(stderr, "%s: error opening file '%s'.\n", prog,file);
            return 1;
        }
    }
    else {
        fprintf(stderr, "Usage: %s <file> [-l]\n", prog);
        return 1;
    }
}
