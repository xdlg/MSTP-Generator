/****************************************************************************
 *
 * Copyright(c) 2012-2012, John Forkosh Associates, Inc. All rights reserved.
 *           http://www.forkosh.com   mailto: john@forkosh.com
 * --------------------------------------------------------------------------
 * This file is part of gifsave89, which is free software.
 * You may redistribute and/or modify gifsave89 under the terms of the
 * GNU General Public License, version 3 or later, as published by the
 * Free Software Foundation.
 *      gifsave89 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, not even the implied warranty of MERCHANTABILITY.
 * See the GNU General Public License for specific details.
 *      By using gifsave89, you warrant that you have read, understood
 * and agreed to these terms and conditions, and that you possess the legal
 * right and ability to enter into this agreement and to use gifsave89
 * in accordance with it.
 *      Your gifsave89.zip distribution file should contain the file
 * COPYING, an ascii text copy of the GNU General Public License,
 * version 3. If not, point your browser to  http://www.gnu.org/licenses/
 * or write to the Free Software Foundation, Inc., 59 Temple Place,
 * Suite 330,  Boston, MA 02111-1307 USA.
 * --------------------------------------------------------------------------
 *
 * Purpose:   o	gifgraph generates a loop of calls to the bc calculator
 *		of the form,
 *		  "for ( trange ) { for ( xrange ) { expression } }",
 *              or just "for ( xrange ) { expression }" if trange==NULL,
 *		and calls putgif() for each sequence of xrange expression
 *		values, comprising the frames for an animated (or just
 *		one frame, when trange==NULL, for an static) gif graph.
 *		See the "Notes:" below, and also
 *		  http://www.forkosh.com/gifsave89.html
 *		for further details about gifgraph.
 *
 * Source:    o	gifgraph.c
 *
 * Functions: o	The following "table of contents" lists each function
 *		comprising gifgraph in the order it appears in this file.
 *		See individual function entry points for specific comments
 *		about purpose, calling sequence, side effects, etc.
 *		=============================================================
 *		+---
 *		| user-callable entry points
 *		+------------------------------------------------------------
 *		int nframes = gifgraph(gs,expression,xrange,trange)
 *		+---
 *		| cgi driver (for gifgraph() only, compile with -DGIFGRAPH)
 *		+------------------------------------------------------------
 *		#if !defined(GIFGRAPH)
 *		 main(argc,argv)                                   cgi driver
 *		 unescape(instr)           convert %20 in instr to blank, etc
 *		 x2c(what)  convert what="xx" hex to corresponding ascii char
 *		#endif
 *
 * --------------------------------------------------------------------------
 *
 * Notes:     o	dependencies: gifgraph() presently runs only
 *		on Unix-like systems where,
 *		  * the bc calculator is installed, and on your path
 *		  * the (default) shell correctly executes commands like
 *		      echo " some bc instructions " | bc -ql
 *		    that are emitted by gifgraph() with popen(command,"r")
 *	      o	See the "Notes:" under gifgraph() for further details.
 *
 * --------------------------------------------------------------------------
 * Revision History:
 * 09/23/12	J.Forkosh	Installation.
 * 09/23/12	J.Forkosh	Most recent revision
 * See  http://www.forkosh.com/gifsave89changelog.html  for further details.
 *
 ***************************************************************************/


