#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct symboleTable
{
	char var;
	int addrs;
	int size;
	struct symboleTable* next;
};
struct inter
{
	int op;
	int *par;
	struct inter* next;
};
struct lable
{
	char ch[5];
	int ind;
	struct lable* next;
};
int* memory;
char** opTable;
void displayLL(struct symboleTable* head)
{
	while(head!=NULL)
	{
		printf("%c,%d,%d\n",head->var,head->addrs,head->size);
		head=head->next;
	}
}
void displayInterLL(struct inter* head)
{
	int i,count=0;
	while(head!=NULL)
	{
		printf("%d ",head->op);
		for(i=0;i<10;i++)
			printf("%d ",head->par[i]);
		printf("\n");
		head=head->next;
		count++;
	}
}
void displayLableLL(struct lable* head)
{
	while(head!=NULL)
	{
		printf("%s,%d\n",head->ch,head->ind);
		head=head->next;
	}
}
struct symboleTable* createLL(struct symboleTable* head,char ch,int adr,int size)
{
	struct symboleTable* new_node=(struct symboleTable*)malloc(sizeof(struct symboleTable));
	struct symboleTable* temp=head;
	new_node->var=ch;
	new_node->size=size;
	new_node->addrs=adr;
	new_node->next=NULL;
	
