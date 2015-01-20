
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
#include <string.h>



/*****************************************************************************/
/*                                                                           */
/*  report()   Print the input or output.                                    */
/*                                                                           */
/*****************************************************************************/

void report(struct triangulateio *io,int markers,int reporttriangles,int reportneighbors,int reportsegments,
            int reportedges,int reportnorms)
{
  int i, j;

  for (i = 0; i < io->numberofpoints; i++) {
    mexPrintf("Point %4d:", i);
    for (j = 0; j < 2; j++) {
      mexPrintf("  %.6g", io->pointlist[i * 2 + j]);
    }
    if (io->numberofpointattributes > 0) {
      mexPrintf("   attributes");
    }
    for (j = 0; j < io->numberofpointattributes; j++) {
      mexPrintf("  %.6g",
             io->pointattributelist[i * io->numberofpointattributes + j]);
    }
    if (markers) {
      mexPrintf("   marker %d\n", io->pointmarkerlist[i]);
    } else {
      mexPrintf("\n");
    }
  }
  mexPrintf("\n");

  if (reporttriangles || reportneighbors) {
    for (i = 0; i < io->numberoftriangles; i++) {
      if (reporttriangles) {
        mexPrintf("Triangle %4d points:", i);
        for (j = 0; j < io->numberofcorners; j++) {
          mexPrintf("  %4d", io->trianglelist[i * io->numberofcorners + j]);
        }
        if (io->numberoftriangleattributes > 0) {
          mexPrintf("   attributes");
        }
        for (j = 0; j < io->numberoftriangleattributes; j++) {
          mexPrintf("  %.6g", io->triangleattributelist[i *
                                         io->numberoftriangleattributes + j]);
        }
        mexPrintf("\n");
      }
      if (reportneighbors) {
        mexPrintf("Triangle %4d neighbors:", i);
        for (j = 0; j < 3; j++) {
          mexPrintf("  %4d", io->neighborlist[i * 3 + j]);
        }
        mexPrintf("\n");
      }
    }
    mexPrintf("\n");
  }

  if (reportsegments) {
    for (i = 0; i < io->numberofsegments; i++) {
      mexPrintf("Segment %4d points:", i);
      for (j = 0; j < 2; j++) {
        mexPrintf("  %4d", io->segmentlist[i * 2 + j]);
      }
      if (markers) {
        mexPrintf("   marker %d\n", io->segmentmarkerlist[i]);
      } else {
        mexPrintf("\n");
      }
    }
    mexPrintf("\n");
  }

  if (reportedges) {
    for (i = 0; i < io->numberofedges; i++) {
      mexPrintf("Edge %4d points:", i);
      for (j = 0; j < 2; j++) {
        mexPrintf("  %4d", io->edgelist[i * 2 + j]);
      }
      if (reportnorms && (io->edgelist[i * 2 + 1] == -1)) {
        for (j = 0; j < 2; j++) {
          mexPrintf("  %.6g", io->normlist[i * 2 + j]);
        }
      }
      if (markers) {
        mexPrintf("   marker %d\n", io->edgemarkerlist[i]);
      } else {
        mexPrintf("\n");
      }
    }
    mexPrintf("\n");
  }
}




