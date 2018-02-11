# 1 "mstore.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "mstore.c"
long mult2(long ,long);

void multstore(long x,long y,long *dest){
    long t = mult2(x,y);
    *dest = t;
}
