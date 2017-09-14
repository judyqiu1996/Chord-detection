/****************************************************************************
 * 
 *   Header file to declare an enumeration and a set of functions
 *   to determine machine byte-order (endianness) and to swap
 *   little-endian data into big-endian representation, and vice versa.
 *
 *   'SNDAN' analysis files are always big-endian for consistency across
 *   platforms, whereas 'WAVE' files are always little-endian.
 *
 *   Changes:                        
 *	10/31/97 zheng	Add byte_order() to decide machine byte order
 *      01/29/98 jjjm   Move these functions from 'header.c' to this file,
 *                      and separate the test for the machine's byte order
 *                      from the decision about whether or not to byte-reverse
 *                      the sound data (in order to support WAVE files).
 *****************************************************************************/


enum endianness { big_endian,	                        /* jjjm 01/29/98 */
		  little_endian };

void byteswap2(short *);				/* jwb 8/12/94 */
void byteswap4(int *);					/* jwb 8/12/94 */
int byte_order();		 /* zheng 11/29/97; rev. jjjm 01/29/98 */
