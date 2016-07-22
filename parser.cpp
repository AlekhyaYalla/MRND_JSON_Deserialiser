#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
typedef struct phoneNumber
{
	char type[10];
	int number[10];
};
typedef struct record
{
	char firstname[20];
	char lastname[20];
	char height[3];
	int age;
	phoneNumber phnBook;
};
typedef struct node
{
	record* rec;
	node* next;
};

float get_avg_age(record *,int);
FILE* json_record(FILE *, struct record *);
FILE* set_record(FILE *,record *,int );
float get_int(FILE *,int ,char);
int exponent(int );
int if_firstname(char *);
int if_lastname(char *);
int if_height(char *);
int if_age(char *);
int if_phonenumber(char *);
char* get_height(FILE *,int,char);
void to_array(record *,int ,record *);
node* to_list(node *,record *);
FILE* get_phonerecord(FILE *,record *,char );
FILE* phone_record(FILE *,phoneNumber ,char,record *);
node* queries(node *);
void main()
{
	FILE *fp;
	errno_t e;
	char *s1,*s2,ch;
	int i=0,k=0;
	float age;
	node *head,*curr;
	head=(node*)malloc(sizeof(node));
	curr=(node*)malloc(sizeof(node));
	s1=(char*)malloc(sizeof(s1)*10);
	s2=(char*)malloc(sizeof(s1)*10);
	e=fopen_s(&fp,"D://file11.txt","r+");
	while(!feof(fp))
	{
		ch=fgetc(fp);
		while(ch==' '&&ch!='{'&&ch=='\n')
			ch=fgetc(fp);
		if(ch=='{')
		{
			record *r;
			r=(record*)malloc(sizeof(record)*1);
			fp=json_record(fp,r);
			if(k==0)
			{
				head=to_list(head,r);
				curr=head;
				k++;
			}
			else
			{
				node *temp;
				temp=(node*)malloc(sizeof(node));
				temp=to_list(temp,r);
				curr->next=temp;
				curr=temp;
			}
		}
	}
	/*curr=head;
	while(curr!='\0')
	{
		//printf("\n\n%s\t%s\t%s\t%d\n%s: ",curr->rec->firstname,curr->rec->lastname,curr->rec->height,curr->rec->age,curr->rec->phnBook.type);
		i=0;
		while(i<10)
		{
			printf("%d",curr->rec->phnBook.number[i]);
			i++;
		}
		curr=curr->next;
	}*/
	head=queries(head);
	_getch();
}
node* queries(node *head)
{
	int n,i;
	char *name;
	node *temp;
	name = (char*)malloc(sizeof(char));
	printf("\n\n1 Phone number\t2 Surname\t3 Age\t4 Height\t5 Exit\n");
	scanf("%d", &n);
	if (n == 5)
		return head;
	printf("\nEnter name:");
	fflush(stdin);
	gets(name);
	temp = head;
	while (temp != '\0')
	{
		i=strcmp(name, temp->rec->firstname);
		if (i == 0)
		{
			switch (n)
			{
			case 1:
				printf("%s: ", temp->rec->phnBook.type );
				i = 0;
				while (i < 10)
					printf("%d", temp->rec->phnBook.number[i++]);
				break;
			case 2:
				printf("Surname: %s\n", temp->rec->lastname);
				break;
			case 3:
				printf("Age: %d\n", temp->rec->age);
				break;
			case 4:
				printf("Height: %s\n", temp->rec->height);
				break;
			}
			return head;
		}
		else
			temp = temp->next;
	}
}
node* to_list(node *temp,record *rec)
{
	temp->rec=rec;
	temp->next='\0';
	return temp;
}
float get_avg_age(record *arr,int n)
{
	int i=0;
	float s=0;
	while(i<n)
	{
		s+=arr[i].age;
		i++;
	}
	return s/n;
}

