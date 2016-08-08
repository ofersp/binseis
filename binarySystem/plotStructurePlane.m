%
% PLOTSTRUCTUREPLANE Plot a 2d slice through a bSys scalar field.
%
% plotStructurePlane(bSys, Fr, planeDim)

function plotStructurePlane(bSys, Fr, planeDim)
  
  R = bSys.sst.R(end);
  if planeDim == 3
    C1 = bSys.r1 + R*[-1.3, -1.3, 0];
    C2 = bSys.r1 + R*[+1.3, +1.3, 0];
  elseif planeDim == 2
    C1 = bSys.r1 + R*[-1.3, 0, -1.3];
    C2 = bSys.r1 + R*[+1.3, 0, +1.3];
  elseif planeDim == 1
    C1 = bSys.r1 + R*[0, -1.3, -1.3];
    C2 = bSys.r1 + R*[0, +1.3, +1.3];
	end
	
  spaceStep = R / 300;
  [X, Y, Z] = coordsGrid(C1, C2, spaceStep);
  [x, y, z] = coordsGridAxes(C1, C2, spaceStep);
  F = squeeze(sampleStructure(bSys, Fr, X, Y, Z));

	if planeDim == 3
		imagesc(x, y, F');
		title('structure in x-y plane');
	elseif planeDim == 2
		imagesc(x, z, F');
		title('structure in x-z plane');
	elseif planeDim == 1
		imagesc(y, z, F');
		title('structure in y-z plane');
	end
	pbaspect('manual');