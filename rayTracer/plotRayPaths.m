%
% PLOTRAYPATHS Plot path of rays in bSys superimposed on some scalar field.
%
% plotRayPaths(bSys,Fr,rayPaths,projectOntoDim,plotEndPoints,plotSurfacePoints)

function plotRayPaths(bSys, Fr, rayPaths, projectOntoDim, ...
	plotEndPoints, plotSurfacePoints)

  plotStructurePlane(bSys, Fr, projectOntoDim);
	hold on;
  
	if projectOntoDim == 3
		title('path projected onto x-y plane');
	elseif projectOntoDim == 2
		title('path projected onto x-z plane');
	elseif projectOntoDim == 1
		title('path projected onto y-z plane');
  end  

  for i=1:numel(rayPaths)

    if plotEndPoints
      simI = rayPaths{i}.simSamples;
      pStyle = '.k';
    elseif plotSurfacePoints
      simI = find(rayPaths{i}.atSurface > 0);
      pStyle = '.k';
    else
      simI = 1:rayPaths{i}.simSamples;
      pStyle = '-k';
    end
    
    if projectOntoDim == 3
      plot(rayPaths{i}.r(simI,1), rayPaths{i}.r(simI,2), pStyle);
		elseif projectOntoDim == 2
      plot(rayPaths{i}.r(simI,1), rayPaths{i}.r(simI,3), pStyle);
		elseif projectOntoDim == 1
      plot(rayPaths{i}.r(simI,2), rayPaths{i}.r(simI,3), pStyle);			
    end
	end
	hold off;	

