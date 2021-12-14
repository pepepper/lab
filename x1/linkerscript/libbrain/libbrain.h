#ifndef H_LIB_BRAIN_H
#define H_LIB_BRAIN_H

typedef unsigned long size_t;
typedef void FILE;
typedef void* DIR;

struct stat {
    unsigned int flags;
    unsigned int st_atim;
    unsigned int st_mtim;
    unsigned int st_ctim;
    size_t st_size;
};

struct dirent {
    unsigned int type;
    size_t size;
    size_t filename_length;
    char filename[260];
};

enum {
    SEEK_SET = 0,
    SEEK_CUR,
    SEEK_END,
};

void *sys_malloc(size_t size);
int sys_free(void *ptr);
FILE *sys_fopen(const char *filename, const char *mode);
int sys_fclose(FILE *stream);
int sys_ftell(FILE *stream);
size_t sys_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t sys_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int sys_fseek(FILE *stream, int offset, int whence);
int sys_feof(FILE *stream);
int sys_fflush(FILE *stream);
int sys_fgetc(FILE *stream);
char *sys_fgets(char *s, int n, FILE *stream);
int sys_fputc(int ch, FILE *stream);
int sys_fputs(const char *str, FILE *stream);
int sys_ferror(FILE *stream);
int sys_remove(const char *filepath);
int sys_rename(const char *oldpath, const char *newpath);
int sys_mkdir(const char *pathname);
int sys_stat(const char *filepath, struct stat *buf);
int sys_closedir(DIR *dirp);
struct dirent *sys_readdir(DIR *dirp);
DIR *sys_opendir(const char *pathname);

int sys_printf(const char *fmt);

#endif
