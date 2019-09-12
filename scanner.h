#ifndef scanner_h
#define scanner_h

#include<cstdio>

typedef enum {_id,_entero,_real,_eof,_err} token;

void open();
void close();

int read();
void fail();
void success();
void fallback();

void wsp();
bool eof();
bool identifier();
token num();
token next();

void scanner();

#endif


