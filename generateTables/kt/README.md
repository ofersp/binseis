Kippenhahn-Thomas tables for `bstar`
====================================

Scripts in this folder produce the file `data/tables/ktTables.bin`, which 
contains geometric correction tables required by the `stellarStructure` code 
in the binary case. See appendix 2 of our paper and Kippenhahn and Thomas 1970 
(KT1970) for details.

To generate this binary table file the following steps should be performed -

Initialize the Binary-Seismology toolbox
----------------------------------------

Enter a MATLAB prompt and run `initBinseisToolbox` at the base of the 
source tree.

Generate the tables
-------------------

Within the same MATLAB prompt run `ktBuildTables`. This will compute the 
tables (could take a couple of hours) and produce the file `ktTables.bin` in 
`data/tables/kt` from which it can be read by the `stellarStructure` code.

References
----------
[KT1970] Kippenhahn R., Thomas H., 1970, in IAU Colloq. 4: Stellar Rotation, 
A. Slettebak, ed., p. 20