/* -------------------------------------------------------------------------
standard headers...
-------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ==========================================================================
 * Function:	gifgraph ( gs, expression, xrange, trange )
 * Purpose:	user utility function, only for unix-like systems --
 *		  Graphs the function f(x) or f(x,t) defined by 'expression',
 *		  within the range of x's defined by the expression 'xrange',
 *		  with one such animation frame for each t within 'trange'.
 *		  See Notes: below for further explanation.
 * --------------------------------------------------------------------------
 * Arguments:	gs (I)		void * to a GS data structure (caller
 *				needn't know its internal structure)
 *		expression (I)	char * to an expression, passed to the
 *				bc calculator, defining the function
 *				to be graphed
 *		xrange (I)	char * to a bc expression of the form
 *				"x=0;x<100;x++" which gifgraph places
 *				inside a "for('xrange')"
 *		trange (I)	char * passed as NULL to graph just
 *				a single frame, i.e., f = f(x),
 *				or another "for('trange')" expression,
 *				where each t-value in the loop generates
 *				a frame f(x,t) for all x-values at that t.
 *		See Notes: below for further explanation of arguments
 * --------------------------------------------------------------------------
 * Returns:	( int )		#frames generated, i.e., 1 if trange=NULL,
 *				or else the number of times trange looped,
 *				or 0 for any error.
 * --------------------------------------------------------------------------
 * Notes:     o	dependencies: gifgraph presently only runs
 *		on Unix-like systems where,
 *		  * the bc calculator is installed, and on your path
 *		  * the (default) shell correctly executes commands like
 *		      echo " some bc instructions " | bc -ql
 *		    that are emitted by gifgraph() with popen(command,"r")
 *	      o	gifgraph generates a loop of calls to the bc calculator
 *		of the form,
 *		  "for ( trange ) { for ( xrange ) { expression } }",
 *              or just "for ( xrange ) { expression }" if trange==NULL,
 *		and calls putgif() for each sequence of xrange expression
 *		values, comprising the frames for an animated (or just
 *		one frame, when trange==NULL, for an static) gif graph.
 *	      o your expression must output one number per line,
 *		interpreted by gifgraph as the sequence of y=f(x;t) values
 *		to be graphed (or just f=f(x) if trange==NULL).
 *	      o	your (independent) variables in xrange and trange needn't
 *		explicitly be 'x' ant 't'; they simply have to be whatever
 *		you use in expression. Just remember, your arguments to
 *		gifgraph() are used to construct a bc "program",
 *		  "for ( trange ) { for ( xrange ) { expression } }",
 *		where a sequence of x-range values produced by expression
 *		comprises the "y-values" to be graphed.
 *	      o	Note that these graphed "y-values" are equally spaced
 *		along the x-axis, i.e., gifgraph() knows nothing about
 *		the "delta-x" between consecutive values, and just assumes
 *		it's some x_{i+1}-x_i=constant.
 * ======================================================================= */
