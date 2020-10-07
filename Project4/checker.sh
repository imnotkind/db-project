#!/bin/bash

cp EduBtM_64bit/edubtm_BinarySearch.c EduBtM_new/
cp EduBtM_64bit/edubtm_Compact.c EduBtM_new/
cp EduBtM_64bit/edubtm_Compare.c EduBtM_new/
cp EduBtM_64bit/EduBtM_CreateIndex.c EduBtM_new/
cp EduBtM_64bit/edubtm_Delete.c EduBtM_new/
cp EduBtM_64bit/EduBtM_DeleteObject.c EduBtM_new/
cp EduBtM_64bit/EduBtM_DropIndex.c EduBtM_new/
cp EduBtM_64bit/EduBtM_Fetch.c EduBtM_new/
cp EduBtM_64bit/EduBtM_FetchNext.c EduBtM_new/
cp EduBtM_64bit/edubtm_FirstObject.c EduBtM_new/
cp EduBtM_64bit/edubtm_FreePages.c EduBtM_new/
cp EduBtM_64bit/edubtm_InitPage.c EduBtM_new/
cp EduBtM_64bit/edubtm_Insert.c EduBtM_new/
cp EduBtM_64bit/EduBtM_InsertObject.c EduBtM_new/
cp EduBtM_64bit/edubtm_LastObject.c EduBtM_new/
cp EduBtM_64bit/edubtm_root.c EduBtM_new/
cp EduBtM_64bit/edubtm_Split.c EduBtM_new/

cd EduBtM_new/
make clean && make && echo -e "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" | ./EduBtM_Test
make clean