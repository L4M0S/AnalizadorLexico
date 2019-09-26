#include"scanner.h"

#include<cstdio>
#include<cctype>
#include<string>
#include<iostream>

#define udef -1

using namespace std;

int p=0,q=0;

int findearchivo=0;

int lineas=1;

string cadena;

FILE *file;

void open(char* archivo)
{
	file=fopen(archivo,"r");
	
	while(fgetc(file)!=EOF)
	{	
		//findearchivo++;
	}
	findearchivo=ftell(file);
	//printf("- findearchivo:%i -\n",findearchivo);
	//printf("- ftello:%i -\n\n",ftell(file));
	fseek(file,q,SEEK_SET);
}

void close()
{
	fclose(file);
}

int read()
{
	//printf("\np:%i q:%i\n",p,q);
	//printf("cavesal:%i\n",ftell(file));
	char r=fgetc(file);
	//printf("read:%c\n",r);
	if(r==10) p++;
	/*if(!isspace(r))	
	{
		cadena.push_back(r);
	}*/
	p++;
	return r;
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
	fseek(file,p,SEEK_SET);
}

void wsp()
{	
	//printf("--");
	/*
	while(isspace(read()))
	{
		
	}	
	
	fallback();
	success();
	//printf("--");
	//*/
	//*
	int e=read();
	while(isspace(e))
	{
		if(e==10) 
		{
			fallback();
			success();
			lineas++;
		}
		e=read();
	}
	fallback();
	success();
	//*/
}

bool eof()
{
	//*
	if(ftell(file)>=findearchivo)
	{
		//printf("true\n");
		return true;
	}
	//printf("false\n");
	return false;
	//*/
	/*
	//fgetc(file);
	if(feof(file))
	//if(fgetc(file)!=EOF)
	{
		//fseek(file,-1,SEEK_CUR);
		//fallback();
		//success();
		//printf("eof\n");
		printf("\n -oef- \n");
		return true;
	}
	//fseek(file,-1,SEEK_CUR);
	//fail();
	return false;
	//return feof(file);
	//*/
}

token identifier()
{
	int actual=0, prior=0;
	int c;
	
	while (actual>udef)
	{
		prior=actual;
		c=read();
		//printf("\nid c= %c\n",c);
		switch(actual)
		{
			case 0:
				if(c=='_') actual=1;
				else if(isalpha(c)) actual=2;                                   //((c>='a' && c<='z') || (c>='A' && c<='Z')) actual=2;
				else actual=udef;
				break;
			case 1:
				if(isdigit(c) || c=='_') actual=1;
				else if(isalpha(c)) actual=2;
				else actual=udef;
				break;
			case 2:
				if(isalpha(c) || isdigit(c) || c=='_') actual=2;
				else if(c==39) actual=3;
				else actual=udef;
				break;
			case 3:
				if(c==39) actual=3;
				else actual=udef;
				break;
		}
		//c=NULL;
		//printf("actual:%i ",actual);
	}
	//printf("\nprior:%i \n",prior);
	if(prior==2 || prior==3)
	{
		fallback();
		success();
		return _id;		
	}
	else
	{
		fail();
		return _err;
	}
}

token num()
{
	int actual=0, prior=0;
	int c=0;
	
	while (actual>udef)
	{
		prior=actual;
		c=read();
		
		//printf("\nnum c= %c\n",c);
		//printf("ACTUAL:%i ",actual);
		switch(actual)
		{
			case 0:
				//printf("case0 ");
				if(c=='0'){/*printf("actual:1 ");*/ actual=1;}
				else if(c>='1' && c<='9') {/*printf("actual:2 ");*/actual=5;}
				else { /*printf("udef ");*/actual=udef;}
				break;
			case 1:
				//printf("case1 ");
				if(c>='0' && c<='7') actual=1;
				else if(c=='x' || c=='X') actual=2;
				else if(c=='.') actual=6;
				else actual=udef;
				break;
			case 2:
				//printf("case2 ");
				if(isdigit(c) || (c>='a' && c<='f') || (c>='A' && c<='F')) actual=3;
				else actual=udef;
				break;
			case 3:
				//printf("case3 ");
				if(isdigit(c) || (c>='a' && c<='f') || (c>='A' && c<='F')) actual=4;
				else actual=udef;
				break;
			case 4:
				//printf("case4 ");
				if(isdigit(c) || (c>='a' && c<='f') || (c>='A' && c<='F')) actual=3;
				else actual=udef;
				break;
			case 5:
				//printf("case5 ");
				if(isdigit(c)) actual=5;
				else if(c=='.') actual=6;
				else if(c=='E' || c=='e') actual=8;
				else actual=udef;
				break;
			case 6:
				if(isdigit(c)) actual=7;
				else actual=udef;
				break;
			case 7:
				if(isdigit(c)) actual=7;
				else if(c=='E' || c=='e') actual=8;
				else actual=udef;
				break;
			case 8:
				if(c=='+' || c=='-') actual=9;
				else if(isdigit(c)) actual=10;
				else actual=udef;
				break;
			case 9:
				if(isdigit(c)) actual=10;
				else actual=udef;
				break;
			case 10:
				if(isdigit(c)) actual=10;
				else actual=udef;
				break;
		}
		//printf("actual:%i ",actual);
		//c=NULL;
	}
	//printf("\nprior:%i \n",prior);
	if(prior==1)
	{
		//printf("\n-octal-\n");
		fallback();
		success();
		return _octal;		
	}
	
	else if(prior==4)
	{
		//printf("\n-hexa-\n");
		fallback();
		success();
		return _hexa;		
	}
	
	else if(prior==5 || prior==7 || prior==10)
	{
		//printf("\n-real-\n");
		fallback();
		success();
		return _real;
	}
	else
	{
		fail();
		return _err;
	}
}

token next()
{
	//printf("next \n");
	//printf("final ");
	wsp();
	
	if(eof()){/*printf("fin ");*/return _eof;}
	
	//printf("blanco \n");
	
	 
	//printf("id \n");
	//else if(identifier()==_id) {/*printf("fini ");*/return _id;}
	
	//printf("int \n");
	//else if(num()==_octal) {/*printf("fine ");*/return _octal;}
	//printf("float \n");
	//else if(num()==_hexa) {return _hexa;}	
	//else if(num()==_real) {printf("\n-fin real-\n ");return _real;}
	//*
	switch(identifier())
	{
		case _id: return _id; 
			 break;
	}//*/
	//*
	switch(num())
	{
		case _octal: return _octal; 
			 break;
		case _hexa: return _hexa; 
			 break;
		case _real: return _real; 
			 break;
	}
	//*/
	
	
	//printf("\n-fin error-\n ");
	return _err;
}

void scanner()
{	
	//fseek(file,0,SEEK_SET);
	//printf("%c",fgetc(file));
	//*
	token a;
	do
	{
		//printf("otra \n");
		a=next();
		//if(a==_eof)printf("final\n");
		switch(a)
		{
			case _id:
				printf("identificador\n");
				break;
			case _octal:
				printf("octal\n");
				break;
			case _hexa:
				printf("hexadecimal\n");
				break;
			case _real:
				printf("real\n");
				break;
			case _eof:
				printf("eof\n");
				printf("Analisis finalizado. Lineas analizadas: %i \n",lineas);
				break;
			case _err:
				printf("err\n");
				printf("Analisis interrumpido por error en la linea %i \n",lineas);
				break;		
		}
		//cout<<cadena<<endl;
		//cadena=" ";
		//system("pause");
	}while(a!=_eof && a!=_err);
	//*/
}
