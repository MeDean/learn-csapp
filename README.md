## 1. 编译(预处理->编译->目标文件)
* * * * *
编译即形成目标代码/文件，目标代码是编译器的输出结果，常见扩展名是".o"或".obj"，分为一下三个步骤：
* 预处理

    即扩展源代码，插入所有用#include命令指定的文件，并扩展所有用#define声明指定的宏
    strat.i:一个ASCII码的中间文件

    gcc/g++ 命令：g++ -E start.cpp > start.i

* 编译

    将预处理后的文件转换成汇编语言,生成文件.s[编译器egcs]                
    start.s:一个ASCII码的汇编语言文件

    gcc/g++ 命令：g++ -S start.i -o(>) start.s

    mstore.c
    
        long mult2(long ,long);
        void multstore(long x,long y,long *dest){
            long t = mult2(x,y);
            *dest = t; 
        }

    gcc -Og -S mstore.c > mstore.s

    mstore.s

        multstore:
        .LFB0:
            .cfi_startproc
            pushq	%rbx
            .cfi_def_cfa_offset 16
            .cfi_offset 3, -16
            movq	%rdx, %rbx
            call	mult2
            movq	%rax, (%rbx)
            popq	%rbx
            .cfi_def_cfa_offset 8
            ret
            .cfi_endproc 
    
    所有以'.'开头的行都是指导汇编器和链接器工作的伪指令，可以忽略。
    上面左面有14个十六进制字节值分成若干组,每组1~5个字节.每组都是一个指令,右边是等价的汇编语言.
    * x86-64指令长度从1-16个字节不等
    * 设计指令格式的方式是，从某个给定位置开始，可以将字节唯一地解码成机器指令。
    * 反汇编器只是基于机器代码中的字节序列来确定汇编代码，不需要访问改程序的源代码或汇编代码
    * 反汇编器使用的指令命名规则与GCC生成的汇编代码使用的有些区别。如反汇编器给call和ret指令添加了'q'后缀，同样省略这些后缀也没问题

* 生成目标文件
    汇编变为目标代码(机器代码)生成.o的二进制的目标文件[汇编器as:汇编语言翻译为机器语言的程序]    
    start.o:一个可重定位目标文件

    gcc/g++ 命令：g++ -c start.s > start.o

    对mstore.o进行gdb调试:x/14xb multstore,即显示14个16进制数字

        0x0 <multstore>:0x53	0x48	0x89	0xd3	0xe8	0x00	0x00	0x00
        0x8 <multstore+8>:0x00	0x48	0x89	0x03	0x5b	0xc3

    对mstore.o反汇编：objdump -d mstore.o

        0000000000000000 <multstore>:
        Offset Bytes               Equivalent assembly language
            0:	53                   	push   %rbx
            1:	48 89 d3             	mov    %rdx,%rbx
            4:	e8 00 00 00 00       	callq  9 <multstore+0x9>
            9:	48 89 03             	mov    %rax,(%rbx)
            c:	5b                   	pop    %rbx
            d:	c3                   	retq  

    
    查看二进制文件的方法vim -b mstore.o   加上-b参数，以二进制打开，然后输入命令  :%!xxd -g 1  切换到十六进制模式显示        

    目标代码是机器代码的一种形式，它包含所有指令的二进制表示，但是还没有填入全局值的地址
## 2. 链接
* * *
将目标代码跟c++函数库连接，并将源程序所用的库代码与目标代码合并，并产生最终的可执行代码文件(可执行代码使我们要考虑的机器代码的第二种形式，也就是处理器执行的代码格式) 
运行链接器ld,将多个可重定位目标文件(由不同代码和数据节组成,每一节都是一个连续的字节序列)以及必要的系统目标文件组合起来，组成一个可执行目标文件
链接器工作任务分为两个步骤：
* 符号解析：目标文件定义和引用符号，每个符号对应于一个函数、一个全局变量或一个静态变量(C语言中任何以static属性声明的变量)。符号解析的目的是将每一个符号引用正好和一个符号定义关联起来。
* 重定位：编译器和汇编器生成从地址0开始的代码和数据节。链接器通过把每个符号定义与一个内存地址关联起来，从而重定位这些节，然后修改所有对这些符号的引用,使得它们指向这个内存位置。

gcc/g++ 命令：g++ start.o > start -L /usr/include/c++/5/iostream


***
main.c

    #include <stdio.h>
    void multstore(long,long,long *);

    int main(){
        long d;
        multstore(2,3,&d); 
        printf("2 * 3 --> %ld\n",d);
        return 0;
    }
    long mult2(long a,long b){
        long s = a * b;
        return s;
    }
生成可执行文件prog：gcc -Og -o prog main.c mstore.c

对prog反汇编：objdump -d prog
抽出其中一段：

    000000000040061b <multstore>:
        40061b:	53                   	push   %rbx
        40061c:	48 89 d3             	mov    %rdx,%rbx
        40061f:	e8 ef ff ff ff       	callq  400613 <mult2>
        400624:	48 89 03             	mov    %rax,(%rbx)
        400627:	5b                   	pop    %rbx
        400628:	c3                   	retq   
        400629:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
这段代码与mstore.c反汇编产生的代码几乎一样,其中最重要的区别就是左边列出的地址不同---链接器将这段代码的地址移到了一段不同的地址范围中。第二个不同之处在于链接器填上了callq指令调用函数mult2需要.第七行，nop即(No Operation),对程序没有影响,为了使函数代码凑够一定数量,使得存储器性能而言，能更好的放置下一代码块。

### 3.可重定位目文件
一个典型的ELF可重定位目标文件:
ELF头以一个16字节的序列开始，这个序列描述了生成该文件的系统的字的大小和字节顺序。ELF头剩下的部分包含帮助链接器语法分析和解释目标文件的信息。其中包括ELF头的大小、目标文件的类型(如可重定位，可执行，或者共享的)、机器类型、节目头表的文件偏移，以及头部表中条目的大小和数量。
包含下面几个节:
* .text:已编译程序的机器代码。
* .rodata:只读数据，比如printf语句中的格式串和开关语句的跳转表。
* .data:已初始化的全局和静态C变量。局部变量在运行时被保存在栈中，既不出现在.data节中，也不出现在.bss节中
* .bss:未初始化的全局和静态C变量，以及所有被初始化为0的全局或静态变量.这一节不占用实际的空间,仅仅是一个占位符。未初始化变量不需要占据磁盘空间，运行时，在内存分配这些变量初始值为0.
vim查看二进制文件：
vim -b your_binary_file
:%!xxd -g 1

#### 3.1符号和符号表
每个重定位模块m都有一个符号表，包含m定义和引用的符号的信息。在链接器上下文中，有三种不同的符号：
* 由模块m定义并能被其他模块引用的全局符号。全局链接器符号对应于非静态的C函数和全局变量。
* 由其他模块定义并被模块m引用的全局符号。这些符号称为外部符号，对应于其他模块中定义的非静态C函数和全局变量。
* 只被模块m定义和引用的局部变量。他们对应于带static属性的C函数和全局变量。这些符号在模块m中任何位置都可见，但不能被其他模块引用

任何带有static属性声明的全局变量和函数都是模块私有的，任何不带static属性声明的全局变量和函数都是公共的，可以被其他模块访问
