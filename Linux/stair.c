/*
 * @Author: jyx
 * @Date: 2024-10-27 13:00:08
 * @LastEditors: jyx
 * @Description:
 */
#include <stdio.h>

unsigned long long factorial(int n)
{
    unsigned long long fact = 1;
    for (int i = 1; i <= n; i++)
    {
        fact *= i;
    }
    return fact;
}

int main()
{
    int n;
    printf("Enter a number: ");
    scanf("%d", &n);

    if (n < 0)
    {
        printf("Factorial is not defined for negative numbers.\n");
    }
    else
    {
        unsigned long long fact = factorial(n);
        printf("Factorial of %d is %llu\n", n, fact);
    }

    return 0;
}
