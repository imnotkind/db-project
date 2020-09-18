# Project3 HACK

## stucture alignment

http://www.catb.org/esr/structure-packing/

> In general, a struct instance will have the alignment of its widest scalar member.



```c
struct A {
    char *p;     /* 8 bytes */
    char c;      /* 1 byte */
};

struct B {
    char p;     /* 1 byte */
    char c;      /* 1 byte */
};
```

A 는 widest scalar member가 8byte이므로 8byte alignment 필요, sizeof(A) == 16

B는 1byte alignment, sizeof(B) == 2



> Here’s a last important detail: If your structure has structure members, the inner structs want to have the alignment of longest scalar too. Suppose you write this:

```c
struct foo5 {
    char c;
    struct foo5_inner {
        char *p;
        short x;
    } inner;
};

struct foo5 {
    char c;           /* 1 byte*/
    char pad1[7];     /* 7 bytes */
    struct foo5_inner {
        char *p;      /* 8 bytes */
        short x;      /* 2 bytes */
        char pad2[6]; /* 6 bytes */
    } inner;
};
```

