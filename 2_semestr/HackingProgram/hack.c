#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

const unsigned long long normal_hash = 12508817916473043957;
const unsigned long long cracked_hash = 4682479997069948058;

char* MyReadFile(const char* file_name, size_t* program_size) {
    assert(file_name);

    FILE* program_file = fopen(file_name, "rb");
    assert(program_file);

    fseek(program_file, 0, SEEK_END);
    size_t fsize = ftell(program_file);
    fseek(program_file, 0, SEEK_SET);

    char* program = calloc(fsize, sizeof(program[0]));
    assert(program);

    fread(program, 1, fsize, program_file);
    fclose(program_file);

    *program_size = fsize;
    return program;
}

void MyWriteFile(const char* file_name, size_t program_size, char* program) {
    assert(file_name);
    assert(program);

    FILE* program_file = fopen(file_name, "wb");
    assert(program_file);

    fwrite(program, program_size, 1, program_file);
    fclose(program_file);
}

unsigned long long Hash(void* buffer, size_t buffer_size) {
  assert(buffer != NULL);

  char* norm_buffer = (char*) buffer;
  unsigned long long hash = 0;

  for (int i = 0; i < buffer_size; ++i)
    hash = hash * 1664525 + (unsigned char)norm_buffer[i] + 1013904223;

  return hash;
}

void delay(int number_of_seconds)
{
    int milli_seconds = 100 * number_of_seconds;
    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds)
        ;
}

void PrintStatusBar() {
    int stars_number = 13;

    for (int i = 0; i < stars_number + 1; ++i) {
        printf("Program is cracking: [");
        for (int j = 0; j < i; ++j)
            printf("*");
        for (int j = 0; j < stars_number - i; ++j)
            printf(" ");
        printf("]\n");
        delay(10);
        system("cls");
    }
}

void PrintEndMessage() {

    const char* Msg = "Congratulations! Program cracked\n\nPlease, donat me\n";
    size_t Msg_len = strlen(Msg);

    for (int i = 0; i < Msg_len; ++i) {
        putc(Msg[i], stdout);
        delay(1);
    }

}

int main() {
    size_t program_size = 0;
    char* program = MyReadFile("S:\\Hack\\PASS.COM", &program_size);
    unsigned long long hash = Hash(program, program_size);


    if (program[22] == 117 && hash == cracked_hash) {
        printf("Program is already cracked!!\n");
        return 0;
    }

    if (hash != normal_hash) {
        printf("Program is modified. Please check version\n");
        return 0;
    }


    //                      PROGRAM CRACKING
    //====================================================================
    program[22] += 1;
    //====================================================================



    PlaySound("song.wav",NULL,SND_ASYNC | SND_LOOP | SND_FILENAME);
    PrintStatusBar();
    PrintEndMessage();

    MyWriteFile("S:\\Hack\\PASS.COM", program_size, program);
    delay(1000);


    struct {
        int a;
        char b;
        int d;
    } s;

    printf("%d\n", sizeof(s));

    return 0;

}
