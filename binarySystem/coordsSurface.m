%
% COORDSSURFACE Sample the coords of an equi-valued surface in given bSys.
%
% Sample the coordinates defined by Fr == Fsurface sampling at N angular points.
% [sx,sy,sz] = coordsSurface(bSys, Fr, Fsurface, N)

function [sx,sy,sz] = coordsSurface(bSys, Fr, Fsurface, N)

  [sz,sy,sx] = sphere(N);
  
  R = bSys.sst.R(end);
  step = R * 1e-4; % resolution for solving Fr == Fsurface
  distUpper = 1.4; % upper bound for binary radial distortion factor
  distLower = 0.8;

  for i=1:numel(sx)

    nHat = [sx(i) sy(i) sz(i)];
    nHat = nHat / norm(nHat);
    
    r0 = bSys.r1 + nHat * (distLower * R);
    r1 = bSys.r1 + nHat * (distUpper * R);
		[rx,ry,rz] = coordsRay(r0, r1, step);    
    Fray = sampleStructure(bSys, Fr, rx, ry, rz);

    [dummy, minI] = min(abs(Fray - Fsurface));
    
    p = nHat * ...
      (distLower + (distUpper - distLower) * (minI / numel(Fray))) * R;
    
    sx(i) = p(1);
    sy(i) = p(2);
    sz(i) = p(3);  
  end  
