#include "_apollorobot.h"
#include "stdlib.h"
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
_Listptr Ins_List_Head;//程序链表的头指针


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
//建立一个有表头结点的单链表
int Create_List(void)
{
		Ins_List_Head = (_Listptr)malloc(sizeof(_Instructor));
		if( !Ins_List_Head)
			return -1;
		
		Ins_List_Head->next = (void*)0;
		return 0;
	
}
//增加节点,将元素加到下标为index的地方。其实是依次将结点从表头结点接下来
int Add_Node(int index, enum _FLAG flag, char *content)
{
				int i ;
				_Listptr    q = (_Listptr)malloc(sizeof(_Instructor));
				_Listptr    p = Ins_List_Head;
			
				if(index <= 0 || !q)
					return -1;
				
				q -> EditContent = content;
				q -> _flag = flag;
				
				for(i = 0;i < index - 1; i++)
					p = p ->next ;//此时p即为下标为index的结点的前驱节点
				
				if(!p)
					return -1;
				else{
							q -> next = p->next ;//使前驱的后继成为新增结点的后继
							p->next  = q ;  //使新增结点的称为前驱的后继
				}
				
				return 0;
}
//替换下标为index的结点的内容
int  Replace_Node(int index, enum _FLAG flag,char *content)
{
				int i ;
				_Listptr    p = Ins_List_Head;
			
				if(index <= 0 )
					return -1;

				for(i = 0;i < index  ; i++)
					p = p ->next ;//此时p即为下标为index的结点
				
				if(!p)
					return -1;
				else{
							p->_flag  = flag ;
							p->EditContent = content;
				}
				
				return 0;

}

//删除下标为Index的结点
int Delete_Node(int index)
{
		int i;
		_Listptr p = Ins_List_Head;
		_Listptr q;
	
		if(index <= 0)
			return -1;
		
		for(i = 0; i< index - 1;i++)
				p = p -> next;//此时p为index结点的前驱结点
		if(!p)
		{
			return -1;
		}
		else
		{
			q = p->next ;//q结点即为Index结点
			p -> next = q -> next;//使index结点的后继成为p的后继
			free(q);  //释放index结点的空间
		}
}
/*******************************************End of File***********************************************/