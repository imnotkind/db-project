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

## Define structs

아예 타입을 struct로 define해보자

```c
00000000 BfmHashKey      struc ; (sizeof=0x8, align=0x8, mappedto_11)
00000000                                         ; XREF: BufferTable/r
00000000 pageNo          dd ?
00000004 volNo           dw ?
00000006                 db ? ; undefined
00000007                 db ? ; undefined
00000008 BfmHashKey      ends
00000008
00000000 ; ---------------------------------------------------------------------------
00000000
00000000 BufferTable     struc ; (sizeof=0x10, align=0x8, mappedto_12)
00000000 key             BfmHashKey ?
00000008 fixed           dw ?
0000000A bits            db ?
0000000B                 db ? ; undefined
0000000C nextHashEntry   dw ?
0000000E                 db ? ; undefined
0000000F                 db ? ; undefined
00000010 BufferTable     ends
00000010
00000000 ; ---------------------------------------------------------------------------
00000000
00000000 BufferInfo      struc ; (sizeof=0x20, align=0x8, mappedto_13)
00000000 bufSize         dw ?
00000002 nextVictim      dw ?
00000004 nBufs           dw ?
00000006                 db ? ; undefined
00000007                 db ? ; undefined
00000008 bufTable        dq ?                    ; offset
00000010 bufferPool      dq ?                    ; offset
00000018 hashTable       dq ?                    ; offset
00000020 BufferInfo      ends
00000020
```

완성했다. 이제 코드는 아주 아름다워졌다.

```c
HashTableEntry_ptr = &bufInfo[type_0_or_1].hashTable[(__int16)((key->pageNo + volNo) % (3 * nBufs - 1))];
    bufInfo[type].bufTable[index].nextHashEntry = *HashTableEntry_ptr;// BI_NEXTHASHENTRY(type, index) = BI_HASHTABLEENTRY(type, hashValue)
    *HashTableEntry_ptr = index;
    result = 0;
```





## macros

https://www.cnblogs.com/goodhacker/p/7692443.html

```
#define _WORD  uint16
typedef int16 WORD;

WORD2(x) : *(((_WORD*)&x)+2)
#define SWORDn(x, n)   (*((int16*)&(x)+n))
```





# Just Memo

*.vol 파일(디스크)를 실행파일이 바꾸므로 유의할 것