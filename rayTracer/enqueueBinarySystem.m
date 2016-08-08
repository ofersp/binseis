%
% ENQUEUEBINARYSYSTEM Place bSys in trace queue.
%
% enqueueBinarySystem(bSys, fnameBinarySystem)

function enqueueBinarySystem(bSys, fnameBinarySystem)

	fid = fopen(fnameBinarySystem, 'wb');
	
	fwrite(fid, bSys.D, 'double');
	fwrite(fid, bSys.M1, 'double');
	fwrite(fid, bSys.M2, 'double');
	fwrite(fid, bSys.r1, 'double');
	fwrite(fid, bSys.r2, 'double');
	fwrite(fid, bSys.omegaOrbit, 'double');
	fwrite(fid, bSys.L1, 'double');
	fwrite(fid, bSys.Rbound, 'double');
	fwrite(fid, bSys.Lsmooth, 'double');
	
	fwrite(fid, bSys.sst.R(1), 'double');
	fwrite(fid, bSys.sst.R(end), 'double');
	fwrite(fid, bSys.sst.resBoundR, 'double');
	fwrite(fid, bSys.sst.resBoundI, 'int32');
	fwrite(fid, bSys.sst.basePhotosphereP, 'double');
	fwrite(fid, bSys.sst.basePhotosphereVs, 'double');
	fwrite(fid, numel(bSys.sst.P), 'int32');
	fwrite(fid, bSys.sst.P, 'double');
	fwrite(fid, bSys.sst.rho, 'double');
	fwrite(fid, bSys.sst.Gamma1, 'double');
	fwrite(fid, bSys.sst.avgG1, 'double');		
	
	fwrite(fid, bSys.kt.psi(1), 'double');
	fwrite(fid, bSys.kt.psi(end), 'double');
	fwrite(fid, numel(bSys.kt.r), 'int32');
	fwrite(fid, bSys.kt.r, 'double');

	fclose(fid);
