#include"scanner.h"

#include<cstdio>
#include<cctype>

#define udef -1

int p=0,q=0;

int endoffile=0;

FILE *file;

void open()
{
	file=fopen("test.txt","r");
	
	while(fgetc(file)!=EOF)
	{	
		endoffile++;
	}
	
	fseek(file,q,SEEK_SET);
}

void close()
{
	fclose(file);
}

int read()
{
	p++;
	return fgetc(file);
}

void fail()
{
	p=q;
	fseek(file,q,SEEK_SET);
}

void success()
{
	q=p;
}

void fallback()
{
	p--;
	fseek(file,-1,SEEK_CUR);
}

void wsp()
{
	while(isspace(read()))
	{
	
	}	
	
	fallback();
	success();
	
}

bool eof()
{
	if(p>=endoffile)
	{
		return true;
	}
	return false;
	/*
	//fgetc(file);
	if(feof(file))
	//if(fgetc(file)!=EOF)
	{
		//fseek(file,-1,SEEK_CUR);
		//fallback();
		//success();
		//printf("eof\n");
		return true;
	}
	//fseek(file,-1,SEEK_CUR);
	//fail();
	return false;
	//return feof(file);
	*/
}

bool identifier()
{
	int actual=0, prior=0;
	char c;
	
	while (actual!=udef)
	{
		prior=actual;
		c=read();
		printf(" %i i%c",p,c);
		switch(actual)
		{
			case 0:
				if(isalpha(c)) actual=2;
				else if(c=='_') actual=1;
				else actual=udef;
				break;
			case 1:
				if(isdigit(c) || c=='_') actual=1;
				else if(isalpha(c)) actual=2;
				else actual=udef;
				break;
			case 2:
				if(isalpha(c) || isdigit(c) || c=='_') actual=2;
				else actual=udef;
				break;
		}
		c=NULL;
	}
	
	if(prior==2)
	{
		fallback();
		success();
		return true;		
	}
	
	fail();
	return false;
}

token num()
{
	int actual=0, prior=0;
	char n;
	
	while (actual!=udef)
	{
		prior=actual;
		
		n=read();
		
		printf(" %i n%c",p,n);
		printf("ACTUAL:%i ",actual);
		switch(actual)
		{
			case 0:
				printf("case0 ");
				if(n>='1' && n<='9'){printf("actual:1 "); actual=1;}
				else if(n=='0') {printf("actual:2 ");actual=2;}
				else { printf("udef ");actual=udef;}
				break;
			case 1:
				printf("case1 ");
				if(isdigit(n)) actual=1;
				else if(n=='.') actual=3;
				else actual=udef;
				break;
			case 2:
				printf("case2 ");
				if(n=='.') actual=3;
				else if(isdigit(n)) actual=4;
				else actual=udef;
				break;
			case 3:
				printf("case3 ");
				if(isdigit(n)) actual=5;
				else actual=udef;
				break;
			case 4:
				printf("case4 ");
				if(isdigit(n)) actual=4;
				else if(n=='.') actual=3;
				else actual=udef;
				break;
			case 5:
				printf("case5 ");
				if(isdigit(n)) actual=5;
				else actual=udef;
				break;
		}
		printf("A:%i ",actual);
		n=NULL;
	}
	
	if(prior==1 || prior==2)
	{
		fallback();
		success();
		return _entero;		
	}
	
	if(prior==5)
	{
		fallback();
		success();
		return _real;
	}
	
	fail();
	return _err;
}

token next()
{
	wsp();
	
	if(identifier()) return _id;
	if(num()==_entero) return _entero;
	if(num()==_real) return _real;
	
	if(eof()) return _eof;
	
	return _err;
}

void scanner()
{	
	token a;
	do
	{
		a=next();
		switch(a)
		{
			case _id:
				printf("identificador\n");
				break;
			case _entero:
				printf("entero\n");
				break;
			case _real:
				printf("real\n");
				break;
			case _eof:
				printf("eof\n");
				break;
			case _err:
				printf("err\n");
				break;		
		}	
	}while(!eof() && a!=_err);
}
