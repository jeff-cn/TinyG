/*
 * util.c - a random assortment of useful functions
 * Part of TinyG project
 *
 * Copyright (c) 2010 - 2013 Alden S. Hart Jr.
 *
 * This file ("the software") is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2 as published by the
 * Free Software Foundation. You should have received a copy of the GNU General Public
 * License, version 2 along with the software.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, you may use this file as part of a software library without
 * restriction. Specifically, if other files instantiate templates or use macros or
 * inline functions from this file, or you compile this file and link it with  other
 * files to produce an executable, this file does not by itself cause the resulting
 * executable to be covered by the GNU General Public License. This exception does not
 * however invalidate any other reasons why the executable file might be covered by the
 * GNU General Public License.
 *
 * THE SOFTWARE IS DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT WITHOUT ANY
 * WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/* util.c/.h contains a dog's breakfast of supporting functions that are 
 * not specific to tinyg: including:
 *
 *	  - math and min/max utilities and extensions 
 *	  - vector manipulation utilities
 *	  - support for debugging routines
 */ 

#include "tinyg.h"
#include "util.h"

#ifdef __cplusplus
extern "C"{
#endif

/**** Vector utilities ****
 * copy_vector()			- copy vector of arbitrary length
 * copy_axis_vector()		- copy an axis vector
 * get_axis_vector_length()	- return the length of an axis vector
 * set_vector()				- load values into vector form
 * set_vector_by_axis()		- load a single value into a zero vector
 */

float vector[AXES];	// statically allocated global for vector utilities

/*
void copy_vector(float dst[], const float src[], uint8_t length) 
{
	for (uint8_t i=0; i<length; i++) {
		dst[i] = src[i];
	}
}
*/

void copy_axis_vector(float dst[], const float src[]) 
{
	memcpy(dst, src, sizeof(float)*AXES);
}

uint8_t vector_equal(const float a[], const float b[]) 
{
	if ((fp_EQ(a[AXIS_X], b[AXIS_X])) &&
	 	(fp_EQ(a[AXIS_Y], b[AXIS_Y])) &&
	 	(fp_EQ(a[AXIS_Z], b[AXIS_Z])) &&
	 	(fp_EQ(a[AXIS_A], b[AXIS_A])) &&
	 	(fp_EQ(a[AXIS_B], b[AXIS_B])) &&
	 	(fp_EQ(a[AXIS_C], b[AXIS_C]))) {
		return (true);
	}
	return (false);
}

float get_axis_vector_length(const float a[], const float b[]) 
{
	return (sqrt(square(a[AXIS_X] - b[AXIS_X]) +
				 square(a[AXIS_Y] - b[AXIS_Y]) +
				 square(a[AXIS_Z] - b[AXIS_Z]) +
				 square(a[AXIS_A] - b[AXIS_A]) +
				 square(a[AXIS_B] - b[AXIS_B]) +
				 square(a[AXIS_C] - b[AXIS_C])));
}

float *set_vector(float x, float y, float z, float a, float b, float c)
{
	vector[AXIS_X] = x;
	vector[AXIS_Y] = y;
	vector[AXIS_Z] = z;
	vector[AXIS_A] = a;
	vector[AXIS_B] = b;
	vector[AXIS_C] = c;
	return (vector);
}

float *set_vector_by_axis(float value, uint8_t axis)
{
	clear_vector(vector);
	switch (axis) {
		case (AXIS_X): vector[AXIS_X] = value; break;
		case (AXIS_Y): vector[AXIS_Y] = value; break;
		case (AXIS_Z): vector[AXIS_Z] = value; break;
		case (AXIS_A): vector[AXIS_A] = value; break;
		case (AXIS_B): vector[AXIS_B] = value; break;
		case (AXIS_C): vector[AXIS_C] = value;
	}
	return (vector);
}

/**** Math and other general purpose functions ****/

/* Slightly faster (*) multi-value min and max functions
 * 	min3() - return minimum of 3 numbers
 * 	min4() - return minimum of 4 numbers
 * 	max3() - return maximum of 3 numbers
 * 	max4() - return maximum of 4 numbers
 *
 * Implementation tip: Order the min and max values from most to least likely in the calling args
 *
 * (*) Macro min4 is about 20uSec, inline function version is closer to 10 uSec (Xmega 32 MHz)
 * 	#define min3(a,b,c) (min(min(a,b),c))
 *	#define min4(a,b,c,d) (min(min(a,b),min(c,d)))
 *	#define max3(a,b,c) (max(max(a,b),c))
 *	#define max4(a,b,c,d) (max(max(a,b),max(c,d)))
 */

inline float min3(float x1, float x2, float x3)
{
	float min = x1;
	if (x2 < min) { min = x2;} 
	if (x3 < min) { return (x3);} 
	return (min);
}

inline float min4(float x1, float x2, float x3, float x4)
{
	float min = x1;
	if (x2 < min) { min = x2;} 
	if (x3 < min) { min = x3;} 
	if (x4 < min) { return (x4);}
	return (min);
}

inline float max3(float x1, float x2, float x3)
{
	float max = x1;
	if (x2 > max) { max = x2;} 
	if (x3 > max) { return (x3);} 
	return (max);
}

inline float max4(float x1, float x2, float x3, float x4)
{
	float max = x1;
	if (x2 > max) { max = x2;} 
	if (x3 > max) { max = x3;} 
	if (x4 > max) { return (x4);}
	return (max);
}

/**** String utilities ****
 * strcpy_U() 	   - strcpy workalike to get around initial NUL for blank string - possibly wrong
 * isnumber() 	   - isdigit that also accepts plus, minus, and decimal point
 * escape_string() - add escapes to a string - currently for quotes only
 * read_float()    - read a float from a normalized char array
 */
/*
#ifdef __ARM
uint8_t * strcpy_U( uint8_t * dst, const uint8_t * src )
{
	uint16_t index = 0;
	do {
		dst[index] = src[index];	
	} while (src[index++] != 0);
	return dst;
}
#endif
*/

uint8_t isnumber(char c)
{
	if (c == '.') { return (true); }
	if (c == '-') { return (true); }
	if (c == '+') { return (true); }
	return (isdigit(c));
}

char_t *escape_string(char_t *dst, char_t *src) 
{
	char_t c;
	char_t *start_dst = dst;

	while ((c = *(src++)) != 0) {	// NUL
		if (c == '"') { *(dst++) = '\\'; }
		*(dst++) = c;
	}
	return (start_dst);
}


/* 
 * compute_checksum() - calculate the checksum for a string
 * 
 *	Stops calculation on null termination or length value if non-zero.
 *
 * 	This is based on the the Java hashCode function. 
 *	See http://en.wikipedia.org/wiki/Java_hashCode()
 */
#define HASHMASK 9999

uint16_t compute_checksum(char const *string, const uint16_t length) 
{
	uint32_t h = 0;
	uint16_t len = strlen(string);

	if (length != 0) {
		len = min(len, length);
	}
    for (uint16_t i=0; i<len; i++) {
		h = 31 * h + string[i];
    }
    return (h % HASHMASK);
}

/*
 * SysTickTimer_getValue() - this is a hack to get around some compatibility problems
 */
#ifdef __ARM
uint32_t SysTickTimer_getValue()
{
	return (SysTickTimer.getValue());
}
#endif

#ifdef __cplusplus
}
#endif
