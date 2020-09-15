# Project2 HACK

## Total data of structs

**BfmHashKey : 8 byte**

```c
typedef struct {
    PageNo pageNo;		/* a PageNo */
    VolNo volNo;		/* a volumeNo */
} BfMHashKey;
```

**BufferTable: 16 byte**

```c
typedef struct {
    BfMHashKey 	key;		/* identify a page */
    Two    	fixed;		/* fixed count */
    One    	bits;		/* bit 1 : DIRTY, bit 2 : VALID, bit 3 : REFER, bit 4 : NEW */
    Two    	nextHashEntry;
} BufferTable;
```

**BufferInfo : 32 byte**

`bufInfo`의 경우 `QWORD[]: 8 byte array` 로 정의되어 있는데, 액세스할때보면

` bufInfo[4 * type]  ` 으로 액세스한다.

이는 결국  bufInfo를 32바이트로 보고, 

```c
/* Buffer Types */
#define PAGE_BUF     0
#define LOT_LEAF_BUF 1
```

type이 0 또는 1이니 bufInfo[4]는 결국 &bufInfo + 4*8 인 것이다!

```c
typedef struct {
    Two                 bufSize;        /* size of a buffer in page size */
    UTwo                nextVictim;     /* starting point for searching a next victim */
    Two                 nBufs;          /* # of buffers in this buffer pool */
    BufferTable*	 	bufTable;
    char*		 		bufferPool;	/* a set of buffers */
    Two*       		 	hashTable;	/* hash table */
} BufferInfo;
```

세어 보면 6 + 3*8 에다 alignment니 32바이트라고 보는 게 맞다.

아예 타입을 struct로 define해보자



## macros

https://www.cnblogs.com/goodhacker/p/7692443.html

```
WORD2(x) : *(((_WORD*)&x)+2)
```



### structs

Views->SubViews->Structure

let's define all structures



