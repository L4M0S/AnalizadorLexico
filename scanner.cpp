#include"scanner.h"

#include<cstdio>
#include<cctype>
#include<string>
#include<iostream>
#include<map>
#include<set>
#include <iterator>

#define udef -1

using namespace std;

typedef std::map <int,string> Diccionario;

Diccionario reservadas;

void llenarDiccionario()
{
	reservadas[0]="identity";
	reservadas[1]="transpose";
	reservadas[2]="throw";
}

int p=0,q=0;

int findearchivo=0;

int lineas=1;

string cadena="";

FILE *file;

void agregar(char a)
{
	cadena=cadena+a;
}

void open(char* archivo)
{
	file=fopen(archivo,"r");
	/*
	while(fgetc(file)!=EOF)
	{	
	  	//findearchivo++;
	}
	*/
	//findearchivo=ftell(file);
	fseek(file,0,SEEK_END);
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
	if(r==10) {p++;}//printf("salto de linea\n");lineas++;
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
			//printf("\nsalto de linea\n");
			lineas++;
			//cout<<endl<<lineas<<endl;
			fallback();
			success();			
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
	cout<<endl<<ftell(file)<<" "<<findearchivo<<endl;
	if(ftell(file)>=findearchivo || feof(file) )
	//if(read()==EOF)
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

token identificador()
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
				else if(isalpha(c)) actual=2;                                   
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 1:
				if(isdigit(c) || c=='_') actual=1;
				else if(isalpha(c)) actual=2;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 2:
				if(isalpha(c) || isdigit(c) || c=='_') actual=2;
				else if(c==39) actual=3;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 3:
				if(c==39) actual=3;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
		}
		//c=NULL;
		//printf("actual:%i ",actual);
		if(actual!=udef)agregar(c);
	}
	//printf("\nprior:%i \n",prior);
	if(prior==2 || prior==3)
	{
		for(int i=0;i<reservadas.size();i++)
		{
			if(cadena==reservadas[i])
			{
				fallback();
				success();
				return _reservada;	
			}
		}
		fallback();
		success();
		return _id;		
	}
	else
	{
		cadena="";
		fail();
		return _error;
	}
}

token numero()
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
				else if((isalpha(c) && (c!='x'||c!='X')) || c=='_') actual=11; 
				else { /*printf("udef ");*/actual=udef;}
				//if(!isspace(c))agregar(c);
				break;
			case 1:
				//printf("case1 ");
				if(c>='0' && c<='7') actual=1;
				else if(c=='x' || c=='X') actual=2;
				else if(c=='.') actual=6;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 2:
				//printf("case2 ");
				if(isdigit(c) || (c>='a' && c<='f') || (c>='A' && c<='F')) actual=3;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 3:
				//printf("case3 ");
				if(isdigit(c) || (c>='a' && c<='f') || (c>='A' && c<='F')) actual=4;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 4:
				//printf("case4 ");
				if(isdigit(c) || (c>='a' && c<='f') || (c>='A' && c<='F')) actual=3;
				else if((isalpha(c) && ((c>='a' && c<='f')||(c>='A' && c<='F'))) || c=='_') actual=11;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 5:
				//printf("case5 ");
				if(isdigit(c)) actual=5;
				else if(c=='.') actual=6;
				else if(c=='E' || c=='e') actual=8;
				else if((isalpha(c) && (c!='e'||c!='E')) || c=='_') actual=11;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 6:
				if(isdigit(c)) actual=7;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 7:
				if(isdigit(c)) actual=7;
				else if(c=='E' || c=='e') actual=8;
				else if((isalpha(c) && (c!='e'||c!='E')) || c=='_') actual=11;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 8:
				if(c=='+' || c=='-') actual=9;
				else if(isdigit(c)) actual=10;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 9:
				if(isdigit(c)) actual=10;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 10:
				if(isdigit(c)) actual=10;
				else if((isalpha(c) && (c!='e'||c!='E')) || c=='_') actual=11;
				else actual=udef;
				//if(!isspace(c))agregar(c);
				break;
			case 11:
				actual=udef;
				break;
		}
		//printf("actual:%i ",actual);
		//c=NULL;
		if(actual!=udef)agregar(c);
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
		cadena="";
		fail();
		return _error;
	}
}

token comentario()
{
	int actual=0, prior=0;
	int c=0;
	
	while (actual>udef)
	{
		prior=actual;
		c=read();
		
		//printf("\ncom c= %c\n",c);
		//printf("ACTUAL:%i \n",actual);
		switch(actual)
		{
			case 0:
				//printf("case0 ");
				if(c=='#'){/*printf("actual:1 ");*/ actual=1;}
				else { /*printf("udef ");*/actual=udef;}
				break;
			case 1:
				//printf("case1 ");
				if(c!=10) actual=1;
				else if(c==10) {actual=2;lineas++;}//cout<<endl<<"c"<<lineas<<endl;
				else actual=udef;
				break;
			case 2:
				actual=udef;
				break;
		}
		//printf("actual:%i ",actual);
		//c=NULL;
	}
	//printf("\nprior:%i \n",prior);
	if(prior==2)
	{
		//printf("\n-octal-\n");
		fallback();
		success();
		return _comentario;		
	}
	else
	{
		fail();
		return _error;
	}
}

