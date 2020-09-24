/******************************************************************************/
/*                                                                            */
/*    Copyright (c) 2013-2015, Kyu-Young Whang, KAIST                         */
/*    All rights reserved.                                                    */
/*                                                                            */
/*    Redistribution and use in source and binary forms, with or without      */
/*    modification, are permitted provided that the following conditions      */
/*    are met:                                                                */
/*                                                                            */
/*    1. Redistributions of source code must retain the above copyright       */
/*       notice, this list of conditions and the following disclaimer.        */
/*                                                                            */
/*    2. Redistributions in binary form must reproduce the above copyright    */
/*       notice, this list of conditions and the following disclaimer in      */
/*       the documentation and/or other materials provided with the           */
/*       distribution.                                                        */
/*                                                                            */
/*    3. Neither the name of the copyright holder nor the names of its        */
/*       contributors may be used to endorse or promote products derived      */
/*       from this software without specific prior written permission.        */
/*                                                                            */
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     */
/*    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       */
/*    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       */
/*    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          */
/*    COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,    */
/*    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;        */
/*    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER        */
/*    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT      */
/*    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN       */
/*    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE         */
/*    POSSIBILITY OF SUCH DAMAGE.                                             */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/*    ODYSSEUS/EduCOSMOS Educational Purpose Object Storage System            */
/*    (Version 1.0)                                                           */
/*                                                                            */
/*    Developed by Professor Kyu-Young Whang et al.                           */
/*                                                                            */
/*    Advanced Information Technology Research Center (AITrc)                 */
/*    Korea Advanced Institute of Science and Technology (KAIST)              */
/*                                                                            */
/*    e-mail: odysseus.educosmos@gmail.com                                    */
/*                                                                            */
/******************************************************************************/
/*
 * Module: edubtm_Compact.c
 * 
 * Description:
 *  Two functions edubtm_CompactInternalPage() and edubtm_CompactLeafPage() are
 *  used to compact the internal page and the leaf page, respectively.
 *
 * Exports:
 *  void edubtm_CompactInternalPage(BtreeInternal*, Two)
 *  void edubtm_CompactLeafPage(BtreeLeaf*, Two)
 */


#include <string.h>
#include "EduBtM_common.h"
#include "EduBtM_Internal.h"



/*@================================
 * edubtm_CompactInternalPage()
 *================================*/
/*
 * Function: edubtm_CompactInternalPage(BtreeInternal*, Two)
 *
 * Description:
 * (Following description is for original ODYSSEUS/COSMOS BtM.
 *  For ODYSSEUS/EduCOSMOS EduBtM, refer to the EduBtM project manual.)
 *
 *  Reorganize the internal page to make sure the unused bytes in the page
 *  are located contiguously "in the middle", between the entries and the
 *  slot array. To compress out holes, entries must be moved toward the
 *  beginning of the page.
 *
 * Returns:
 *  None
 *
 * Side effects:
 *  The leaf page is reorganized to compact the space.
 */
void edubtm_CompactInternalPage(
    BtreeInternal       *apage,                 /* INOUT internal page to compact */
    Two                 slotNo)                 /* IN slot to go to the boundary of free space */
{
	BtreeInternal       tpage;                  /* temporay page used to save the given page */
    Two                 apageDataOffset;        /* where the next object is to be moved */
    Two                 len;                    /* length of the leaf entry */
    Two                 i;                      /* index variable */
    btm_InternalEntry   *entry;                 /* an entry in leaf page */

    memcpy(&tpage, apage, PAGESIZE);

    len = 0;
    apageDataOffset = 0;
    for(i = 0; i < tpage.hdr.nSlots; i++){

        if(i != slotNo){
            entry = tpage.data + tpage.slot[-i];
            len = 4 + ALIGNED_LENGTH(2 + entry->klen); //spid + ALIGN(klen + kval)
            
            memcpy(apage->data + apageDataOffset, entry, len);
            apage->slot[-i] = apageDataOffset;

            apageDataOffset += len;
        }
    }

    if(slotNo != NIL){
        entry = tpage.data + tpage.slot[-slotNo];
        len = 4 + ALIGNED_LENGTH(2 + entry->klen);
        memcpy(apage->data + apageDataOffset, entry, len);
        apage->slot[-slotNo] = apageDataOffset;
        apageDataOffset += len;
    }

    apage->hdr.free = apageDataOffset;
    apage->hdr.unused = 0;
} /* edubtm_CompactInternalPage() */



/*@================================
 * edubtm_CompactLeafPage()
 *================================*/
/*
 * Function: void edubtm_CompactLeafPage(BtreeLeaf*, Two)
 *
 * Description:
 * (Following description is for original ODYSSEUS/COSMOS BtM.
 *  For ODYSSEUS/EduCOSMOS EduBtM, refer to the EduBtM project manual.)
 *
 *  Reorganizes the leaf page to make sure the unused bytes in the page
 *  are located contiguously "in the middle", between the entries and the
 *  slot array. To compress out holes, entries must be moved toward the
 *  beginning of the page.
 *	
 * Return Values :
 *  None
 *
 * Side Effects :
 *  The leaf page is reorganized to comact the space.
 */
void edubtm_CompactLeafPage(
    BtreeLeaf 		*apage,			/* INOUT leaf page to compact */
    Two       		slotNo)			/* IN slot to go to the boundary of free space */
{	
	BtreeLeaf 		tpage;			/* temporay page used to save the given page */
    Two                 apageDataOffset;        /* where the next object is to be moved */
    Two                 len;                    /* length of the leaf entry */
    Two                 i;                      /* index variable */
    btm_LeafEntry 	*entry;			/* an entry in leaf page */
    Two 		alignedKlen;		/* aligned length of the key length */

    memcpy(&tpage, apage, PAGESIZE);

    len = 0;
    apageDataOffset = 0;
    for(i = 0; i < tpage.hdr.nSlots; i++){

        if(i != slotNo){
            entry = tpage.data + tpage.slot[-i];
            len = 2 + 2 + ALIGNED_LENGTH(entry->klen) + sizeof(ObjectID);
            //sizeof(nObjects) + sizeof(klen) + alignedKlen + sizeof(ObjectID)

            memcpy(apage->data + apageDataOffset, entry, len);
            apage->slot[-i] = apageDataOffset;

            apageDataOffset += len;
        }
    }

    if(slotNo != NIL){
        entry = tpage.data + tpage.slot[-slotNo];
        len = 2 + 2 + ALIGNED_LENGTH(entry->klen) + sizeof(ObjectID);
        memcpy(apage->data + apageDataOffset, entry, len);
        apage->slot[-slotNo] = apageDataOffset;
        apageDataOffset += len;
    }

    apage->hdr.free = apageDataOffset;
    apage->hdr.unused = 0;

} /* edubtm_CompactLeafPage() */
