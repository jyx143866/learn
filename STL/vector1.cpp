/***
 * @Author: jyx
 * @Date: 2024-10-23 14:01:37
 * @LastEditors: jyx
 * @Description:
 */
/***
 * @Author: jyx
 * @Date: 2024-10-23 14:01:37
 * @LastEditors: jyx
 * @Description: 简单测试使用vector的相关接口
 */
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

void MyCout(int val)
{
    cout << val << endl;
}

void test1()
{
    // 创建vector容器
    vector<int> v;
    v.push_back(5);
    v.push_back(8);
    v.push_back(11);
    v.push_back(9);
    v.push_back(6);
    vector<int>::iterator vbegin = v.begin();
    vector<int>::iterator vend = v.end();
    // 第一种遍历方式
    for (vector<int>::iterator i = v.begin(); i != v.end(); i++)
    {
        MyCout;
    }
    // 第二种遍历方式
    for_each(v.begin(), v.end(), MyCout);
}

int main()
{
    test1();
    return 0;
}