/* --- entry point --- */
int	gifgraph ( void *gs, char *expression, char *xrange, char *trange )
{
/* ---
 * allocations and declarations
 * ------------------------------- */
/* --- use popen() to exec bc calculator from the shell --- */
char	*execbc = " bc -ql ";		/* invoke bc from shell */
FILE	*bcout  = NULL;			/* bc's stdout */
char	command[2048] = "\000",		/* shell command */
	bcline[512]   = "\000";		/* line from bc's stdout */
/* --- values generated by bc --- */
double	fvals[2048];			/* strtod converts bcline to double*/
int	maxvals = 2048,			/* our array size for fvals[] */
	nvals   = 0;			/* number of fvals in xrange */
/* --- convert fvals to pixels for a graph --- */
unsigned char *fpix=NULL, *pixgraph(),	/* pixels for graph of fvals[] */
	*pixcheck();			/*checkerboard if directives request*/
/* --- graph the pixels --- */
int	gifwidth(), gifheight(), putgif(); /* graph fpix */
/* --- #frames generated, i.e., #timesteps within 'trange' expression --- */
int	nframes = 0;			/* number of frames/vals in trange */
int	isbceof = 0;			/* true when bc emits "eof" signal */
/* --- directives controlling graph appearance --- */
char	*dirdelim="!!", *valdelim=",",	/* delims for directives, values */
	*directives=NULL, *values=NULL;	/* ptr to directives, values */
int	directvals[99], ndirect=0;	/* directive values */
int	chkbg=0,nchk=0, chkfg1=0,chkfg2=0, chksz=0,chkoff=0, /*chk params*/
	thissz=0, thisoff=0;		/* chk size,offset for current frame*/
/* ---
 * initialization
 * ----------------- */
/* --- check for missing args --- */
if ( gs == NULL				/* no gifsave99 data struct */
||   expression == NULL			/* or no f(x) expression to graph */
||   xrange == NULL ) goto end_of_job;	/* or no x0<=x<=x1 range, so quit */
/* --- check expression for embedded directives --- */
if ( (directives=strstr(expression,dirdelim)) != NULL ) { /*have directives*/
  *directives = '\000';			/*terminate expression at dirdelim*/
  directives += strlen(dirdelim);	/* bump directives past dirdelim */
  while ( 1 ) {				/* parse out directive values */
    if ( (values=strstr(directives,valdelim)) != NULL ) /*have more values */
      *values = '\000';			/* null-terminate this value */
    directvals[ndirect++] = atoi(directives); /* interpret as int */
    if ( values == NULL ) break;	/* finished last value */
    directives = values + strlen(valdelim); } /* bump to next value */
  } /* --- end-of-if(directives!=NULL) --- */
/* --- init checkerboard --- */
if ( ndirect == 5 ) {			/* fg/bg&nchk, fg1,fg2, chksz,chkoff */
  chkbg  = (directvals[0]<0? 0 : 1);	/* bg if <0, or fg if >0 and... */
  nchk   = abs(directvals[0]);		/* ... ncolors is abs() */
  chkfg1=directvals[1]; chkfg2=directvals[2]; /* checkerboard colors */
  chksz  = directvals[3];		/* size of checkerboard squares */
  chkoff = directvals[4];		/* offset per frame */
  } /* --- end-of-if(checkerboard) --- */
/* --- init command --- */
strcpy(command,"echo \"scale=6; ");	/*shell echo's program piped to bc*/
/* ---
 * if given trange, begin command with trange loop over animation frames
 * ----------------------------------------------------------------------- */
if ( trange != NULL ) {			/* have trange for animation */
  sprintf(command+strlen(command),	/* concat trange loop to command */
    "for(%.255s) {", trange ); }	/* "for(t0<t<t1){", or whatever */
/* ---
 * must have xrange loop and expression for at least one frame
 * -------------------------------------------------------------- */
/* ---evaluate expression over xrange loop (at current t-value, if any)--- */
sprintf(command+strlen(command),	/* concat xrange loop to command */
  "for(%.255s) { %.1024s }", xrange,expression ); /* "for(x0<x<x1){expr}" */
/* --- signal end-of-frame for animations --- */
if ( trange != NULL ) {			/* graphing animation sequence */
  sprintf(command+strlen(command),	/* emit end-of-frame after x-loop */
    "; print \\\"eof\\n\\\"; }" ); }	/* that's ; print "eof\n"; } to bc */
/* ---
 * complete the command, submit it to bc, and capture stdout with fvals[]
 * ----------------------------------------------------------------------- */
/* --- finish command with "| bc -ql" pipe to bc --- */
sprintf(command+strlen(command),	/* have for(){for(){};print "eof";}*/
  "\" | %s", execbc );			/* close echo "" and pipe to bc */
/* --- issue command and capture stdout --- */
if ( (bcout = popen(command,"r"))	/* issue command and capture stdout*/
== NULL ) goto end_of_job;		/* or quit if failed */
/* ---
 * now read bc's pipe one fval[] at a time
 * ------------------------------------------ */
while ( 1 ) {				/* read lines till eof */
  /* ---
   * accumulate fvals[] for current (or only) frame
   * ------------------------------------------------- */
  if ( fgets(bcline,255,bcout)!=NULL ) { /*line with fval[current t,next x]*/
    if ( memcmp(bcline,"eof",3) != 0 ) { /*unless it's end-of-frame signal*/
      if ( nvals < maxvals )		/* then until fvals array full */
        fvals[nvals++] = strtod(bcline,NULL);/*convert bc output and count*/
      continue; }			/* get next fvals[] */
    else isbceof = 1; }			/* end-of-frame signalled */
  /* ---
   * done with this frame: either bc end-of-frame, or end-of-file
   * --------------------------------------------------------------- */
  /* --- pixelize and then graph fvals[nvals] --- */
  if ( nvals > 0 ) {			/* got fvals[] to pixelize */
    fpix = pixgraph(gifwidth(gs),gifheight(gs),fvals,nvals);/*pixelize fvals*/
    if ( fpix!=NULL && nchk>0 ) {	/* checkerboard requested */
      thissz = chksz;  thisoff += chkoff; /* size,offset for this frame */
      fpix = pixcheck(gifwidth(gs),gifheight(gs),fpix, /*checkerboard graph*/
             thissz,thisoff, nchk,chkbg, chkfg1,chkfg2); }
    if ( fpix != NULL ) {		/* got pixels to graph */
      putgif(gs,fpix);			/* graph pixels */
      free(fpix);			/* get rid of pixels */
      nframes++; } }			/* count another frame in gif */
  /* --- do we have more frames? --- */
  if ( !isbceof ) break;		/* done after end-of-file */
  if ( nframes > 999 ) break;		/* sanity check on trange */
  /* --- reset for next frame */
  nvals   = 0;				/* reset #vals in fvals[] */
  isbceof = 0;				/* reset end-of-frame flag */
  } /* --- end-of-while(1) --- */
pclose(bcout);				/* done with bc pipe */
/* ---
 * end-of-job
 * ------------- */
end_of_job:
  return ( nframes );			/* #frames in gif back to caller */
} /* --- end-of-function gifgraph() --- */


