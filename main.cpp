/*
 * main.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: tpmaxwel
 */

// #include "cdms_reader.h"
// #include "threads_test.h"
#include "thresholdFunction.h"
// #include "dotProduct.h"
#include <stdio.h>


int main(void) {

//    const char* input_file = "~/Data/comp-ECMWF/ecmwf.inst3_3d_wxm_Np.20110502_00z+20110502_00z.nc";
//    int rv = cdms_test( input_file );

	int rv = thresholdFunction();

	return rv;

}
