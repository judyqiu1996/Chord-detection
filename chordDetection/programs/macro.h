/*******************************************************************************
				macro.h
	Useful macros for various applications
	changes:
	06/03/96  jwb	Add sq(x)
 ******************************************************************************/
#ifdef abs
#undef abs
#endif /* ifdef abs */
#define abs(x)  (((x) > 0) ? (x) : (-(x)))
#ifdef min
#undef min
#endif /* ifdef min */
#define min(x,y)  (((x) < (y)) ? (x) : (y))
#ifdef max
#undef max
#endif /* ifdef max */
#define max(x,y) (((x) > (y)) ? (x) : (y))
#ifdef amod
#undef amod
#endif /* ifdef amod */
#define amod(x,y) ((x) - ((int)((x)/(y)))*(y))
#
#define HUGEI 1.0e+38 
#
#define sq(x)  ((x)*(x))