#if !defined(GIFGRAPH)
/* -------------------------------------------------------------------------
additional standard headers
-------------------------------------------------------------------------- */
#include <ctype.h>			/* needed by unescape() only */

/* -------------------------------------------------------------------------
some character macros for gifgraph's main() driver (not required by gifgraph)
-------------------------------------------------------------------------- */
/* ---
 * macro to check if a string is empty
 * ----------------------------------- */
#define	isempty(s)  ((s)==NULL?1:(*(s)=='\000'?1:0))
/* ---
 * macro to strncpy() n bytes and make sure it's null-terminated
 * ------------------------------------------------------------- */
#define	strninit(target,source,n) \
	if( (target)!=NULL && (n)>=0 ) { \
	  char *thissource = (source); \
	  (target)[0] = '\000'; \
	  if ( (n)>0 && thissource!=NULL ) { \
	    strncpy((target),thissource,(n)); \
	    (target)[(n)] = '\000'; } } else
/* ---
 * macro to strip leading and trailing whitespace
 * ---------------------------------------------- */
#define	WHITESPACE  " \t\n\r\f\v"	/* skipped whitespace chars */
#define	trimwhite(thisstr) if ( (thisstr) != NULL ) { \
	int thislen = strlen(thisstr); \
	while ( --thislen >= 0 ) \
	  if ( strchr(WHITESPACE,(thisstr)[thislen]) != NULL ) \
	    (thisstr)[thislen] = '\000'; \
	  else break; \
	if ( (thislen = strspn((thisstr),WHITESPACE)) > 0 ) \
	  {strsqueeze((thisstr),thislen);} } else
/* --- 
 * macro to strcpy(s,s+n) using memmove() (also works for negative n)
 * ------------------------------------------------------------------ */
#define	strsqueeze(s,n) if((n)!=0) { if(!isempty((s))) { \
	int thislen3=strlen(s); \
	if ((n) >= thislen3) *(s) = '\000'; \
	else memmove(s,s+(n),1+thislen3-(n)); }} else/*user supplies final;*/

/* ==========================================================================
 * Function:	main ( int argc, char *argv[] )
 * Purpose:	cgi driver for gifgraph
 * --------------------------------------------------------------------------
 * Arguments:	argc (I)	as usual, if run from the command line
 *		argv (I)	Same as ?querystring arg1&arg2&etc ...
 *				argv[1] = #cols for graph, as #pixels
 *				argv[2] = #rows
 *				argv[3] = bc expression passed to gifgraph()
 *				argv[4] = bc xrange            "  gifgraph()
 *				argv[5] = optional bc trange   "  gifgraph()
 *		When run as http://.../cgi-bin/gifgraph?querystring
 *				? #cols&#rows& expression & xrange [& trange]
 * --------------------------------------------------------------------------
 * Returns:	( int )		always 0
 * --------------------------------------------------------------------------
 * Notes:     o	To simplify and better illustrate the programming involved,
 *		images are not cached. For production use, add an md5 hasher,
 *		hash the querystring to generate a filename[.gif], and just
 *		emit that file from a cache directory if it already exists.
 *		If not, call gifgraph() and save its gifimage as that file,
 *		for use the next time the same image is requested.
 *	      o	See http://www.forkosh.com/gifsave89.html
 *		for a more detailed discussion of the following information.
 * ======================================================================= */
