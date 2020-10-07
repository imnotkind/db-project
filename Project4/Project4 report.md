# Project4 report

20160463 성해빈

## 구현한 함수

### EduBtM_CreateIndex()
### EduBtM_DropIndex()
### EduBtM_InsertObject()
### EduBtM_DeleteObject()
### EduBtM_Fetch()
### EduBtM_FetchNext()

### edubtm_InitLeaf()
### edubtm_InitInternal()
### edubtm_FreePages()
### edubtm_Insert()
### edubtm_InsertLeaf()
### edubtm_InsertInternal()
### edubtm_SplitLeaf()
### edubtm_SplitInternal()
### edubtm_root_insert()
### edubtm_Delete()
### edubtm_DeleteLeaf()
### edubtm_CompactLeafPage()
### edubtm_CompactInternalPage()
### edubtm_Fetch()
### edubtm_FetchNext()
### edubtm_FirstObject()
### edubtm_LastObject()
### edubtm_BinarySearchLeaf()
### edubtm_BinarySearchInternal()
### edubtm_KeyCompare()


## 느낀 점 discussion

이건 뭐 아무 생각도 안 들고 그냥 양이 살인적이었다.  
구현해야 하는 internal function 리스트를 보고 정말 울 뻔했다.  

catObjForFile 이 sm_SysTable을 가리키는지 sm_Btree를 가리키는지 확실하게 알려줬으면 한다 (매뉴얼에 있기는 한데 너무 조그맣게 있고 코드에도 있었으면..)  
이건 살짝 project3에서 sm_catoverlayfordata 가 object 형식으로 있는지 몰랐던 거랑 상황이 비슷하다. 그래서 더 화난다.  

Btm_LeafEntry가 ObjectId를 통으로 저장한다는걸 좀 더 명시적으로 알려줬으면 좋았겠다. ppt 그림은 너무 애매하다. 진짜 말 그대로 깨지면서 깨달았다.  

총체적으로 끔찍한 로우레벨 악몽 같았다. B+tree는 안 그래도 복잡한데 C까지 겹쳐서 너무나도 힘들었다.  

