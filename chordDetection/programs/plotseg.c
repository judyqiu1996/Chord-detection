/******************************************************************************
 *
 *		plotseg.c
 *
 *	contains functions plotseg()  plotseg1(), plotseg2(),
 *	plotseg3(), plotbar(), plotbar1()
 *
 *	includes call to plabel(x,y), which is an extra label function
 *	supplied by the user.
 *
 *	J. Beauchamp  1987
 *	11/9/88: install ref to axskip, allowing skip of axes plots
 *		 for monan specific plots
 *	7/3/89:  add plotseg2()
 *	7/25/89: fix string out in plotbar()
 *	10/11/89: change g_inquire_ndc to g_inquire_viewport,
 *		  fix time axis bug, and install plotseg3()
 * 	12/17/89: remove useless statement in plotbar() (before for)
 *	11/4/90: fix no. digits horiz. axis for large xinc
 *	11/4/90: extend range of if(!axskip) and make variables static
 *		 for plotseg() and plotseg1()
 *	11/6/90: insert research mode code (approve axis values)
 *		 including approve() routine
 *	2/28/91: replace HUGE with HUGEI for IEEE huge value given in
 *		 macro.h
 *	11/26/91: remove axskip, pause defs for non-monan applications.
 *		  remove research mode code and approve function.
 *	6/30/92:  append with plotbar1, a different version of plotbar.
 *	7/10/92:  install plabel in plotbar1()
 *	11/29/97	zheng	Use gets and sscanf for input
 *	10/09/98  jwb	plotseg2(): Install line thickness variable.
 *****************************************************************************/
#include <math.h>
#include <stdio.h>
#include "g_raph.h"
#include "macro.h"
#define NHTICKS 11
#define NVTICKS 11
#define A .15    /* left X border  */
#define B .05    /* right X border */
#define C .25    /* bottom Y border */
#define D .05    /* top Y border   */
#define P fprintf

double textwidth,textheight;

