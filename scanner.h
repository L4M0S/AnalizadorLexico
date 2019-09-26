#ifndef scanner_h
#define scanner_h

#include<cstdio>

typedef enum {_reservada,_id, _octal,_hexa,_real, _eof,_err} token;

void open(char*);
void close();

int read();
void fail();
void success();
void fallback();

void wsp();
bool eof();
token identifier();
token num();
token next();

void scanner();

#endif


