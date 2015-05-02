#ifndef __Algorithm_filter_H
#define	__Algorithm_filter_H

#include "stm32f10x.h"

double IIR_I_Filter(double InData, double *x, double *y, double *b, short nb, double *a, short na);

#endif /* __Algorithm_filter_H */
