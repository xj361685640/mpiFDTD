#ifndef _NO_MODEL_H
#define _NO_MODEL_H

#include "bool.h"
#include "myComplex.h"
#include <stdio.h>
#include <stdlib.h>
extern double ( *noModel_EPS(void))(double, double, int, int);
extern void (*noModel_output(void))(FILE *, double complex*);

extern bool noModel_isFinish(void);
#endif
