 /**
 * @file fastsmooth.h
 * @author Anderson Felippe <anderson.felippe@br.bosch.com>
 * @date 31 Jul 2018
 * @brief Fastsmooth implementation
 *
 * This is a fastsmooth implementation C port from the one used in MATLAB
 * which was originally based on the theory by T. C. O'Haver, May, 2008.
  *
 **/

#ifndef FASTSMOOTH_H
#define FASTSMOOTH_H

#include <stdint.h>

int8_t fastsmooth(double *smooth_vec, double *input_vec, int width,
		  int type, int ends, size_t n);

#endif /*_FASTSMOOTH_H_*/