FILE* json_record(FILE *fp,record *r)
{
	int i;
	char ch,*s1;
	s1=(char*)malloc(sizeof(char)*11);
	ch=fgetc(fp);
	while(ch!='}'&&ch!=EOF)
	{
		while((ch=='\n'||ch=='\t'||ch=='"'||ch==',')&&(ch!='}'))
			ch=fgetc(fp);
		i=0;
		while(ch!='"'&&ch!='}')
		{
			s1[i++]=ch;
			ch=fgetc(fp);
		}
		s1[i]='\0';
		if (if_firstname(s1)==1)
			fp=set_record(fp,r,1);
		else if(if_lastname(s1)==1)
			fp=set_record(fp,r,2);
		else if(if_height(s1)==1)
			fp=set_record(fp,r,3);
		else if(if_age(s1)==1)
			fp=set_record(fp,r,4);
		else if(if_phonenumber(s1)==1)
			fp=set_record(fp,r,5);
		while((ch==','||ch==' ')&&ch!='"'&&ch!='}')
			ch=fgetc(fp);
	}
	return fp;
}
FILE* set_record(FILE *fp,record *r,int id)
{
	char ch;
	int i=0;
	ch=fgetc(fp);
	while(ch!='"'&&id<3)
		ch=fgetc(fp);
	if(id<3)
		ch=fgetc(fp);
	while(ch!='"'&&id<3)
	{
		if(id==1)
			r->firstname[i]=ch;
		else
			r->lastname[i]=ch;
		ch=fgetc(fp);
		i++;
	}
	if(id==1)
		r->firstname[i]='\0';
	else if(id==2)
		r->lastname[i]='\0';
	if(id==3)
	{
		ch=fgetc(fp);
		if(ch==':')
			ch=fgetc(fp);

		while(i<3)
		{
			r->height[i++]=ch;
			ch=fgetc(fp);
		}
		r->height[i]='\0';
	}
	else if(id==4)
	{
		ch=fgetc(fp);
		if(ch==':')
			ch=fgetc(fp);
		r->age=int(get_int(fp,id,ch));
	}
	else if(id==5)
	{
		if(ch==':')
			ch=fgetc(fp);
		fp=get_phonerecord(fp,r,ch);
	}
	return fp;
}
FILE* get_phonerecord(FILE *fp,record *r,char ch)
{
	char *s1,*s2;
	int i=0,j;
	while(ch==' '||ch=='\t'||ch=='\n'||ch=='[')
		ch=fgetc(fp);
	while(ch!=']')
	{
		ch=fgetc(fp);
		if(ch!='"'&&ch!=']')
		{
			phoneNumber p;
			memset(&p,'\0',sizeof(phoneNumber));
			fp=phone_record(fp,p,ch,r);
		}
	}
	return fp;
}
FILE* phone_record(FILE *fp,phoneNumber p,char ch,record *r)
{
	int i=0;
	while(ch!='"'&&ch!=']')
	{
		p.type[i++]=ch;
		ch=fgetc(fp);
	}
	p.type[i]='\0';
	i=0;
	while(ch==':'||ch=='"')
		ch=fgetc(fp);
	while(ch!=']'&&ch!='\n')
	{
		p.number[i++]=ch-48;
		ch=fgetc(fp);
	}
	r->phnBook=p;
	ch=fgetc(fp);
	return fp;
}

char* get_height(FILE *fp,int id,char ch)
{
	int i=0;
	char *s;
	s=(char*)malloc(sizeof(char)*3);
	while(i<3)
	{
		s[i++]=ch;
		ch=fgetc(fp);
		
	}
	s[i]='\0';
	return s;
}

float get_int(FILE *fp,int id, char ch)
{
	int s=0,i=0,j,k=0;
	float s1=0;
	if(ch=='.')
		ch=fgetc(fp);
	while(ch!=','&&ch!='.'&&ch!='\n')
	{
		i++;
		ch=fgetc(fp);
	}
	fseek(fp,-(i+1),SEEK_CUR);
	j=i;
	while(i>0)
	{
		ch=fgetc(fp);
		s+=((ch-48)*exponent(i-1));
		i--;
	}
	ch=fgetc(fp);
	return s;
}
int exponent(int i)
{
	int res=1;
	while(i>0)
	{
		i--;
		res*=10;
	}
	return res;
}
int if_firstname(char *s1)
{
	if((s1[0]=='f')&&(s1[1]=='i')&&(s1[2]=='r')&&(s1[3]=='s')&&(s1[4]=='t')&&(s1[5]=='n')&&(s1[6]=='a')&&(s1[7]=='m')&&(s1[8]=='e'))
		return 1;
	else 
		return 0;
}
int if_lastname(char *s1)
{
	int i=0;
	if((s1[0]=='l'||s1[0]=='L')&&(s1[1]=='a')&&(s1[2]=='s')&&(s1[3]=='t')&&(s1[4]=='n'||s1[4]=='N')&&(s1[5]=='a')&&(s1[6]=='m')&&(s1[7]=='e'))
		return 1;
	else 
		return 0;
}
int if_height(char *s1)
{
	int i=0;
	if((s1[0]=='h'||s1[0]=='H')&&(s1[1]=='e')&&(s1[2]=='i')&&(s1[3]=='g')&&(s1[4]=='h')&&(s1[5]=='t'))
		return 1;
	else 
		return 0;
}
int if_age(char *s1)
{
	int i=0;
	if((s1[0]=='a'||s1[0]=='A')&&(s1[1]=='g')&&(s1[2]=='e'))
		return 1;
	else 
		return 0;
}
int if_phonenumber(char *s1)
{
	int i=0;
	if((s1[0]=='p'||s1[0]=='P')&&(s1[1]=='h')&&(s1[2]=='o')&&(s1[3]=='n')&&(s1[4]=='e')&&(s1[5]=='n')&&(s1[6]=='u')&&(s1[7]=='m')&&(s1[8]=='b')&&(s1[9]=='e')&&(s1[10]=='r'))
		return 1;
	else 0;
}
void to_array(record *arr,int n,record *r)
{
	int i,j,k;
	i=0;
	while(r->firstname[i]!='\0')
	{
		arr[n].firstname[i]=r->firstname[i];
		i++;
	}
	arr[n].firstname[i]='\0';
	i=0;
	while(r->lastname[i]!='\0')
	{
		arr[n].lastname[i]=r->lastname[i];
		i++;
	}
	arr[n].lastname[i]='\0';
	i=0;
	while(r->height[i]!='\0')
	{
		arr[n].height[i]=r->height[i];
		i++;
	}
	arr[n].height[i]='\0';
	arr[n].age=r->age;
}












		