plotseg(x,y,npts,xlabel,ylabel) float x[],y[]; int npts; char *xlabel,*ylabel;
{
/*  vertical and horizontal scales determined by program  */
    static double xmin,xmax,xinc,ymin,ymax,yinc;
    static double xwmin,xwmax,ywmin,ywmax,omag;
    static int nhdigits,nvdigits,nvticks,nhticks;
    int i,j;
    char resp[20];

    /*	 find the min and max values of x and y  for the plot ranges  */
    xmin = HUGEI; xmax = -HUGEI;  ymin = HUGEI; ymax = -HUGEI;
    for(i=0;i<npts;i++)
    {
	xmin = min(xmin,(double)x[i]);
	xmax = max(xmax,(double)x[i]);
	ymin = min(ymin,(double)y[i]);
	ymax = max(ymax,(double)y[i]);
    }
    if(ymin == ymax)
    { while(1)
      {
        P(stderr,"give vertical range (ymin ymax): ");
        gets(resp);					/* zheng 11/29/97 */
	if (sscanf(resp,"%f %f",&ymin,&ymax)==2) break;
      }
    }


    /* compute horizontal and vertical axis parameters */

    xinc = (xmax - xmin)/(NHTICKS -1);
    nhdigits = max(.845 - log10(xinc),0.); /* set for .7 threshold  */
    if(nhdigits) xinc = 
       pow(10.,(double)-nhdigits)*floor(xinc*pow(10.,(double)nhdigits)+.4999);
    else
    { omag = floor(log10(xinc));
      xinc = .5*floor(2.*xinc*pow(10.,-omag)+.4999);
      if((xinc - floor(xinc) > .25)&&(xinc > 2.5)) xinc = ceil(xinc);
      xinc *= pow(10.,omag);
      if(xinc - floor(xinc) > .25) nhdigits = 1;  /* handles X.5 case */
    }
    xmin = floor(xmin/xinc)*xinc;
    xmax = ceil(xmax/xinc)*xinc;
    nhticks = 1 + (xmax - xmin)/xinc + .5;
    if((nhdigits == 0)&&((xinc - floor(xinc)) > .01)) nhdigits = 1;

    yinc = (ymax - ymin)/(NVTICKS -1);
    nvdigits = max(.99 - log10(yinc),0.);
    if(nvdigits) yinc =
       pow(10.,(double)-nvdigits)*floor(yinc*pow(10.,(double)nvdigits)+.4999);
    else
    { omag = floor(log10(yinc));
      yinc = .5*floor(2.*yinc*pow(10.,-omag)+.4999);
      if((yinc - floor(yinc) > .25)&&(yinc > 2.5)) yinc = ceil(yinc);
      yinc *= pow(10.,omag);
      if(yinc - floor(yinc) > .25) nvdigits = 1;  /* handles Y.5 case */
    }
/*   P(stderr,"before floor/ceil: ymin=%f, ymax=%f, yinc=%f\n",ymin,ymax,yinc); */
    ymin = floor(ymin/yinc)*yinc;
    ymax = ceil(ymax/yinc)*yinc;
    nvticks = 1 + (ymax - ymin)/yinc +.5;
/*    P(stderr,"after floor/ceil: ymin=%f, ymax=%f, nvticks=%d\n",ymin,ymax,nvticks);
    getchar();  */

    /*	set window within current viewport  */

    xwmin = (1.+A)*xmin - A*xmax;  xwmax = (1.+B)*xmax - B*xmin;
    ywmin = (1.+C)*ymin - C*ymax;  ywmax = (1.+D)*ymax - D*ymin;

    g_init();
/*
    P(stderr,"xwmin=%f,xwmax=%f,ywmin=%f,ywmax=%f\n",xwmin,xwmax,ywmin,ywmax);
    fflush(stderr);
*/
    g_set_window(xwmin,xwmax,ywmin,ywmax);
/*
    P(stderr,"xmin=%f,ymin=%f,xmax=%f,xlabel=%s,xinc=%f,nhticks=%d,nhdigits=%d\n", xmin,ymin,xmax,xlabel,xinc,nhticks,nhdigits); fflush(stderr);
*/
    g_axis(xmin,ymin,xmax-xmin,xlabel, 1,2,0, xmin, xinc, nhticks, nhdigits);
    g_axis(xmin,ymin,ymax-ymin,ylabel, 1,2,90,ymin, yinc, nvticks, nvdigits);
    g_text_extent(" ",&textwidth,&textheight);
    plabel(xmin, ymin -4.*textheight);

   /*  draw graph  */

    g_move_abs((double)x[0],(double)y[0]);
    for(i=1;i<npts;i++)  g_draw_abs((double)x[i],(double)y[i]);

    g_term();  

}

plotseg1(x,y,npts,xlabel,ylabel,x1,x2,y1,y2,nhticks,nvticks) 
	float x[],y[]; int npts; char *xlabel,*ylabel;
    	float x1,x2,y1,y2; int nhticks,nvticks;
/*   vertical and horizontal scales predetermined.  call this when you
     know what you want. */
{
    static double xmin,xmax,ymin,ymax,xwmin,xwmax,ywmin,ywmax,xinc,yinc,ym;
    static double xvmin,xvmax,yvmin,yvmax;
    static double xvmin_new,xvmax_new,yvmin_new,yvmax_new;
    static int nhdigits,nvdigits;
    int i,j;

     xmin=x1; xmax=x2; ymin=y1; ymax=y2;
    /* compute horizontal and vertical axis parameters */

    xinc = (xmax - xmin)/(nhticks -1);
    nhdigits = max(.99 - log10(xinc),0.);
    if((nhdigits == 0)&&((xinc - floor(xinc)) > .01)) nhdigits = 1;

    yinc = (ymax - ymin)/(nvticks -1);
    nvdigits = max(.99 - log10(yinc),0.);

    /*	set mapping of our coords to viewport using g_set_window  */
    xwmin = (1.+A)*xmin - A*xmax;  xwmax = (1.+B)*xmax - B*xmin;
    ywmin = (1.+C)*ymin - C*ymax;  ywmax = (1.+D)*ymax - D*ymin;
    g_init();
    g_set_window(xwmin,xwmax,ywmin,ywmax);

    g_axis(xmin,ymin,xmax-xmin,xlabel, 1,2,0, xmin, xinc, nhticks, nhdigits);
    g_axis(xmin,ymin,ymax-ymin,ylabel, 1,2,90,ymin, yinc, nvticks, nvdigits);

    g_text_extent(" ",&textwidth,&textheight);
    plabel(xmin, ymin -4.*textheight);

   /*  reset viewport and window and draw graph  */
    g_inquire_viewport(&xvmin,&xvmax,&yvmin,&yvmax);
    xvmin_new = xvmin + (A/(1.+A+B))*(xvmax-xvmin);
    xvmax_new = xvmax - (B/(1.+A+B))*(xvmax-xvmin);
    yvmin_new = yvmin + (C/(1.+C+D))*(yvmax-yvmin);
    yvmax_new = yvmax - (D/(1.+C+D))*(yvmax-yvmin);
    g_set_viewport(xvmin_new,xvmax_new,yvmin_new,yvmax_new);
    g_set_window(xmin,xmax,ymin,ymax);

    g_move_abs((double)x[0],(double)y[0]);
    for(i=1;i<npts;i++)  g_draw_abs((double)x[i],(double)y[i]);

    g_term();
}

