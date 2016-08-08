%
% KTFUNCTIONS Compute the dimensionless Kippenhahn-Thomas functions.
%
% [psi,V,S,r,avgG1,avgG2,avgG3,avgInvG1,avgInvG2,avgInvG3] = ktFunctions()

function [psi,V,S,r,avgG1,avgG2,avgG3,avgInvG1,avgInvG2,avgInvG3] = ...
  ktFunctions()

	constants;

	numSurfaces = 110;
	alpha = 0.85;
	beta = 0.1;
	delta = 2;
	spaceStep = 1e-3;	

	M1 = 1; M2 = 1; hD = 1;
	r1 = [-hD, 0, 0];
	r2 = [hD, 0, 0];
	omegaOrbit = sqrt(G/4);
	dV = spaceStep^3;	

	% pre-allocate buffers

	V = zeros(1, numSurfaces); S = V; 
	avgG1 = V; avgG2 = V; avgG3 = V;
	avgInvG1 = V; avgInvG2 = V; avgInvG3 = V;

  Dx = [1;0;-1];
	Dy = [1,0,-1]; Dz(1,1,:) = [1,0,-1];

	% define the set of equipotentials at which to sample

	P1 = [-1-alpha, 0, 0]; P2 = [-1-beta, 0, 0];
	
	[X, Y, Z] = coordsGrid(P1, P2, (alpha-beta)/numSurfaces);
	psiBoundary = flipud(samplePsiExternal(r1,r2,M1,M2,omegaOrbit,X,Y,Z)/G);

	for i=1:numSurfaces
		psi(i) = (psiBoundary(i) + psiBoundary(i+1)) / 2; 
	end

	% for all slices on the negative y negative z part of the grid

	for currZ = 0:-spaceStep:-alpha
    msgStr = sprintf('Processing slice %d/%d\r',...
      1+round(-currZ/spaceStep), 1+round(alpha/spaceStep));
		fprintf(1, msgStr);

		% find the effective potential, the gravitational acceleration and it's 
    % inverse within the current slice

		C1 = [-alpha, -alpha, currZ - 2*spaceStep] + r1;
		C2 = [0, 0, currZ];
		[X, Y, Z] = coordsGrid(C1, C2, spaceStep);
		Psi = samplePsiExternal(r1, r2, M1, M2, omegaOrbit, X, Y, Z) / G;			

    gx = convn(-Psi, Dx, 'same') / (2*spaceStep); gx = squeeze(gx(:,:,2));
    gy = convn(-Psi, Dy, 'same') / (2*spaceStep); gy = squeeze(gy(:,:,2));
    gz = convn(-Psi, Dz, 'same') / (2*spaceStep); gz = squeeze(gz(:,:,2));
		
		Psi = Psi(:,:,2); % we can now keep just the center plane
		gSqr = gx.^2 + gy.^2 + gz.^2;
		gSqr(:,end) = gSqr(:,end-1);
		gSqr(end,:) = gSqr(end-1,:);
		gSqr = gSqr(:);
		g = sqrt(gSqr);		

		% accumulate the KT functions V, S, r, avgG, avgInvG for each 
		% surface in the current slice
	
		for i=1:numSurfaces

			dpsi = (psiBoundary(i+1) - psiBoundary(i)) * delta;
			dVdpsi = dV / dpsi;

			interior = 1.0 * (Psi(:) < psi(i));
			shell = 1.0*((Psi(:)>(psi(i)-dpsi/2)).*(Psi(:)<(psi(i)+dpsi/2)));

			V(i) = V(i) + sum(interior) * dV;
      S(i) = S(i) + sum(shell .* g) * (dV/dpsi);

			shellDotG2 = shell .* gSqr;
			shellDotG3 = shellDotG2 .* g;
			shellDotG4 = shellDotG2 .* shellDotG2;
			avgG1(i) = avgG1(i) + sum(shellDotG2) * dVdpsi;
			avgG2(i) = avgG2(i) + sum(shellDotG3) * dVdpsi;
			avgG3(i) = avgG3(i) + sum(shellDotG4) * dVdpsi;

			shellDotGM1 = shell ./ g;
			shellDotGM2 = shellDotGM1 .* shellDotGM1;
			avgInvG1(i) = avgInvG1(i) + sum(shell) * dVdpsi;
			avgInvG2(i) = avgInvG2(i) + sum(shellDotGM1) * dVdpsi;
			avgInvG3(i) = avgInvG3(i) + sum(shellDotGM2) * dVdpsi;
    end
    
    fprintf(1,repmat('\b',1,length(msgStr)));
	end

	% correct functions so that they apply to the complete equipotential 
  % surfaces and not just to the sampled quadrants 

	V = 4 * V;
	S = 4 * S;
	r = ((3/(4*pi)) * V).^(1/3);
	avgG1 = 4 * avgG1 ./ S;
	avgG2 = 4 * avgG2 ./ S;
	avgG3 = 4 * avgG3 ./ S;
	avgInvG1 = 4 * avgInvG1 ./ S;
	avgInvG2 = 4 * avgInvG2 ./ S;
	avgInvG3 = 4 * avgInvG3 ./ S;
  