// Matlab is expecting this function

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

	int buflen = mxGetNumberOfElements(prhs[4]) + 1;

	char *option;
	option = (char *)mxCalloc(buflen, sizeof(char));
	
	/* Output Variables  in C */

	if (mxGetString(prhs[4], option, buflen) != 0)
		mexErrMsgIdAndTxt("MATLAB:explore:invalidStringArray",
		"Could not convert string data.");


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
	in.pointlist = (REAL *)malloc(Pnrows * 2 * sizeof(REAL));
    
    for (int i = 0; i < Pnrows; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            in.pointlist[i*2 + j] = inMatrixP[i + Pnrows*j];
        }

    }

	in.pointmarkerlist = (int *)malloc(Pnrows * sizeof(int));

	for (int i = 0; i < PBnrows; i++)
	{
		in.pointmarkerlist[i] = (int)inMatrixPB[i];
        
	}

	in.numberofpointattributes = PAnrows;
	in.pointattributelist = (TRIREAL *)NULL;

	in.numberofsegments = Snrows;
	in.segmentlist = (int *)malloc(Snrows * 2 * sizeof(int));

	for (int i = 0; i < Snrows; i++)
	{
        for (int j = 0; j < 2; j++)
        {
            in.segmentlist[i*2 + j] = (int)inMatrixS[i + Snrows*j];
        }
	}

	in.segmentmarkerlist = (int *)NULL;

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



	triangulate(option, &in, &mid, &vorout);
    


//    ************** Matrix P ***************************
    plhs[0] = mxCreateNumericMatrix(mid.numberofpoints, 2, mxDOUBLE_CLASS, mxREAL);
    
    double  *pointer; 
    pointer = mxGetPr(plhs[0]);

    /* Copy data into the mxArray */
    for (int index = 0; index < mid.numberofpoints; index++ ) {
        for (int col = 0; col < 2; col++)
        {    
        pointer[index + mid.numberofpoints*col] = mid.pointlist[index*2 + col];
        }
    }
    
//    ************** Matrix PB ***************************
    plhs[1] = mxCreateNumericMatrix(mid.numberofpoints, 1, mxDOUBLE_CLASS, mxREAL);

    pointer = mxGetPr(plhs[1]);

    /* Copy data into the mxArray */
    for (int index = 0; index < mid.numberofpoints; index++ ) {   
        pointer[index] = mid.pointmarkerlist[index];
    }
    
//    ************** Matrix PA ***************************
    plhs[2] = mxCreateNumericMatrix(mid.numberofpoints, mid.numberofpointattributes, mxDOUBLE_CLASS, mxREAL);

    pointer = mxGetPr(plhs[2]);

    /* Copy data into the mxArray */
    for (int index = 0; index < mid.numberofpoints; index++ ) { 
        for( int col = 0; col < mid.numberofpointattributes; col++ ) {
            pointer[mid.numberofpointattributes*index + col] = mid.pointattributelist[mid.numberofpointattributes*index + col];
        }
        
    }    
//    ************** Matrix T ***************************
    plhs[3] = mxCreateNumericMatrix(mid.numberoftriangles, mid.numberofcorners, mxDOUBLE_CLASS, mxREAL);

    pointer = mxGetPr(plhs[3]);

    /* Copy data into the mxArray */
    for (int index = 0; index < mid.numberoftriangles; index++ ) { 
        for( int col = 0; col < mid.numberofcorners; col++ ) {
            pointer[index + mid.numberoftriangles*col] = mid.trianglelist[mid.numberofcorners*index + col];
        }
        
    } 
    
//    ************** Matrix S ***************************
    plhs[4] = mxCreateNumericMatrix(mid.numberofsegments,  2, mxDOUBLE_CLASS, mxREAL);

    pointer = mxGetPr(plhs[4]);

    /* Copy data into the mxArray */
    for (int index = 0; index < mid.numberofsegments; index++ ) { 
        for( int col = 0; col < 2; col++ ) {
            pointer[index + mid.numberofsegments*col] = mid.segmentlist[2*index + col];
        }
        
    } 
    
//    ************** Matrix SB ***************************
    plhs[5] = mxCreateNumericMatrix(mid.numberofsegments,  1, mxDOUBLE_CLASS, mxREAL);

    pointer = mxGetPr(plhs[5]);

    /* Copy data into the mxArray */
    for (int index = 0; index < mid.numberofsegments; index++ ) { 
            pointer[index] = mid.segmentmarkerlist[index];
    }     
    
