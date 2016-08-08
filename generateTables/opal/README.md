Opacity tables for `bstar`
==========================

Scripts in this folder produce the file `data/tables/opalX70Z02.bin`, an 
opacity table required by the `stellarStructure` code. 

This table is produced from those of Iglesias and Rogers 1996 (IR96) at the 
high temperature range, and those of Alexander and Ferguson 2005 (AF2005) 
at low temperatures. Both are based on the Grevesse and Noels 1993 (GN93) 
solar abundances and for the stellar composition X=0.7, Z=0.02. 

To generate the binary table file the following steps should be performed -

Obtain the IR96 and AF2005 data-sets
------------------------------------

  - From `http://opalopacity.llnl.gov/existing.html` download the Type 1 
    GN93 solar composition table `GN93hz`. A direct link at the time of 
    writing is -  
    `http://opalopacity.llnl.gov/pub/opal/type1data/GN93/ascii/GN93hz`

  - From `http://webs.wichita.edu/physics/opacity/` download the low 
    temperature GN93 solar composition table `f05.g93.tar.gz` (based on 
    the AF2005 calculation). A direct link at the time of writing is -  
    `http://webs.wichita.edu/physics/opacity/Data/f05.g93.tar.gz`

  - Copy these two files into the `data/tables.external/opalTable`
    directory of the source tree.

Initialize the Binary-Seismology toolbox
----------------------------------------

Enter a MATLAB prompt and run `initBinseisToolbox` at the base of the 
source tree.

Extract tables for the specific composition
-------------------------------------------

Within the MATLAB prompt run `opalExtractTables`. This will produce the 
MATLAB loadable ascii matrices `irX70Z02.txt` and `afX70Z02.txt` in 
`data/tables.external/opalTable` (their row/column format is similar to the 
OPAL format). These encode composition specific tables extracted from 
`GN93hz` and the archive `f05.g93.tar.gz`.

Combine the IR96 and AF2005 composition specific tables
-------------------------------------------------------

Within the same MATLAB prompt run `opalCombineTables`. This will load the 
ascii files `irX70Z02.txt`, `afX70Z02.txt` and interpolate them into 
internal tables having constant step-size. Finally these two tables will be
combined to produce the final table file `opalX70Z02.bin` which smoothly 
interpolates between the two in their overlap region. The final table is 
then stored in `data/tables/opal` from which it can be read by the 
`stellarStructure` code.

Output table file format
------------------------

The binary opacity table file produced, `opalX70Z02.bin`, has the following 
format -

	1 int - numLogT
	1 int - numLogR
 	1 double - minLogT
	1 double - maxLogT
 	1 double - minLogR
	1 double - maxLogR
	numLogT*numLogR double - Log(kappa) equi-R columns in column first order

with the following definitions taken from the OPAL file header -

	T6 = 1.e-6*T[K]
	R = density[g/cm^3]/T6^3
	LogR = Log10(R)
	LogT = Log10(T[K])
	Log(kappa) = Log10(Rosseland mean opacity [cm^2/g])

References
----------

[IR96] Iglesias C. A., Rogers F. J., 1996, Ap. J., 464, 943  
[AF2005] Jason W. Ferguson, et al., 2005, arXiv:astro-ph/0502045v1  
[GN93] Grevesse N., Noels A., 1993, in Origin and Evolution of 
the Elements, N. Prantzos, E. Vangioni-Flam, & M. Casse, ed., pp.15–25  