	if(head==NULL)
		head=new_node;
	else
	{
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=new_node;
	}
	return head;
}
struct inter* createInterLL(struct inter* head,int code,int* arr)
{
	struct inter* new_node=(struct inter*)malloc(sizeof(struct inter));
	struct inter* temp=head;
	new_node->op=code;
	new_node->par=arr;
	new_node->next=NULL;
	if(head==NULL)
		head=new_node;
	else
	{
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=new_node;
	}
	return head;
}
struct lable* createLableLL(struct lable* head,char* ch,int count)
{
	struct lable* new_node=(struct lable*)malloc(sizeof(struct lable));
	struct lable* temp=head;
	strcpy(new_node->ch,ch);
	new_node->ind=count;
	new_node->next=NULL;
	if(head==NULL)
		head=new_node;
	else
	{
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=new_node;
	}
	return head;
}
void displayMem()
{
	int i;
	for(i=0;i<10;i++)
		printf("%d ",memory[i]);
}
void mem(int addrs,int val)
{
	memory[addrs-1]=val;
}
struct symboleTable* setSymboleTable(struct symboleTable* head,char* buff)
{
	int i,k=0,size,val;
	static int adr=0;
	char var;
	char* str=(char*)malloc(30*sizeof(char)),*s=(char*)malloc(30*sizeof(char));
	memory=(int*)malloc(10*sizeof(int));
	for(i=0;i<10;i++)
		memory[i]=-1;
	for(i=0;buff[i]!=' ';i++,k++)
		str[k]=buff[i];
	str[k]='\0';
	if(strcmp(str,"DATA")==0)
	{
		var=buff[5];
		if(strlen(buff)==6)
			size=1;
		else
		{
			for(i=7,k=0;buff[i]!=']';i++,k++)
				s[k]=buff[i];
			s[k]='\0';
			size=atoi(s);
		}
		head=createLL(head,var,adr,size);
		adr+=size;
	}
	else if(strcmp(str,"CONST")==0)
	{
		var=buff[6];
		for(i=8,k=0;str[i]!='\0';i++,k++)
				s[k]=str[i];
		val=atoi(s);
		mem(adr,val);
		head=createLL(head,var,adr,val);
		adr+=val;
	}
	return head;
}
int searchInSybTable(char ch,struct symboleTable* head)
{
	struct symboleTable* temp=head;
	while(temp!=NULL)
	{
		if(temp->var==ch)
			return temp->addrs;
		temp=temp->next;
	}
	return 0;
}
void displayOpTable()
{
	int i;
	for(i=0;i<16;i++)
		printf("%d. %s\n",i+1,opTable[i]);
}
void createOp()
{
	strcpy(opTable[0],"MOV");
	strcpy(opTable[1],"MOV");
	strcpy(opTable[2],"ADD");
	strcpy(opTable[3],"SUB");
	strcpy(opTable[4],"MUL");
	strcpy(opTable[5],"JUMP");
	strcpy(opTable[6],"IF");
	strcpy(opTable[7],"EQ");
	strcpy(opTable[8],"LT");
	strcpy(opTable[9],"GT");
	strcpy(opTable[10],"LTEQ");
	strcpy(opTable[11],"GTEQ");
	strcpy(opTable[12],"PRINT");
	strcpy(opTable[13],"READ");
	strcpy(opTable[14],"ENDIF");
	strcpy(opTable[15],"ELSE");
}
void push(int* stack,int line,int* top)
{
	stack[++(*top)]=line;
}
int pop(int* stack,int* top)					
{
	return stack[(*top)--];
}
void showStack(int* stack,int top)
{
	if(top==-1)
		puts("Present Stack is empty");
	while(top>-1)
	{
		printf("%d \n",stack[top]);
		top--;
	}
}
void updateInter(struct inter* head,int x,int y)
{
	struct inter* temp=head;
	int count=0,i;
	while(temp!=NULL)
	{
		count++;
		if(count==y-1)
		{
			for(i=0;i<10;i++)
			{
				if(temp->par[i]==-2)
					temp->par[i]=x+1;
			}
		}
		temp=temp->next;
	}
}
int stack[10];
int top=-1;
int searchLable(struct lable* head,char* str)
{
	displayLableLL(head);
	struct lable* temp=head;
	while(temp!=NULL)
	{
		if(!strcmp(temp->ch,str))
			return temp->ind;
	}
	return 0;
}
struct lable* lableHead=NULL;
struct inter* execute(char* buff,struct symboleTable* head,struct inter* interHead)
{
	static int count=0;
	count++;
	int i,j,k,addrs,val1,val2,val3;
	int *arr=(int*)malloc(10*sizeof(int));
	char *str=(char*)malloc(30*sizeof(char));
	for(i=0;i<10;i++)
		arr[i]=-1;
	if(buff[1]==':')
	{
		str[0]=buff[0];
		str[1]='\0';
		lableHead=createLableLL(lableHead,str,count);
	}
	switch(buff[0])
	{
		case 'M':
				i=0;
				while(buff[i]!=' ')i++;
				while(buff[i]==' ')i++;
				if(buff[i+1]=='X')
				{
					addrs=searchInSybTable(buff[i+3],head);
					arr[0]=buff[i]-'A';
					arr[1]=addrs;
					interHead=createInterLL(interHead,1,arr);
				}
				else if(buff[i+1]==','||buff[i+1]=='[')
				{
					addrs=searchInSybTable(buff[i],head);
					arr[0]=addrs;
					if(buff[i+1]==',')
					{
						arr[1]=buff[i+3]-'A';
						interHead=createInterLL(interHead,2,arr);
					}
					else
						interHead=createInterLL(interHead,2,arr);
				}
				break;
		case 'R':
				i=0;
				while(buff[i]!=' ')i++;
				while(buff[i]==' ')i++;
				arr[0]=buff[i]-'A';
				interHead=createInterLL(interHead,14,arr);
				break;
		case 'A':
				i=0;
				while(buff[i]!=' ')i++;
				while(buff[i]==' ')i++;
				arr[0]=buff[i]-'A';
				arr[1]=buff[i+4]-'A';
				arr[2]=buff[i+8]-'A';
				interHead=createInterLL(interHead,3,arr);
				break;
		case 'S':
				i=0;
				while(buff[i]!=' ')i++;
				while(buff[i]==' ')i++;
				arr[0]=buff[i]-'A';
				arr[1]=buff[i+4]-'A';
				arr[2]=buff[i+8]-'A';
				interHead=createInterLL(interHead,4,arr);
				break;
		case 'P':
				i=0;
				while(buff[i]!=' ')i++;
				while(buff[i]==' ')i++;
				if(buff[i+1]=='X')
				{
					arr[0]=buff[i]-'A';
					interHead=createInterLL(interHead,12,arr);
				}
				else
				{
					addrs=searchInSybTable(buff[i],head);
					arr[0]=addrs;
					interHead=createInterLL(interHead,12,arr);
				}
				break;
		case 'I':i=0;
				k=0;
				arr[0]=buff[3]-'A';
				i=6;
				while(buff[i]!=' ')
				{
					str[k++]=buff[i];
					i++;
				}
				str[k]='\0';
				for(j=0;j<16;j++)
					if(!strcmp(opTable[j],str))
						arr[2]=j+1;
				arr[1]=buff[i+1]-'A';
				arr[3]=-2;
				interHead=createInterLL(interHead,7,arr);
				if(top==-1)
					push(stack,count,&top);
				else
				{
					push(stack,count,&top);
					lableHead=createLableLL(lableHead,"IF",count);
				}
				break;
		case 'E':if(buff[1]=='L')
				{
					arr[0]=-2;
					interHead=createInterLL(interHead,16,arr);
					push(stack,count,&top);
					showStack(stack,top);
					lableHead=createLableLL(lableHead,"ELSE",count);
				}
				else if(buff[1]=='N'&&buff[3]=='I')
				{
						
						showStack(stack,top);
						val1=count;
						val2=pop(stack,&top);
						updateInter(interHead,val1,val2);
						val3=pop(stack,&top);
						updateInter(interHead,val2,val3);
				}
				break;
		case 'J':	str[0]=buff[5];
					str[1]='\0';
					val1=searchLable(lableHead,str);
					arr[0]=val1;
					interHead=createInterLL(interHead,5,arr);
				
	}
	return interHead;
}
int main()
{
	FILE* fp;
	int i,k;
	fp=fopen("program.txt","r");
	char* buff=(char*)malloc(255*sizeof(char));
	char* str=(char*)malloc(255*sizeof(char));
	opTable=(char**)malloc(16*sizeof(char*));
	for(i=0;i<16;i++)
		opTable[i]=(char*)malloc(20*sizeof(char));
	createOp();
	struct symboleTable* head=NULL;
	struct inter* interHead=NULL;
	while(fgets(buff, 255, fp)!=NULL)
	{
		buff[strlen(buff)-1]='\0';
		if(strcmp(buff,"START:")==0)
			break;
		head=setSymboleTable(head,buff);
	}
	while(fgets(buff, 255, fp)!=NULL)
	{
		buff[strlen(buff)-1]='\0';
		i=0;
		while(buff[i]=='\t')i++;
		for(k=0;buff[i]!='\0';i++,k++)
			str[k]=buff[i];
		str[k]='\0';
		interHead=execute(str,head,interHead);
	}
	puts("\n----------------------------\n");
	puts("Op Code Table\n");
	displayOpTable();
	puts("\n----------------------------\n");
	puts("Symbole Table\n");
	displayLL(head);
	puts("\n----------------------------\n");
	puts("InterMediate Language Table\n");
	displayInterLL(interHead);
	puts("\n----------------------------\n");
	puts("Memory Allocation Table\n");
	displayMem();
	puts("\n----------------------------\n");
	puts("Lable Table\n");
	displayLableLL(lableHead);
	puts("\n----------------------------\n");
	puts("Stack\n");
	showStack(stack,top);
	return 0;
}