//    ************** Matrix E ***************************
    plhs[6] = mxCreateNumericMatrix(mid.numberofedges,  2, mxDOUBLE_CLASS, mxREAL);

    pointer = mxGetPr(plhs[6]);

    /* Copy data into the mxArray */
    for (int index = 0; index < mid.numberofedges; index++ ) { 
        for( int col = 0; col < 2; col++ ) {
            pointer[index + mid.numberofedges*col] = mid.edgelist[2*index + col];
        }
        
    }    
    
//    ************** Matrix EB ***************************
    plhs[7] = mxCreateNumericMatrix(mid.numberofedges,  1, mxDOUBLE_CLASS, mxREAL);

    pointer = mxGetPr(plhs[7]);

    /* Copy data into the mxArray */
    for (int index = 0; index < mid.numberofedges; index++ ) { 
            pointer[index] = mid.edgemarkerlist[index];
    }     
    
//    ************** Matrix VP ***************************
    plhs[8] = mxCreateNumericMatrix(vorout.numberofpoints,  2, mxDOUBLE_CLASS, mxREAL);

    pointer = mxGetPr(plhs[8]);

    /* Copy data into the mxArray */
    for (int index = 0; index < vorout.numberofpoints; index++ ) { 
        for( int col = 0; col < 2; col++ ) {
            pointer[index + vorout.numberofpoints*col] = vorout.pointlist[2*index + col];
        }
        
    } 
    
//    ************** Matrix VE ***************************
    plhs[9] = mxCreateNumericMatrix(vorout.numberofedges,  2, mxDOUBLE_CLASS, mxREAL);

    pointer = mxGetPr(plhs[9]);

    /* Copy data into the mxArray */
    for (int index = 0; index < vorout.numberofedges; index++ ) { 
        for( int col = 0; col < 2; col++ ) {
            pointer[index + vorout.numberofedges*col] = vorout.edgelist[2*index + col];
        }
        
    }  
    
//    ************** Matrix VN ***************************
    plhs[10] = mxCreateNumericMatrix(vorout.numberofedges,  2, mxDOUBLE_CLASS, mxREAL);

    pointer = mxGetPr(plhs[10]);

    /* Copy data into the mxArray */
    for (int index = 0; index < vorout.numberofedges; index++ ) { 
        for( int col = 0; col < 2; col++ ) {
            pointer[index + vorout.numberofedges*col] = vorout.normlist[2*index + col];
        }
        
    } 
    
//    ************** Matrix VA ***************************
    plhs[11] = mxCreateNumericMatrix(vorout.numberofpoints,  mid.numberofpointattributes, mxDOUBLE_CLASS, mxREAL);

    pointer = mxGetPr(plhs[11]);

    /* Copy data into the mxArray */
    for (int index = 0; index < vorout.numberofpoints; index++ ) { 
        for( int col = 0; col < mid.numberofpointattributes; col++ ) {
            pointer[index + vorout.numberofpoints*col] = vorout.pointattributelist[mid.numberofpointattributes*index + col];
        }
        
    } 
    
  free(in.pointlist);
  free(in.pointattributelist);
  free(in.pointmarkerlist);
  free(in.segmentlist);
  free(in.segmentmarkerlist);
//  free(in.regionlist);  
  free(mid.pointlist);
  free(mid.pointattributelist);
  free(mid.pointmarkerlist);
  free(mid.trianglelist);
  free(mid.triangleattributelist);
  /* Free(mid.trianglearealist); */
  free(mid.neighborlist);
  free(mid.segmentlist);
  free(mid.segmentmarkerlist);
  free(mid.edgelist);
  free(mid.edgemarkerlist);
  free(vorout.pointlist);
  free(vorout.pointattributelist);
  free(vorout.edgelist);
  free(vorout.normlist);

  /*  Free(out.pointlist);
  Free(out.pointattributelist);
  Free(out.trianglelist);
  Free(out.triangleattributelist); */
    return;
  


}



