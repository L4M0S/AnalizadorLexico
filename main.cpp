#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>

#include"scanner.h" 

using namespace std;

char nombrearchivo[50];


int main()
{
	cout<<"nombre del archivo: ";
	cin>>nombrearchivo;
	cout<<endl;
	
	if(open(nombrearchivo))	scanner();
	else cout<<endl<<"el archivo no existe"<<endl;
	
	close();
	
	printf("\n");
	system("pause");
	return 0;
}

