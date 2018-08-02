 /**
 * @file fastsmooth.cpp
 * @author Anderson Felippe <anderson.felippe@br.bosch.com>
 * @date 31 Jul 2018
 * @brief Fastsmooth implementation
 *
 * This is a fastsmooth implementation C port from the one used in MATLAB
 * which was originally based on the theory by T. C. O'Haver, May, 2008.
  *
 **/

#include <stdio.h>
#include <string.h>
#include "fastsmooth.h"

// Private prototypes
void run_fastsmooth_algorithm(double *smooth_vec, double *input_vec,
		  		int smooth_width, int ends, size_t num_el);
double sum(double *sum_vec, int start_point, int end_point);
double mean(double *mean_vec, int start_point, int end_point);
int m_round(double num);

int8_t fastsmooth(double *smooth_vec, double *input_vec,
		  int width, int type, int ends, size_t n)
{
	if (width < 0 || type < 0 || ends < 0)
		return -1;

	switch (type) {

		case 1:
			run_fastsmooth_algorithm(smooth_vec, input_vec, width, ends, n);
			break;
		case 2:
			run_fastsmooth_algorithm(smooth_vec, input_vec, width, ends, n);
                        memcpy(input_vec, smooth_vec, sizeof(double) * n);
			run_fastsmooth_algorithm(smooth_vec, input_vec, width, ends, n);
			break;
		case 3:
			run_fastsmooth_algorithm(smooth_vec, input_vec, width, ends, n);
			int i;
			for (i = 0; i < 2; i++) {
				memcpy(input_vec, smooth_vec, sizeof(double) * n);
				run_fastsmooth_algorithm(smooth_vec, input_vec, width, ends, n);
			}
			break;
		default:
			printf("Wrong type. It must be between 1 and 3.\n");
			return -1;
			break;
	}

	return 0;
}

void run_fastsmooth_algorithm(double *smooth_vec, double *input_vec,
		  		int smooth_width, int ends, size_t num_el)
{
	uint32_t i;
	double s[num_el];
	int num_el_v = num_el - 1;
	double sum_points = 0;
	uint32_t width = m_round(smooth_width);
	uint32_t halfw = m_round(width / 2);
	uint32_t k = 0;
	uint32_t s_index;
	// Adjust indexes for odd and even numbers:
	if ((smooth_width % 2) == 0)
                s_index = halfw - 1;
        else
                s_index = halfw;
	// Sum up all vector points
	sum_points = sum(input_vec, 0, width);
	//Zero 's' vector
	memset(s, 0, num_el);

	for (k = 0; k < (num_el - width); k++) {
		s[k + s_index] = sum_points;
		sum_points -= input_vec[k];
		sum_points += input_vec[k + width];
	}

	s[k + s_index] = sum(input_vec, num_el_v - width + 1, num_el);

	// Calculate output smooth vector
	for (i = 0; i < (uint32_t)num_el; i++) {
		smooth_vec[i] = s[i] / (double)width;
	}
	// Taper the ends of the signal is ends == 1
	if (ends == 1) {
		double start_point = smooth_width / 2.0;
		smooth_vec[0] = (input_vec[0] / 2.0) + (input_vec[1] / 2.0);

		for (k = 1; k < start_point; k++) {
			smooth_vec[k] = mean(input_vec, 0, 2 * k + 1);
			smooth_vec[num_el_v - k] = mean(input_vec, (num_el_v - (2 * k)), num_el);
		}

		smooth_vec[num_el_v] = (input_vec[num_el_v] / 2.0) + (input_vec[num_el_v - 1] / 2.0);
	}
}

double sum(double *sum_vec, int start_point, int end_point)
{
	int i;
	double result = 0;

	for (i = start_point; i < end_point; i++)
		result += sum_vec[i];

	return result;
}

double mean(double *mean_vec, int start_point, int end_point)
{
	int i;
	double n = 0.0;
	double result = 0.0;

	for (i = start_point; i < end_point; i++) {
		result += mean_vec[i];
		++n;
	}

	return (result / n);
}

int m_round(double num)
{
        return num < 0 ? num - 0.5 : num + 0.5;
}
