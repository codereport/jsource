/* Copyright 1990-2007, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */

#pragma once

void jepath(char*, char*);
void jesetpath(char*);
void* jeload(void* callbacks);  // returns J
A jegeta(int, char*);
I jeseta(I n, char* name, I x, char* d);
int jedo(char const*);  // could be jedi
void jefree();
char* jegetlocale();
void jefail();
void* jega(I t, I n, I r, I* s);  // returns A
void* jehjdll();
int jefirst(int type, char* arg);
