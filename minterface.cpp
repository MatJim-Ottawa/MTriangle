
/* If SINGLE is defined when triangle.o is compiled, it should also be       */
/*   defined here.  If not, it should not be defined here.                   */

/* #define SINGLE */

#ifdef SINGLE
#define TRIREAL float
#else /* not SINGLE */
#define TRIREAL double
#endif /* not SINGLE */

#include "triangle.h"
#include "mex.h"
#include <stdlib.h>
#include <stdio.h>


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	/*  
		P,
		PB,
		PA,
		S,
		SB,
		H
		a
		q
		Y
		SS
		j
		D
		V
		Q
	*/

	if (!(nrhs > 3))
	{
		mexPrintf("Value: %d", nrhs);
		mexErrMsgIdAndTxt("MTriangle:NoInputs", "Must provide inputs");
	}

	/* Type Checking of inputs! */
	if (!mxIsDouble(prhs[0])) 
	{
		
		mexErrMsgIdAndTxt("MTriangle:notDouble", "Input matrix must be type double.");
	}

	/* Input Variables */

	double *inMatrixP, *inMatrixPB, *inMatrixPA, *inMatrixS;
	size_t Pncols, Pnrows, PBnrows, PAnrows, Snrows;

	/* Output Variables  in C */

	double *outMatrix;

	inMatrixP = mxGetPr(prhs[0]);
	Pncols = mxGetN(prhs[0]);
	Pnrows = mxGetM(prhs[0]);

	inMatrixPB = mxGetPr(prhs[1]);
	PBnrows = mxGetM(prhs[1]);

	inMatrixPA = mxGetPr(prhs[2]);
	PAnrows = mxGetM(prhs[2]);

	inMatrixS = mxGetPr(prhs[3]);
	Snrows = mxGetM(prhs[3]);

	struct triangulateio in, mid, vorout;

	in.numberofpoints = Pnrows;
	in.pointlist = inMatrixP;


	in.pointmarkerlist = (int *)malloc(in.numberofpoints * sizeof(int));

	for (int i = 0; i < PBnrows; i++)
	{
		in.pointmarkerlist[i] = (int)inMatrixPB[i];
		mexPrintf("pointmarkerList\n");
	}

	in.numberofpointattributes = PAnrows;
	in.pointattributelist = inMatrixPA;

	in.numberofsegments = Snrows;
	in.segmentlist = (int *)malloc(in.numberofpoints * 2 * sizeof(int));

	for (int i = 0; i < Snrows * 2; i++)
	{
		in.segmentlist[i] = (int)inMatrixS[i];
		mexPrintf("segList\n");
	}

	in.numberofholes = 0;
	in.numberofregions = 0;

	/* Make necessary initializations so that Triangle can return a */
	/*   triangulation in `mid' and a voronoi diagram in `vorout'.  */

	mid.pointlist = (TRIREAL *)NULL;            /* Not needed if -N switch used. */
	/* Not needed if -N switch used or number of point attributes is zero: */
	mid.pointattributelist = (TRIREAL *)NULL;
	mid.pointmarkerlist = (int *)NULL; /* Not needed if -N or -B switch used. */
	mid.trianglelist = (int *)NULL;          /* Not needed if -E switch used. */
	/* Not needed if -E switch used or number of triangle attributes is zero: */
	mid.triangleattributelist = (TRIREAL *)NULL;
	mid.neighborlist = (int *)NULL;         /* Needed only if -n switch used. */
	/* Needed only if segments are output (-p or -c) and -P not used: */
	mid.segmentlist = (int *)NULL;
	/* Needed only if segments are output (-p or -c) and -P and -B not used: */
	mid.segmentmarkerlist = (int *)NULL;
	mid.edgelist = (int *)NULL;             /* Needed only if -e switch used. */
	mid.edgemarkerlist = (int *)NULL;   /* Needed if -e used and -B not used. */

	vorout.pointlist = (TRIREAL *)NULL;        /* Needed only if -v switch used. */
	/* Needed only if -v switch used and number of attributes is not zero: */
	vorout.pointattributelist = (TRIREAL *)NULL;
	vorout.edgelist = (int *)NULL;          /* Needed only if -v switch used. */
	vorout.normlist = (TRIREAL *)NULL;         /* Needed only if -v switch used. */

	/* Triangulate the points.  Switches are chosen to read and write a  */
	/*   PSLG (p), preserve the convex hull (c), assign a regional       */
	/*   attribute to each element (A), and                              */
	/*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
	/*   neighbor list (n).												*/


	triangulate("pczAevn" , &in, &mid, &vorout);


}