/* --- entry point --- */
int	main ( int argc, char *argv[] )
{
/* --------------------------------------------------------------------------
Allocations and Declarations
-------------------------------------------------------------------------- */
/* --- parse input --- */
char	*querystring = getenv("QUERY_STRING"), /*?query_string signals cgi*/
	querybuff[2048] = "\000";	/* unescape() querystring */
int	isquery = (isempty(querystring)?0:1);/*cgi if given a ?query_string*/
int	iarg=0, maxargs=16, nargs=0;	/* #args from query or commandline */
char	queryarg[16][512];		/* local copy of args */
int	unescape();			/* replace %xx by its ascii char */
/* --- gifgraph() args --- */
int	nframes=0, gifgraph();		/* graphing function */
int	ncols       = 0,		/* screen width */
	nrows       = 0;		/* screen height */
int	row1        = 99999;		/*title at bottom (row1<0 to center)*/
char	*exprarg    = NULL,		/* e.g., "s(x+t)" */
	*xrange     = NULL,		/* e.g., "x=0.0;x<6.28;x+=0.1" */
	*trange     = NULL;		/* e.g., "t=0.0;t<6.28;t+=0.1" */
char	expression[2048] = "\000",	/* expression from exprarg */
	*delim = NULL,			/* optional expression$title */
	prefix[256]="\\small ", title[2048]="\000"; /*prefix plus title*/
char	*dirdelim="!!", *dirptr=NULL;	/* directives delim (need chkcolors) */
/* --- output to file or stdout --- */
char	*file       = (isquery? NULL : "gifgraphtest.gif");
FILE	*fp = stdout;			/* or fopen(file,"wb") if !isquery */
/* --- gifsave99 variables --- */
void	*gifimage=NULL, *gs, *newgif();	/* gifsave89 functions and vars */
int	plaintxtgif(), animategif(), nbytes=0, endgif();
int	bwcolortable[99] = { 255,255,255, 0,0,255, -1 }, /*ncolors=2*/
	chkcolortable[999]= { /* larger gif rgb colortable for checkerboard */
	  255,255,255,    0,  0,  255,	/* default  bg:0=white,   fg:1=blue*/
	  255,  0,  0,    0,255,  0,    0,  0,255,  /* 2=red,3=lime, 4=blue*/
	  255,255,  0,  255,  0,255,    0,255,255,  /* 5=ylw,6=fusch,7=aqua */
	  128,128,128,  192,192,192,                /* 8=gry,9=silvr */
          128,  0,  0,    0,128,  0,    0,  0,128,  /*10=mrn,11=grn, 12=navy*/
	  128,128,  0,  128,  0,128,    0,128,128,  /*13=olv,14=purp,15=teal*/
 	  -1 },  /*ncolors=16*/		/* colortable[] trailer */
	*colortable=bwcolortable, fgindex=1,bgindex=0;
/* --------------------------------------------------------------------------
Initialization (to see how this program uses gifgraph and gifsave89,
skip this section and read "Evaluate and Graph the function..." below)
-------------------------------------------------------------------------- */
/* ---
 * check for a cgi run without a ?query_string
 * ---------------------------------------------- */
if ( !isquery ) {			/* empty querystring */
  char	*host = getenv("HTTP_HOST"),	/* getenv("xxx") result for host, */
	*name = getenv("SERVER_NAME"),	/* and for server name */
	*addr = getenv("SERVER_ADDR");	/* and for server address */
  if ( !isempty(host)			/* if we have host */
  ||   !isempty(name)			/* or name */
  ||   !isempty(addr) ) isquery = 1; }	/* or addr, then assume cgi */
/* ---
 * preprocess http://...?query_string
 * ------------------------------------- */
if ( isquery ) {			/*parse ?query_string arg1&arg2&...*/
  *querybuff = '\000';			/* just in case no querystring */
  if ( !isempty(querystring) ) {	/* have ?querystring */
    strninit(querybuff,querystring,2047); /* local copy of ?querystring */
    unescape(querybuff); }		/* xlate %20 to blank, etc */
  querystring = querybuff;		/* now point into our buffer */
  /* --- parse out as arg1&arg2&... --- */
  while ( !isempty(querystring) && nargs<maxargs ) { /* still have args */
    char *delim = strchr(querystring,'&'); /* look for next & delimiter */
    if ( delim != NULL ) *delim = '\000'; /* null-terminate arg string */
    strninit(queryarg[nargs],querystring,511); /* copy arg */
    trimwhite(queryarg[nargs]);		/*remove leading/trailing whitespace*/
    nargs++;				/* and count another arg */
    if ( delim == NULL ) break;		/* last arg done */
    querystring = delim+1; }		/* next arg starts after & */
  } /* --- end-of-if(isquery) --- */
/* ---
 * or just copy commandline args
 * -------------------------------- */
if ( !isquery )				/* may have commandline args */
 if ( argc > 1 )			/* have argv[]'s after argv[0] */
   for ( iarg=1; iarg<argc&&nargs<maxargs; iarg++ ) { /* for each argv[] */
     strninit(queryarg[nargs],argv[iarg],511); /* copy it to queryargs[] */
     trimwhite(queryarg[nargs]);	/*remove leading/trailing whitespace*/
     nargs++; }				/* bump commandline arg count */
/* ---
 * set up gifgraph() args from queryargs[]
 * ----------------------------------------- */
ncols   = ( nargs>0? atoi(queryarg[0]) : 255 );
nrows   = ( nargs>1? atoi(queryarg[1]) : 192 );
exprarg = ( nargs>2? queryarg[2] :
    "pi=3.1416;x*s((x/4.+t)/(2.*pi))"
    "!! +16, -99,0, 8,-2"
    "$x*\\left.\\sin\\left(\\frac{x/4+t}{2\\pi}\\right)"
    "\\right|_{\\scriptsize x=0,t=0}^{\\scriptsize x=200,t=100}" );
 /* --- "pi=3.1416;x*s((x/4.+t)/(2.*pi))"
        "$x*\\left.\\sin\\left(\\frac{x/4+t}{2\\pi}\\right)"
        "\\right|_{\\scriptsize x=0,t=0}^{\\scriptsize x=200,t=100}" ); --- */
xrange  = ( nargs>3? queryarg[3] : "x=0.0;x<200.;x+=1." );
trange  = ( nargs>4? queryarg[4] : "t=0.0;t<100.;t+=1." );
if ( trange != NULL )			/* have trange arg */
  if(strcmp(trange,"0")==0) trange=NULL; /* ...& 0 &... signals no trange */
if ( exprarg == NULL ) goto end_of_job;	/* no expression */
/* --- get expression and title from exprarg --- */
strcpy(expression,exprarg);		/* local copy of expression */
if ( strstr(expression,dirdelim) != NULL ) /* directives in expression */
  colortable = chkcolortable;
if ( (delim=strchr(expression,'$')) != NULL ) { /*have optional expr$title*/
  *delim++ = '\000';			/* end expression at $, bump delim*/
  while( *delim!='\000' )		/* remove leading whitespace */
    if(isspace((int)(*delim)))delim++; else break; /*break at 1st nonwhite*/
  if ( *delim == 'C' ) {		/*leading C for vertical centering*/
    delim++; row1 = (-1); } }		/*skip C char and signal centering*/
else delim = expression;		/* no $?, use expression as title */
if ( *delim != '\000' ) {		/* have graph title */
  strcpy(title,prefix);			/* begin with standard prefix */
  strcat(title,delim);			/* pick up title afterwards */
  if ( (dirptr=strstr(title,dirdelim)) != NULL ) /* directives in title */
    *dirptr = '\000'; }			/* remove directives from title */
/* --------------------------------------------------------------------------
Evaluate and Graph the function, and Output the completed gif
-------------------------------------------------------------------------- */
if ( (gs=newgif(&gifimage,ncols,nrows,colortable,bgindex)) /*init gifsave89*/
!= NULL ) {				/* check for successful init */
  /* ---
   * evaluate and graph
   * --------------------- */
  if ( trange != NULL ) animategif(gs,0,0,-1,2); /*init animation if needed*/
  plaintxtgif(gs,-1,row1,-1,-1,fgindex,bgindex,title);
  nframes = gifgraph(gs,expression,xrange,trange); /* graph expression */
  nbytes  = endgif(gs);			/* all done */
  /* ---
   * output the gif graph
   * ----------------------- */
  if ( nframes>0 && nbytes>0 )		/* graphed function successfully */
    /* --- cgi query to stdout, commandline to file --- */
    if ( (fp=(isquery?stdout:fopen(file,"wb"))) != NULL ) {
      int nwritten = 0;
      if ( fp == stdout ) {		/* first write http headers */
        fprintf(stdout,"Cache-Control: max-age=9999\n");
        fprintf(stdout,"Content-Length: %d\n",nbytes );
        fprintf(stdout,"Content-Type: image/gif\n\n"); }
      nwritten = fwrite(gifimage,1,nbytes,fp); /* output the gif */
      if ( fp != stdout ) {		/* message to user, close file */
        printf("gifgraph> wrote %d bytes to %s\n",nwritten,file);
        fclose(fp); }			/* done with file */
      } /* --- end-of-if(fp!=NULL) --- */
  } /* --- end-of-if((gs=newgif())!=NULL) --- */
if ( gifimage != NULL ) free(gifimage);
end_of_job:
  return(0);
} /* --- end-of-function main() --- */