plotseg2(x,y,npts,xlabel,ylabel,y1,y2,nvticks,linethick)    /* jwb 10/09/98 */
	float x[],y[]; int npts; char *xlabel,*ylabel;
    	float y1,y2; int nvticks,linethick;		    /* jwb 10/09/98 */
/*  same as plotseg() except vertical scale is predetermined   */
{
    static double xmin,xmax,ymin,ymax,xinc,yinc,omag;
    static double xwmin,xwmax,ywmin,ywmax, xvmin,xvmax,yvmin,yvmax;
    static double xvmin_new,xvmax_new,yvmin_new,yvmax_new;
    static int nhdigits,nvdigits,nhticks;
    int i,j;
    char resp[20];

     ymin=y1; ymax=y2;
    /*	 find the min and max values of x for the plot range  */
    xmin = HUGEI; xmax = -HUGEI;
    for(i=0;i<npts;i++)
    {
	xmin = min(xmin,(double)x[i]);
	xmax = max(xmax,(double)x[i]);
    }
    if(ymin == ymax)
    { while(1)
      {
	P(stderr,"give vertical range (ymin ymax): ");
	gets(resp);					/* zheng 11/29/97 */
        if (sscanf(resp,"%f %f",&ymin,&ymax)==2) break;
      }
    }

    /* compute horizontal axis parameters */
    xinc = (xmax - xmin)/(NHTICKS -1);
    nhdigits = max(.845 - log10(xinc),0.); /* set for .7 threshold  */
    if(nhdigits) xinc = 
       pow(10.,(double)-nhdigits)*floor(xinc*pow(10.,(double)nhdigits)+.4999);
    else
    { omag = floor(log10(xinc));
      xinc = .5*floor(2.*xinc*pow(10.,-omag)+.4999);
      if((xinc - floor(xinc) > .25)&&(xinc > 2.5)) xinc = ceil(xinc);
      xinc *= pow(10.,omag);
      if(xinc - floor(xinc) > .25) nhdigits = 1;  /* handles X.5 case */
    }
    xmin = floor(xmin/xinc)*xinc;
    xmax = ceil(xmax/xinc)*xinc;
    nhticks = 1 + (xmax - xmin)/xinc + .5;
    if((nhdigits == 0)&&((xinc - floor(xinc)) > .01)) nhdigits = 1;

    /* compute vertical axis parameters */
    yinc = (ymax - ymin)/(nvticks -1);
    nvdigits = max(.99 - log10(yinc),0.);

    /*	set mapping of our coords to viewport using g_set_window  */
    xwmin = (1.+A)*xmin - A*xmax;  xwmax = (1.+B)*xmax - B*xmin;
    ywmin = (1.+C)*ymin - C*ymax;  ywmax = (1.+D)*ymax - D*ymin;
    g_init();
    g_set_line_thickness(linethick);			    /* jwb 10/09/98 */
    g_set_window(xwmin,xwmax,ywmin,ywmax);

    g_axis(xmin,ymin,xmax-xmin,xlabel, 1,2,0, xmin, xinc, nhticks, nhdigits);
    g_axis(xmin,ymin,ymax-ymin,ylabel, 1,2,90,ymin, yinc, nvticks, nvdigits);

    g_text_extent(" ",&textwidth,&textheight);
    plabel(xmin, ymin -4.*textheight);

   /*  reset viewport and window and draw graph  */
    g_inquire_viewport(&xvmin,&xvmax,&yvmin,&yvmax);
    xvmin_new = xvmin + (A/(1.+A+B))*(xvmax-xvmin);
    xvmax_new = xvmax - (B/(1.+A+B))*(xvmax-xvmin);
    yvmin_new = yvmin + (C/(1.+C+D))*(yvmax-yvmin);
    yvmax_new = yvmax - (D/(1.+C+D))*(yvmax-yvmin);
    g_set_viewport(xvmin_new,xvmax_new,yvmin_new,yvmax_new);
    g_set_window(xmin,xmax,ymin,ymax);

    g_move_abs((double)x[0],(double)y[0]);
    for(i=1;i<npts;i++)  g_draw_abs((double)x[i],(double)y[i]);

    g_term();

}

