/***
 * @Author: jyx
 * @Date: 2024-10-28 00:15:50
 * @LastEditors: jyx
 * @Description:给定一个非负整数 c ，
 * 你要判断是否存在两个整数 a 和 b，使得 a2 + b2 = c 。
 */
#include <iostream>
#include <vector>
#include <cmath>

class Solution
{
public:
    bool judgeSquareSum(int c)
    {
        int max = (int)sqrt(c);
        for (int i = 0; i < max; i++)
        {
            int j = (int)sqrt(c - pow(i, 2));
            if (pow(i, 2) + pow(j, 2) == c)
                return true;
        }
        return false;
    }
};

int main()
{
    int c;
    std::cin >> c;

    Solution test;

    bool answer = test.judgeSquareSum(c);
    std::cout << answer << std::endl;
    return 0;
}