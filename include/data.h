#ifndef DATA_SESSIONS_H
#define DATA_SESSIONS_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "process.h"
#include "session.h"

bool construct_next_session(FILE *f, session *s);
bool write_session(const char *filename, session *s);

bool write_session_const_bytelen(const char *filename, session *s);

#endif