plotseg3(x,y,npts,xlabel,ylabel,x1,x2,nhticks) 
	float x[],y[]; int npts; char *xlabel,*ylabel;
    	float x1,x2; int nhticks;
/*  same as plotseg() except horizontal scale is predetermined   */
{
    static double xmin,xmax,ymin,ymax,xinc,yinc,omag;
    static double xwmin,xwmax,ywmin,ywmax, xvmin,xvmax,yvmin,yvmax;
    static double xvmin_new,xvmax_new,yvmin_new,yvmax_new;
    static int nhdigits,nvdigits,nvticks;
    int i,j;

     xmin = x1; xmax = x2;
    /*	 find the min and max values of y for the plot range  */
    ymin = HUGEI; ymax = -HUGEI;
    for(i=0;i<npts;i++)
    {
	ymin = min(ymin,(double)y[i]);
	ymax = max(ymax,(double)y[i]);
    }

    /* compute horizontal axis parameters */
    xinc = (xmax - xmin)/(nhticks -1);
    nhdigits = max(.845 - log10(xinc),0.);
    omag = pow(10.,(double)nhdigits);
    if((nhdigits >= 0)&&((xinc*omag - floor(xinc*omag)) > .01)) nhdigits++;

    /* compute vertical axis parameters */
    yinc = (ymax - ymin)/(NVTICKS -1);
    nvdigits = max(.845 - log10(yinc),0.); /* set for .7 threshold  */
    if(nvdigits) yinc = 
       pow(10.,(double)-nvdigits)*floor(yinc*pow(10.,(double)nvdigits)+.4999);
    else
    { omag = floor(log10(yinc));
      yinc = .5*floor(2.*yinc*pow(10.,-omag)+.4999);
      if((yinc - floor(yinc) > .25)&&(yinc > 2.5)) yinc = ceil(yinc);
      yinc *= pow(10.,omag);
      if(yinc - floor(yinc) > .25) nvdigits = 1;  /* handles X.5 case */
    }
    ymin = floor(ymin/yinc)*yinc;
    ymax = ceil(ymax/yinc)*yinc;
    nvticks = 1 + (ymax - ymin)/yinc + .5;

    /*	set mapping of our coords to viewport using g_set_window  */
    xwmin = (1.+A)*xmin - A*xmax;  xwmax = (1.+B)*xmax - B*xmin;
    ywmin = (1.+C)*ymin - C*ymax;  ywmax = (1.+D)*ymax - D*ymin;
    g_init();
    g_set_window(xwmin,xwmax,ywmin,ywmax);

    g_axis(xmin,ymin,xmax-xmin,xlabel, 1,2,0, xmin, xinc, nhticks, nhdigits);
    g_axis(xmin,ymin,ymax-ymin,ylabel, 1,2,90,ymin, yinc, nvticks, nvdigits);

    g_text_extent(" ",&textwidth,&textheight);
    plabel(xmin, ymin -4.*textheight);

   /*  reset viewport and window and draw graph  */
    g_inquire_viewport(&xvmin,&xvmax,&yvmin,&yvmax);
    xvmin_new = xvmin + (A/(1.+A+B))*(xvmax-xvmin);
    xvmax_new = xvmax - (B/(1.+A+B))*(xvmax-xvmin);
    yvmin_new = yvmin + (C/(1.+C+D))*(yvmax-yvmin);
    yvmax_new = yvmax - (D/(1.+C+D))*(yvmax-yvmin);
    g_set_viewport(xvmin_new,xvmax_new,yvmin_new,yvmax_new);
    g_set_window(xmin,xmax,ymin,ymax);

    g_move_abs((double)x[0],(double)y[0]);
    for(i=1;i<npts;i++)  g_draw_abs((double)x[i],(double)y[i]);

    g_term();

}
#define NVTICKS 11
plotbar(n1,n2,y,npts,xlabel,ylabel,string) 
float y[]; int n1, n2, npts; char *xlabel,*ylabel, *string;
{
    double xmin,xmax,ymin,ymax;
    double xwmin,xwmax,ywmin,ywmax;
    double xinc,yinc,dx,ym,vinc,hinc,omag;
    int i,j,nhdigits,nvdigits,nhticks,nvticks;

    /*	 find the min and max values of x and y  for the plot ranges  */
    xmin = n1; xmax = n2;   ymin = ymax = 0.;
    for(i=0;i<npts;i++)
    {
	ymin = min(ymin,(double)y[i]);
	if((double)y[i] > ymax) ymax = y[i]; 
    }
    ym = ymax;

    /* compute horizontal and vertical axis parameters */

    if(npts <= 12) nhticks = npts;
    else if (npts <= 36) nhticks = 1 + 2*(npts/4);
    else nhticks = 11;
    xinc = floor((xmax - xmin)/(nhticks-1) + .5);
    xmax = xmin + (nhticks - 1)*xinc;
    nhdigits = 0;

    yinc = (ymax - ymin)/(NVTICKS -1);
    nvdigits = max(.99 - log10(yinc),0.);
    if(nvdigits) yinc =
       pow(10.,(double)-nvdigits)*floor(yinc*pow(10.,(double)nvdigits)+.4999);
    else
    { omag = floor(log10(yinc));
      yinc = .5*floor(2.*yinc*pow(10.,-omag)+.4999);
      if((yinc - floor(yinc) > .25)&&(yinc > 2.5)) yinc = ceil(yinc);
      yinc *= pow(10.,omag);
      if(yinc - floor(yinc) > .25) nvdigits = 1;  /* handles Y.5 case */
    }
/*   P(stderr,"before floor/ceil: ymin=%f, ymax=%f, yinc=%f\n",ymin,ymax,yinc); */
    ymin = floor(ymin/yinc)*yinc;
    ymax = ceil(ymax/yinc)*yinc;
    nvticks = 1 + (ymax - ymin)/yinc +.5;
    /*	set window within current viewport  */

    xwmin = (1.+A)*xmin - A*xmax;  xwmax = (1.+B)*xmax - B*xmin;
    ywmin = (1.+C)*ymin - C*ymax;  ywmax = (1.+D)*ymax - D*ymin;
/*  printf("nvticks=%d,y[0]=%f,yinc=%f\n",nvticks,y[0],yinc);
    printf("xmin=%f,xmax=%f,ymin=%f,ymax=%f\n",xmin,xmax,ymin,ymax);
   printf("xwmin=%f,xwmax=%f,ywmin=%f,ywmax=%f=\n",xwmin,xwmax,ywmin,ywmax);
    getchar();
*/
    g_init();
    g_set_window(xwmin,xwmax,ywmin,ywmax);

    g_axis(xmin,ymin,xmax-xmin,xlabel, 1,2,0, xmin, xinc, nhticks, nhdigits);

    g_axis(xmin,ymin,ymax-ymin,ylabel, 1,2,90,ymin, yinc, nvticks, nvdigits);

    plabel(.6*xmin+.4*xwmin, (1.+C-D)*ymin - (C-D)*ymax);  
    g_text_extent(" ",&hinc,&vinc); 
    g_move_rel(-(strlen(string)+ 10)*hinc,-vinc); g_text(string);

   /*  draw graph  */

    for(i=1;i<npts;i++)
    {
       g_move_abs((double)(xmin + i),0.);
       g_draw_rel(0.,(double)y[i]);
    }
    g_term();
}

