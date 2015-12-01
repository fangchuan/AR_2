#include "_apollorobot.h"
#include "stdlib.h"
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
_Listptr Ins_List_Head;//���������ͷָ��


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
//����һ���б�ͷ���ĵ�����
int Create_List(void)
{
		Ins_List_Head = (_Listptr)malloc(sizeof(_Instructor));
		if( !Ins_List_Head)
			return -1;
		
		Ins_List_Head->next = (void*)0;
		return 0;
	
}
//���ӽڵ�,��Ԫ�ؼӵ��±�Ϊindex�ĵط�����ʵ�����ν����ӱ�ͷ��������
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
					p = p ->next ;//��ʱp��Ϊ�±�Ϊindex�Ľ���ǰ���ڵ�
				
				if(!p)
					return -1;
				else{
							q -> next = p->next ;//ʹǰ���ĺ�̳�Ϊ�������ĺ��
							p->next  = q ;  //ʹ�������ĳ�Ϊǰ���ĺ��
				}
				
				return 0;
}
//�滻�±�Ϊindex�Ľ�������
int  Replace_Node(int index, enum _FLAG flag,char *content)
{
				int i ;
				_Listptr    p = Ins_List_Head;
			
				if(index <= 0 )
					return -1;

				for(i = 0;i < index  ; i++)
					p = p ->next ;//��ʱp��Ϊ�±�Ϊindex�Ľ��
				
				if(!p)
					return -1;
				else{
							p->_flag  = flag ;
							p->EditContent = content;
				}
				
				return 0;

}

//ɾ���±�ΪIndex�Ľ��
int Delete_Node(int index)
{
		int i;
		_Listptr p = Ins_List_Head;
		_Listptr q;
	
		if(index <= 0)
			return -1;
		
		for(i = 0; i< index - 1;i++)
				p = p -> next;//��ʱpΪindex����ǰ�����
		if(!p)
		{
			return -1;
		}
		else
		{
			q = p->next ;//q��㼴ΪIndex���
			p -> next = q -> next;//ʹindex���ĺ�̳�Ϊp�ĺ��
			free(q);  //�ͷ�index���Ŀռ�
		}
}
/*******************************************End of File***********************************************/