#include"scanner.h"

#include<cstdio>
#include<cctype>
#include<cstring>
#include<iostream>
#include<map>

#define udef -1

using namespace std;

typedef std::map <int,string> Diccionario;

Diccionario reservadas;

int p=0,q=0;

int findearchivo=0;
bool cfin=false;

int lineas=1;

string cadena="";

FILE *file;

string getCadena()
{
	string cadenaR=cadena;
	cadena="";
	return cadenaR;
}

int getLineas()
{
	return lineas;
}

void llenarDiccionario()
{
	reservadas[0]="identity";
	reservadas[1]="transpose";
	reservadas[2]="throw";
}

void agregar(char a)
{
	cadena=cadena+a;
}

bool open(char* archivo)
{
	llenarDiccionario();
	
	file=fopen(archivo,"r");
	if(file)
	{
		fseek(file,0,SEEK_END);
		findearchivo=ftell(file);
		fseek(file,q,SEEK_SET);
		return true;
	}
	
	return false;	
}

void close()
{
	fclose(file);
}

int read()
{
	char r=fgetc(file);
	if(r==10) p++;
	
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
	int e=read();
	while(e==10 || e==32 || e==9)
	{
		if(e==10) 
		{
			lineas++;
			
			fallback();
			success();			
		}
		e=read();
	}
	fallback();
	success();
}

bool eof()
{
	if(ftell(file)>=findearchivo || cfin==true)
	{
		return true;
	}
	return false;
}

token identificador()
{
	int actual=0, prior=0;
	int c;
	
	while (actual>udef)
	{
		prior=actual;
		c=read();
		
		
		switch(actual)
		{
			case 0:
				if(c=='_') actual=1;
				else if(isalpha(c)) actual=2;                                   
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
		if(actual!=udef)agregar(c);
	}
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
		
		switch(actual)
		{
			case 0:
				if(c=='0')actual=1;
				else if(c>='1' && c<='9') actual=5;
				else if((isalpha(c) && (c!='x'||c!='X')) || c=='_') actual=11; 
				else actual=udef;
				break;
			case 1:
				if(c>='0' && c<='7') actual=1;
				else if(c=='x' || c=='X') actual=2;
				else if(c=='.') actual=6;
				else actual=udef;
				break;
			case 2:
				if(isdigit(c) || (c>='a' && c<='f') || (c>='A' && c<='F')) actual=3;
				else actual=udef;
				break;
			case 3:
				if(isdigit(c) || (c>='a' && c<='f') || (c>='A' && c<='F')) actual=4;
				else actual=udef;
				break;
			case 4:
				if(isdigit(c) || (c>='a' && c<='f') || (c>='A' && c<='F')) actual=3;
				else if((isalpha(c) && ((c>='a' && c<='f')||(c>='A' && c<='F'))) || c=='_') actual=11;
				else actual=udef;
				break;
			case 5:
				if(isdigit(c)) actual=5;
				else if(c=='.') actual=6;
				else if(c=='E' || c=='e') actual=8;
				else if((isalpha(c) && (c!='e'||c!='E')) || c=='_') actual=11;
				else actual=udef;
				break;
			case 6:
				if(isdigit(c)) actual=7;
				else actual=udef;
				break;
			case 7:
				if(isdigit(c)) actual=7;
				else if(c=='E' || c=='e') actual=8;
				else if((isalpha(c) && (c!='e'||c!='E')) || c=='_') actual=11;
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
				else if((isalpha(c) && (c!='e'||c!='E')) || c=='_') actual=11;
				else actual=udef;
				break;
			case 11:
				actual=udef;
				break;
		}
		if(actual!=udef)agregar(c);
	}
	
	if(prior==1)
	{
		fallback();
		success();
		return _octal;		
	}
	
	else if(prior==4)
	{
		fallback();
		success();
		return _hexa;		
	}
	
	else if(prior==5 || prior==7 || prior==10)
	{
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
		
		switch(actual)
		{
			case 0:
				if(c=='#') actual=1;
				else actual=udef;
				break;
			case 1:
				if(c!=10 && c!=-1) actual=1;
				else if(c==10) {actual=2;lineas++;}
				else actual=udef;
				break;
			case 2:
				actual=udef;
				break;
		}
		
		if(c==-1) cfin=true;
	}
	
	if(prior==2)
	{
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
			default: actual=udef;
			break;
			
		}
		if(actual!=udef)agregar(c);
	}
	
	if(prior==1 || prior==2 || prior==3 || prior==4)
	{
		fallback();
		success();
		if(prior==1) return _delParIzq;
		else if(prior==2) return _delParDer;	
		else if(prior==3) return _delCorIzq;	
		else if(prior==4) return _delCorDer;	
	}	
	else if(prior==5 || prior==6 || prior==7 || prior==8)
	{
		fallback();
		success();
		if(prior==5) return _ariSuma;
		else if(prior==6) return _ariResta;	
		else if(prior==7) return _ariMulti;	
		else if(prior==8) return _ariDivi;		
	}	
	else if(prior==9 || prior==10)
	{
		fallback();
		success();
		if(prior==9) return _puntComa;
		else if(prior==10) return _puntPuntoComa;
	}
	else if(prior==11)
	{
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
	wsp();
	
	switch(comentario())
	{
		case _comentario: return _comentario; 
			break;
	}
	
	switch(identificador())
	{
		case _reservada:  return _reservada;
			break;
		case _id: return _id; 
			break;
	}
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
		case _delParDer: return _delParDer; 
	 		break;
		case _delParIzq: return _delParIzq; 
	 		break;
		case _delCorDer: return _delCorDer; 
	 		break;
		case _delCorIzq: return _delCorIzq; 
	 		break;
	 		
		case _ariSuma: return _ariSuma; 
	        break;
		case _ariResta: return _ariResta; 
	        break;
		case _ariMulti: return _ariMulti; 
	        break;
		case _ariDivi: return _ariDivi; 
	        break;
	        
		case _puntComa: return _puntComa; 
	    	break;
		case _puntPuntoComa: return _puntPuntoComa; 
	    	break;
	    	
        case _asignacion: return _asignacion; 
	 		break;	 
	}
	
	if(eof())return _eof;
	
	return _error;
}

void scanner()
{	
	llenarDiccionario();
	
	token a;
	do
	{
		
		a=next();
		switch(a)
		{
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
				
			case _delParDer:
				printf("delimitacion parentesis derecho: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _delParIzq:
				printf("delimitacion parentesis izquierdo: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _delCorDer:
				printf("delimitacion corchete derecho: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _delCorIzq:
				printf("delimitacion corchete izquierdo: ");cout<<cadena<<endl;
				cadena="";
				break;
			
			case _ariSuma:
				printf("aritmetico suma: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _ariResta:
				printf("aritmetico resta: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _ariMulti:
				printf("aritmetico multiplicacion: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _ariDivi:
				printf("aritmetico divicion: ");cout<<cadena<<endl;
				cadena="";
				break;
				
			case _puntComa:
				printf("puntuacion coma: ");cout<<cadena<<endl;
				cadena="";
				break;
			case _puntPuntoComa:
				printf("puntuacion punto y coma: ");cout<<cadena<<endl;
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
		
	}while(a!=_eof && a!=_error);
	//*/
}