/******************************************************************************
 *	subroutine function to plot bar graph of y vs. x data                 *
 *									      *
 *	Assumptions:  x data is positive. 				      *
 *		      y data non-negative				      *
 *		      x and y each have npts points			      *
 *	Programmer: J. Beauchamp, 1989					      *
*******************************************************************************/
plotbar1(x,y,npts,xlabel,ylabel) float x[],y[]; int npts; char *xlabel,*ylabel;
{
    double xmin,xmax,ymin=0.,ymax;
    double xwmin,xwmax,ywmin,ywmax;
    double xinc,yinc,omag;
    int i,j,nhdigits,nvdigits,nvticks,nhticks;
    char scanbuf[80];

  /* find the max values of x and y  for the plot ranges (assuming pos vals) */
    xmax = ymax = 0.;
    for(i=0;i<npts;i++)
    {
	xmax = max(xmax,(double)x[i]);
	ymax = max(ymax,(double)y[i]);
    }
/*  P("after initial FOR loop xmax=%f, ymax=%f\n",xmax,ymax); */

    /* compute horizontal and vertical axis parameters */

    xinc = xmax/(NHTICKS -1);
    nhdigits = max(.845 - log10(xinc),0.); /* set for .7 threshold  */
    if(nhdigits) xinc = 
       pow(10.,(double)-nhdigits)*floor(xinc*pow(10.,(double)nhdigits)+.4999);
    else
    { omag = floor(log10(xinc));
      xinc = xinc*pow(10.,-omag);
      if(xinc <= 2.) xinc = 2.;
      else if(xinc <= 2.5) xinc = 2.5;
      else if(xinc <= 5.) xinc = 5.;
      else xinc = 10.;
      xinc = xinc*pow(10.,omag);
    }
    xmax = ceil(xmax/xinc)*xinc;
    nhticks = 1 + xmax/xinc + .5;
    xmax = xinc*(nhticks -1);
    xmin = 0.;

    yinc = (ymax - ymin)/(NVTICKS -1);
    nvdigits = max(.99 - log10(yinc),0.);
/*  P("initial yinc=%f, nvdigits=%d\n",yinc,nvdigits);  */
    if(nvdigits) yinc =
       pow(10.,(double)-nvdigits)*floor(yinc*pow(10.,(double)nvdigits)+.4999);
    else
    { omag = floor(log10(yinc));
      yinc = .5*floor(2.*yinc*pow(10.,-omag)+.4999);
      if((yinc - floor(yinc) > .25)&&(yinc > 2.5)) yinc = ceil(yinc);
      yinc *= pow(10.,omag);
      if(yinc - floor(yinc) > .25) nvdigits = 1;  /* handles Y.5 case */
    }
/*   P("before floor/ceil: ymin=%f, ymax=%f, yinc=%f\n",ymin,ymax,yinc);  */ 
    ymax = ceil(ymax/yinc)*yinc;
    nvticks = 1 + (ymax - ymin)/yinc +.5;
/* P("after floor/ceil: ymin=%f, ymax=%f, nvticks=%d\n",ymin,ymax,nvticks); */

    /*	set window within current viewport  */

    xwmin = (1.+A)*xmin - A*xmax;  xwmax = (1.+B)*xmax - B*xmin;
    ywmin = (1.+C)*ymin - C*ymax;  ywmax = (1.+D)*ymax - D*ymin;

/*  P("after window calculation:\n");
    P("nvticks=%d,y[0]=%f,yinc=%f\n",nvticks,y[0],yinc);
    P("xmin=%f,xmax=%f,ymin=%f,ymax=%f\n",xmin,xmax,ymin,ymax);
    P("xwmin=%f,xwmax=%f,ywmin=%f,ywmax=%f=\n",xwmin,xwmax,ywmin,ywmax);
    getchar();
 */ 

    g_init();
    g_set_window(xwmin,xwmax,ywmin,ywmax);

    g_axis(xmin,ymin,xmax-xmin,xlabel, 1,2,0, xmin, xinc, nhticks, nhdigits);

    g_axis(xmin,ymin,ymax-ymin,ylabel, 1,2,90,ymin, yinc, nvticks, nvdigits);

    g_text_extent(" ",&textwidth,&textheight);
    plabel(xmin, ymin -4.*textheight);

   /*  draw graph  */

    for(i=0;i<npts;i++)
    {
       g_move_abs((double)x[i],0.);
       g_draw_rel(0.,(double)y[i]);
    }

    g_term();

}
