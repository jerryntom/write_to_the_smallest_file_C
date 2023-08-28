#include <stdio.h>
#include <string.h>

struct file_t
{
    FILE *f;
    int size;
};

int open_file(struct file_t* f, const char *filename) {
    if(f == NULL || filename == NULL) return 1;
    FILE *file;

    file = fopen(filename, "r");
    if(file == NULL) {
        file = fopen(filename, "a");
        if(file == NULL) return 2;
        f->f = file;
        fseek(file, 0, SEEK_END);
        f->size = ftell(file);
        fseek(file, 0, SEEK_SET);
    }
    else {
        fseek(file, 0, SEEK_END);
        f->size = ftell(file);
        fseek(file, 0, SEEK_SET);
        fclose(file);
        file = fopen(filename, "a");
        if(file == NULL) return 2;
        f->f = file;
    }

    return 0;
}

struct file_t* find_min(const struct file_t *files, int size) {
    if(files == NULL || size <= 0) return NULL;

    int min_size, min_size_index;

    min_size = (*files).size;
    min_size_index = 0;

    for(int index = 1; index < size; index++) {
        if((*(files + index)).size < min_size) {
            min_size = (*(files + index)).size;
            min_size_index = index;
        }
    }

    return (struct file_t *)(files + min_size_index);
}

void close_file(struct file_t *f) {
    if(f == NULL) return;

    if(f->f != NULL) fclose(f->f);
}

int main() {
    char buf[1000];
    struct file_t files[5], *file;
    int index_of_existing_file = 0;
    FILE *test, *fin;
    printf("Please input file paths:\n");
    do {
        fgets(buf, 999, stdin);
        if(strcmp(buf, "\n") != 0) {
            for(int i = 0; i < 999 && *(buf + i) != '\0'; i++) {
                if(*(buf + i) == '\n') {
                    *(buf + i) = '\0';
                }
            }
            test = fopen(buf, "r");
            if(test != NULL) {
                fclose(test);
                open_file(&(*(files + index_of_existing_file)), buf);
                index_of_existing_file++;
            }
        }
    } while(strcmp(buf, "\n") != 0 && index_of_existing_file <= 5);

    if(index_of_existing_file == 0) {
        printf("Couldn't open file");
        return 4;
    }

    printf("Please input texts:\n");
    do {
        fgets(buf, 999, stdin);
        if(strcmp(buf, "\n") != 0) {
            for(int i = 0; i < 999 && *(buf + i) != '\0'; i++) {
                if(*(buf + i) == '\n') {
                    *(buf + i) = '\0';
                }
            }
            file = find_min(files, index_of_existing_file);
            fin = file->f;
            fprintf(fin, "%s\n", buf);
            file->size = ftell(fin);
        }
    } while(strcmp(buf, "\n") != 0);

    for(int i = 0; i < index_of_existing_file; i++) {
        if((*(files + i)).f != NULL) {
            close_file(&(*(files + i)));
        }
    }

    printf("Files saved");

    return 0;
}
