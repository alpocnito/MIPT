#include <stdio.h>
#include <stdlib.h>

float* p4_max(int number_elems, float* arr) {
    if (arr == 0) return 0;
    float* max_ptr = arr;
    for (int i = 0; i < number_elems; ++i) {
        if (arr[i] > *max_ptr)
            max_ptr = arr + i;
    }

    return max_ptr;
}

void p1_reverse(char* str_start) {
    if (str_start == 0) return;
    if (*str_start == 0) return;
    
    char* str_end = str_start;

    while (*str_end)
        str_end++;
    str_end--;
    
    do {
        char temp = *str_start;
        *str_start = *str_end;
        *str_end = temp;
        
        str_start++;
        str_end--;
    } while (str_start < str_end);

    return;

}

void p2_append(char* dest, char* src) {
    if (dest == 0 || src == 0) return;
    while (*dest)
        dest++;
    dest--;
    src--;
    
    do {
        dest++;
        src++;
        *dest = *src;
    } while (*src != '\0');

    return;
}

int p3_sum_bytes(char* str) {
    if (str == 0) return 0;

    int sum = 0;
    do {
        sum += *str;
        str++;
    } while (*str != '\0');
    return sum;
}

int main() {
    putchar('\n');
    char temp[11] = "abcde";
    p1_reverse(temp);
    
    printf("<%s>\n", temp);
    p2_append(temp, "12345");
    
    printf("<%s>\n", temp);
    printf("<%u>\n", p3_sum_bytes(temp));
    

    float arr[] = {1.0, 5.0, 8.0, 9.0, -11.0};
    double ans = *p4_max(5, arr);
    
    printf("<%g>\n", ans);
    return 0;
}
