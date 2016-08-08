%
% ENQUEUERAY Add ray init condition to given trace queue.
%
% enqueueRay(ray, fnameRayInitCondQueue)

function enqueueRay(ray, fnameRayInitCondQueue)

	% queue ray for tracing

	fid = fopen(fnameRayInitCondQueue, 'ab');
	
	fwrite(fid, ray.r0, 'double');
	fwrite(fid, ray.k0, 'double');
	fwrite(fid, ray.branch, 'double');
	fwrite(fid, ray.omega, 'double');
	fwrite(fid, ray.timeStep, 'double');
	fwrite(fid, ray.maxSteps, 'int32');
	fwrite(fid, ray.sampleEach, 'int32');
	fwrite(fid, ray.maxSamples, 'int32');
	
	fclose(fid);
