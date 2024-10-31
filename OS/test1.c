/*
 * @Author: jyx
 * @Date: 2024-10-29 11:59:48
 * @LastEditors: jyx
 * @Description:
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int A;
int B = 0;
int C = 2;
static int D;
static int E = 0;
static int F = 4;
const int G = 5;
const char H = 6;
int main()
{
    int a;
    int b = 0;
    int c = 2;
    static int d;
    static int e = 0;
    static int f = 4;
    const int g = 5;

    char char1[] = "abcde";
    char *cptr = "123456";

    int *heap = malloc(sizeof(int) * 4);

    printf("int A   A_addr = %p\n", &A);
    printf("int B = 0   B_addr = %p\n", &B);
    printf("int C = 2   C_addr = %p\n", &C);
    printf("static int D    D_addr = %p\n", &D);
    printf("static int E = 0    E_addr = %p\n", &E);
    printf("static int F = 4    F_addr = %p\n", &F);
    printf("const int G = 5     G_addr = %p\n", &G);
    printf("const char H = 6    H_addr = %p\n", &H);

    printf("\n");

    printf("int a    a_addr = %p\n", &a);
    printf("int b = 0    b_addr = %p\n", &b);
    printf("int c = 2    c_addr = %p\n", &c);
    printf("static int d    d_addr = %p\n", &d);
    printf("static int e = 0    e_addr = %p\n", &e);
    printf("static int f = 4    f_addr = %p\n", &f);
    printf("const int g = 5    g_addr = %p\n", &g);

    printf("char char1[] = abcde    char1[]addr = %p\n", char1);
    printf("char char1[] = abcde    &char1[]\n", &char1);
    printf("char *cptr =  123456     addr = %p\n", &cptr);
    printf("char *cptr =  123456     value = %p\n", cptr);
    printf("value of the cptr   cptr_value_0x%p\n", cptr);
    printf("value of %p     value_0x%p = %d\n", cptr, cptr, *cptr);
    printf("int *heap = malloc(sizeof(int) * 4)     heap_addr = %p\n", heap);
    printf("int *heap = malloc(sizeof(int) * 4)     heap_addr = %p\n", &heap);
}