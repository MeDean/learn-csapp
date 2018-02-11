#include <iostream>
// #include <stdio.h>
// #include <cstdio>
// input & optput stream
#define TMD 5;
using namespace std;

int main()
{   
    //<< 插入运算符

    // endl end line
    //endl 与 \n区别
    //\n只换行，endl确保改行代码立刻输出 相当于fflush(stdin)
    cout << "Hello" <<  "world" << endl;
    cout << "Hello world" << endl;
    // std::cout << "Hello world" << std::endl;
    //printf("哈哈哈");

    //编译(预处理->编译->目标文件)
    // 形成目标代码/文件，目标代码是编译器的输出结果，常见扩展名是".o"或".obj"
    // 1.预处理  g++ -E start.cpp > start.i        生成.i的文件[预处理器cpp] 
    //      即扩展源代码，插入所有用#include命令指定的文件，并扩展所有用#define声明指定的宏                  
    // 2.将预处理后的文件转换成汇编语言,生成文件.s[编译器egcs]                               g++ -S start.i -o start.s  可以用-o也可以用>
    // 3.汇编变为目标代码(机器代码)生成.o的二进制的目标文件[汇编器as:汇编语言翻译为机器语言的程序]        g++ -c start.s > start.o
    //      目标代码是机器代码的一种形式，它包含所有指令的二进制表示，但是还没有填入全局值的地址
    //链接
    //将目标代码跟c++函数库连接，并将源程序所用的库代码与目标代码      g++ start.o > start -L /usr/include/c++/5/iostream
    //      链接器将目标文件与实现库函数(例如printf)的代码合并，并产生最终的可执行代码文件。
    //      可执行代码使我们要考虑的机器代码的第二种形式，也就是处理器执行的代码格式
    //g++ start.o -o start
    return 0;
}