#include <stdio.h>

#define TRUE	1
#define FALSE	0

/* marker types */
#define DOT	0
#define CROSS	1
#define CIRCLE	2

/* cursor type */
#define RUBBER	3

/* linestyles */
#define	SOLID	0
#define DOTTED	1
#define DOTDASH	2
#define SHORTDASH	3
#define LONGDASH	4

/* drawing modes */
#define	OR	0
#define CLEAR	1
#define XOR	2

/* cursor control keys */
#define NORTH	'w'
#define NORTH5	'W'
#define	NORTHEAST	'e'
#define	NORTHEAST5	'E'
#define	EAST	'd'
#define	EAST5	'D'
#define	SOUTHEAST	'c'
#define	SOUTHEAST5	'C'
#define	SOUTH	'x'
#define	SOUTH5	'X'
#define	SOUTHWEST	'z'
#define	SOUTHWEST5	'Z'
#define	WEST	'a'
#define	WEST5	'A'
#define	NORTHWEST	'q'
#define	NORTHWEST5	'Q'

/*************************/
/* error message numbers */
/*************************/


#define ERR_BADERR	0	/* bad error */

/* viewport/window errors */
#define VBASE		1
#define	ERR_VPTBAD	(VBASE + 0)	/* bad viewport */
#define	ERR_VPTFIT	(VBASE + 1)	/* viewport doesn't fit */
#define ERR_WINBAD	(VBASE + 2)	/* bad window */

/* display/initialization errors */
#define CBASE		4
#define ERR_BADNAME	(CBASE + 0)	/* bad display name */
#define ERR_NOINIT	(CBASE + 1)	/* initialization not done */
#define ERR_PREINIT	(CBASE + 2)	/* initialization done twice */
#define ERR_GNOINIT	(CBASE + 3)	/* g_raph not initialized */
#define ERR_GPREINIT	(CBASE + 4)	/* g_raph already initialized */

/* attribute errors */
#define ABASE		9
#define ERR_BADMARK	(ABASE + 0)	/* illegal marker */
#define ERR_BADSTYLE	(ABASE + 1)	/* illegal linestyle */
#define ERR_BADMODE	(ABASE + 2)	/* illegal drawing mode */
#define ERR_NOMARK	(ABASE + 3)	/* marker not available */
#define ERR_NOSTYLE	(ABASE + 4)	/* linestyle not available */
#define ERR_NOMODE	(ABASE + 5)	/* mode not available */

/* input errors */
#define IBASE		15
#define ERR_NOBUTTON	(IBASE + 0)	/* no locator-button available */
#define MAXERR		(IBASE + 0)	/* highest error number */


/*************************/
/* structure definitions */
/*************************/

struct g__screen {	/* screen data */
	int maxx;	/* maximum horizontal pixel address */
	int minx;	/* minimum horizontal pixel address */
	int maxy;	/* maximum vertical pixel address */
	int miny;	/* minimum vertical pixel address */
	float width;	/* screen width in centimeters */
	float height;	/* screen height in centimeters */
	};
struct g__window { float	/* limits of window */
		minx, 
		maxx,
		xlength,	/* maxx - minx */
		miny,
		maxy,
		ylength;	/* maxy - miny */
	};
struct g__viewport { float	/* limits of viewport */
		minx,
		maxx,
		xlength,	/* maxx - minx */
		miny,	
		maxy,
		ylength;	/* maxy - miny */
	};
struct g__ndc { float
	minx,
	maxx,
	miny,
	maxy;
	};
struct g__display {
	int (*button)();	/* graphics input with cursor */
	int (*clear)();		/* clear screen */
	int (*draw)();		/* draw a line */
	int (*init)();		/* initialize display */
	int (*inquire_size)();	/* report diplay sizes */
	int (*flush)();
	int (*linestyle)();	/* set linestyle */
	int (*thick)();		/* set linewidth */
	int (*gray)();		/* set grayscale. added by tjm 3/96*/
	int (*color)();		/* set color. added by tjm 4/96*/
	int (*newpoly)();	/* new postscriipt path tjm 4/96*/
	int (*fill)();		/* fill path tjm 4/96*/
	int (*addline)();	/* addline tjm 4/96*/
	int (*stroke)();	/* stroke the line tjm 4/96*/
	int (*marker)();	/* draw a marker */
	int (*mode)(); 		/* drawing mode */
	int (*move)();		/* move current position */
	int (*set_cursor)();	/* set cursor type */
	int (*terminate)();	/* terminate display */
	int (*text)();		/* graphics texts */
	int (*textsize)();	/* size of characters */
	int (*text_extent)();	/* size of string */
	int (*text_input)();	/* text input routine */
	int (*error_message)();	/* error message handler */
} ;
