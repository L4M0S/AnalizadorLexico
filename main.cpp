#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>

#include"scanner.h" 

using namespace std;

char nombrearchivo[50];


int main()
{
	cout<<"nombre y extencion del archivo: ";
	cin>>nombrearchivo;
	cout<<endl;
	
	if(open(nombrearchivo))
	{
		//scanner();
		
		token a;
		
		do
		{
			a=next();
			switch(a)
			{
				case _reservada:
					printf("reservada: ");cout<<getCadena()<<endl;
					break;
				case _id:
					printf("identificador: ");cout<<getCadena()<<endl;
					break;
					
				case _octal:
					printf("octal: ");cout<<getCadena()<<endl;
					break;
				case _hexa:
					printf("hexadecimal: ");cout<<getCadena()<<endl;
					break;
				case _real:
					printf("real: ");cout<<getCadena()<<endl;
					break;
					
				case _delParDer:
					printf("delimitacion parentesis derecho: ");cout<<getCadena()<<endl;
					break;
				case _delParIzq:
					printf("delimitacion parentesis izquierdo: ");cout<<getCadena()<<endl;
					break;
				case _delCorDer:
					printf("delimitacion corchete derecho: ");cout<<getCadena()<<endl;
					break;
				case _delCorIzq:
					printf("delimitacion corchete izquierdo: ");cout<<getCadena()<<endl;
					break;
				
				case _ariSuma:
					printf("aritmetico suma: ");cout<<getCadena()<<endl;
					break;
				case _ariResta:
					printf("aritmetico resta: ");cout<<getCadena()<<endl;
					break;
				case _ariMulti:
					printf("aritmetico multiplicacion: ");cout<<getCadena()<<endl;
					break;
				case _ariDivi:
					printf("aritmetico divicion: ");cout<<getCadena()<<endl;
					break;
					
				case _puntComa:
					printf("puntuacion coma: ");cout<<getCadena()<<endl;
					break;
				case _puntPuntoComa:
					printf("puntuacion punto y coma: ");cout<<getCadena()<<endl;
					break;
					
				case _asignacion:
					printf("asignacion: ");cout<<getCadena()<<endl;
					break;
					
				case _eof:
					printf("\neof\n");
					printf("Analisis finalizado. Lineas analizadas: %i \n",getLineas());
					break;
				case _error:
					printf("\nerror\n");
					printf("Analisis interrumpido por error en la linea %i \n",getLineas());
					break;		
			}
			
		}while(a!=_eof && a!=_error);
		
	}
	else cout<<endl<<"el archivo no existe"<<endl;
	
	close();
	
	printf("\n");
	system("pause");
	return 0;
}