/* ==========================================================================
 * Functions:	int  unescape ( char *instr )
 *		char x2c ( char *what )
 * Purpose:	unescape replaces 3-character sequences %xx in instr
 *		    with the single character represented by hex xx.
 *		x2c returns the single character represented by hex xx
 *		    passed as a 2-character sequence in what.
 * --------------------------------------------------------------------------
 * Arguments:	instr (I)	char * containing null-terminated
 *				string with embedded %xx sequences
 *				to be converted.
 *		what (I)	char * whose first 2 characters are
 *				interpreted as ascii representations
 *				of hex digits.
 * --------------------------------------------------------------------------
 * Returns:	( int )		length of instr string after replacements.
 *		( char )	x2c returns the single char
 *				corresponding to hex xx passed in what.
 * --------------------------------------------------------------------------
 * Notes:     o	These two functions were taken from util.c in
 *   ftp://ftp.ncsa.uiuc.edu/Web/httpd/Unix/ncsa_httpd/cgi/ncsa-default.tar.Z
 * ======================================================================= */
/* --- entry point --- */
int unescape ( char *instr ) {
  /* --- allocations and declarations --- */
  int  x=0, y=0;			/* instr[] indexes */
  char x2c();				/* xlate xx in %xx to ascii char */
  if ( instr != NULL ) {		/* check caller's arg */
   /* --- xlate %nn to corresponding char --- */
   for( x=y=0; instr[y]!='\000'; ++x,++y ) {
    if ( !isprint(((int)instr[y])) ) instr[y]=' '; /*replace ctrl with ' '*/
    if( (instr[x]=instr[y]) == '%' )	/* shift as necessary, check for % */
      if( isxdigit(((int)instr[y+1]))	/* found hex digit after % */
      &&  isxdigit(((int)instr[y+2])) )	/* and 2nd hex digit after that */
        { instr[x] = x2c(&instr[y+1]);	/* xlate %xx to char */
          y+=2; } }			/* 3 chars %xx replaced by 1 char */
   instr[x] = '\000'; }			/* null-terminate instr */
  return ( x );				/* return its length */
  } /* --- end-of-function unescape() --- */

/* --- entry point --- */
char x2c(char *what) {
  char what0 = (char)toupper((int)what[0]),
       what1 = (char)toupper((int)what[1]);
  char digit = (char)0;
  digit  = (what0 >= 'A' ? ((what0 & 0xdf) - 'A')+10 : (what0 - '0'));
  digit *= 16;
  digit += (what1 >= 'A' ? ((what1 & 0xdf) - 'A')+10 : (what1 - '0'));
  return ( digit );
  } /* --- end-of-function x2c() --- */
#endif
/* ====================== END-OF-FILE GIFGRAPH.C ========================= */
