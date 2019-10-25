#ifndef scanner_h
#define scanner_h

#include<cstdio>
#include<string>

using namespace std;

typedef enum {_reservada,_id, 
			_octal,_hexa,_real,
			_comentario,
			_delParDer,_delParIzq,_delCorDer,_delCorIzq,
			_ariSuma,_ariResta,_ariMulti,_ariDivi,
			_puntComa,_puntPuntoComa,
			_asignacion, 
			_eof,_error} token;



bool open(char*);
void close();

int read();
void fail();
void success();
void fallback();

void wsp();
bool eof();
token identificador();
token numero();
token next();
token comentario();
token especiales();

void scanner();

void agregar(char);

string getCadena();
int getLineas();

#endif


