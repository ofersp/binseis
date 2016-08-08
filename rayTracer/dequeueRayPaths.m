%
% DEQUEUERAYPATHS Read ray paths resulting from a trace execution.
%
% rayPaths = dequeueRayPaths(fnameRayPathQueue, numRays, wroteSurface)

function rayPaths = dequeueRayPaths(fnameRayPathQueue, numRays, wroteSurface)

	numFields = 8;
	rayPaths = cell(numRays, 1);
	fid = fopen(fnameRayPathQueue, 'rb');	

	for i=1:numRays
	
		rayPaths{i}.simSamples = fread(fid, 1, 'int32');
		rayPaths{i}.surfaceSamples = fread(fid, 1, 'int32');
		rayPaths{i}.stopCond = fread(fid, 1, 'int32');

		if wroteSurface; readSamples = rayPaths{i}.surfaceSamples + 1;
		else; readSamples = rayPaths{i}.simSamples; end

		M = fread(fid, [numFields, readSamples], 'single');
		
		rayPaths{i}.r = M(1:3,:)';
		rayPaths{i}.k = M(4:6,:)';
		rayPaths{i}.omega = M(7,:)';
		rayPaths{i}.atSurface = M(8,:)';
	end	
	
	fclose(fid);