token especiales()
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
				if(c=='(') actual=1;
				else if(c==')') actual=2;
				else if(c=='[') actual=3; 
				else if(c==']') actual=4;
				else if(c=='+') actual=5;
				else if(c=='-') actual=6;
				else if(c=='*') actual=7;
				else if(c=='/') actual=8;
				else if(c==',') actual=9;
				else if(c==';') actual=10;
				else if(c==':') actual=11;
				else actual=udef;
				
				break;
			case 1:
				actual=udef;
				break;
			case 2:
				actual=udef;
				break;
			case 3:
				actual=udef;
				break;
			case 4:
				actual=udef;
				break;
			case 5:
				actual=udef;
				break;
			case 6:
				actual=udef;
				break;
			case 7:
				actual=udef;
				break;
			case 8:
				actual=udef;
				break;
			case 9:
				actual=udef;
				break;
			case 10:
				actual=udef;
				break;
			case 11:
				actual=udef;
				break;
		}
		//printf("actual:%i ",actual);
		//c=NULL;
		if(actual!=udef)agregar(c);
	}
	//printf("\nprior:%i \n",prior);
	if(prior==1 || prior==2 || prior==3 || prior==4)
	{
		//printf("\n-octal-\n");
		fallback();
		success();
		return _delimitacion;		
	}	
	else if(prior==5 || prior==6 || prior==7 || prior==8)
	{
		//printf("\n-hexa-\n");
		fallback();
		success();
		return _aritmetico;		
	}	
	else if(prior==9 || prior==10)
	{
		//printf("\n-real-\n");
		fallback();
		success();
		return _puntuacion;
	}
	else if(prior==11)
	{
		//printf("\n-real-\n");
		fallback();
		success();
		return _asignacion;
	}	
	else
	{
		cadena="";
		fail();
		return _error;
	}
}

token next()
{
	//printf("next \n");
	//printf("final ");
	
	
	wsp();
	
	switch(comentario())
	{
		case _comentario: return _comentario; 
			break;
	}
	
	
		
	//printf("blanco \n");
	
	//printf("id \n");
	//else if(identifier()==_id) {/*printf("fini ");*/return _id;}
	
	//printf("int \n");
	//else if(num()==_octal) {/*printf("fine ");*/return _octal;}
	//printf("float \n");
	//else if(num()==_hexa) {return _hexa;}	
	//else if(num()==_real) {printf("\n-fin real-\n ");return _real;}
	//*
	switch(identificador())
	{
		case _reservada:  return _reservada;
			break;
		case _id: return _id; 
			break;
	}//*/
	//*
	switch(numero())
	{
		case _octal: return _octal; 
			break;
		case _hexa: return _hexa; 
			break;
		case _real: return _real; 
			break;
	}
	
	switch(especiales())
	{
		case _delimitacion: return _delimitacion; 
	 		break;
		case _aritmetico: return _aritmetico; 
	        break;
		case _puntuacion: return _puntuacion; 
	    	break;
        case _asignacion: return _asignacion; 
	 		break;	 
	}
	//*/
	
	if(eof()){/*printf("fin ");*/return _eof;}
	//printf("\n-fin error-\n ");
	return _error;
}

void scanner()
{	
	llenarDiccionario();
	//fseek(file,0,SEEK_SET);
	//printf("%c",fgetc(file));
	//*
	//cout<<reservadas[1];
	token a;
	do
	{
		//printf("otra \n");
		a=next();
		//if(a==_eof)printf("final\n");
		switch(a)
		{
			/*case _comentario:
				printf("comentario\n");
				cadena=" ";
				break;*/
			case _reservada:
				printf("reservada: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _id:
				printf("identificador: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _octal:
				printf("octal: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _hexa:
				printf("hexadecimal: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _real:
				printf("real: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _delimitacion:
				printf("delimitacion: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _aritmetico:
				printf("aritmetico: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _puntuacion:
				printf("puntuacion: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _asignacion:
				printf("asignacion: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _eof:
				printf("\neof\n");
				printf("Analisis finalizado. Lineas analizadas: %i \n",lineas);
				break;
			case _error:
				printf("\nerror\n");
				printf("Analisis interrumpido por error en la linea %i \n",lineas);
				break;		
		}
		
		
		//cout<<cadena<<endl;
		//cadena=" ";
		//system("pause");
	}while(a!=_eof && a!=_error);
	//*/
}
