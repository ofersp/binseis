%
% KTDERIVED Compute secondary Kippenhahn-Thomas functions.
%
% Compute the dimensionless u,v,w,fp,ft Kippenhahn-Thomas functions derived 
% from V,S,r,avgG,avgInvG. Correct the resolution error at low radii, 
% and cut-off error at large radii where the equipotentials reach 
% the L1 point.
%
% [eir, maxR, eiu, eiv, eiz, eiw, eifp, eift, eipsi] = ...
%   ktDerived(psi,r,S,avgG1,avgG2,avgG3,avgInvG1,avgInvG2,avgInvG3),...
%     showPlots,writeMat)

function [eir, maxR, eiu, eiv, eiz, eiw, eifp, eift, eipsi] = ...
	ktDerived(psi,r,S,avgG1,avgG2,avgG3,avgInvG1,avgInvG2,avgInvG3,showPlots)

	alpha = 0.99;
  lowR1 = 0.4;
  lowR2 = 0.3;  
  smoothingLength1 = 0.1;
  smoothingLength2 = 0.15;
  maxPsi = -2.0;
  maxR = interp1(psi, r, maxPsi, 'linear') * alpha;

  % extrapolate S,avgG1,avgG2,avgG3,avgInvG1,avgInvG2,avgInvG3 at low
  % values of r assuming a spherically symmetric potential
  
 	innerKernel1 = (1 + erf((lowR1 - r) / smoothingLength1)) / 2;
	outerKernel1 = (1 + erf((r - lowR1) / smoothingLength1)) / 2;
 	innerKernel2 = (1 + erf((lowR2 - r) / smoothingLength2)) / 2;
	outerKernel2 = (1 + erf((r - lowR2) / smoothingLength2)) / 2;
  
  innerS = 4*pi*r.^2;
  innerG1 = 1./r.^2;
  innerG2 = 1./r.^4;
  innerG3 = 1./r.^6;
  innerInvG1 = r.^2;
  innerInvG2 = r.^4;  
  innerInvG3 = r.^6;
  
  S = innerS.*innerKernel1 + S.*outerKernel1;
  avgG1 = innerG1.*innerKernel2 + avgG1.*outerKernel2;
  avgG2 = innerG2.*innerKernel2 + avgG2.*outerKernel2;
  avgG3 = innerG3.*innerKernel2 + avgG3.*outerKernel2;
  avgInvG1 = innerInvG1.*innerKernel2 + avgInvG1.*outerKernel2;  
  avgInvG2 = innerInvG2.*innerKernel2 + avgInvG2.*outerKernel2;  
  avgInvG3 = innerInvG3.*innerKernel2 + avgInvG3.*outerKernel2;
  
  % compute derived functions u,v,w,fp,ft on an evenly spaced radial mesh
  % right up to the critical equipotential maxPsi at maxR

  u =  [1, S ./ (4*pi*r.^2)];
  v =  [1, avgG1 .* r.^2];
  z =  [1, avgG2 .* r.^4];
  w =  [1, avgInvG1 ./ r.^2];
  fp = 1./(u.*w);
  ft = 1./(u.*u.*v.*w);
	r = [0,r];
	psi = [NaN,psi];
  
	ir = r(1):maxR/150:maxR;
	iu = interp1(r, u, ir, 'linear');
	iv = interp1(r, v, ir, 'linear');
	iz = interp1(r, z, ir, 'linear');
  iw = interp1(r, w, ir, 'linear');
	ifp = interp1(r, fp, ir, 'linear');
  ift = interp1(r, ft, ir, 'linear');
  warning('OFF','MATLAB:interp1:NaNinY');
	ipsi = interp1(r, psi, ir, 'linear');
  warning('ON','MATLAB:interp1:NaNinY');
  
  % linearly extrapolate all output functions above maxR
  
  eir = [ir(1:end-1), ir(end):ir(2)-ir(1):ir(end)*1.33];  
  eiu = linExtrap(iu, ir, eir);
  eiv = linExtrap(iv, ir, eir);
  eiz = linExtrap(iz, ir, eir);
  eiw = linExtrap(iw, ir, eir);
  eifp = linExtrap(ifp, ir, eir);  
  eift = linExtrap(ift, ir, eir);
  eipsi = linExtrap(ipsi, ir, eir);

  % fix maxR back to it's original value
  
  maxR = maxR / alpha;
  
  % possibly display results
  if showPlots
    figure(1);
    plot(eir,eifp,'.-b'); hold on; 
    plot(eir,eift,'.-r'); hold off;
    figure(2);
    plot(eir,eiu,'.-b'); hold on; 
    plot(eir,eiv,'.-r');
    plot(eir,eiz,'.-g');
    plot(eir,eiw,'.-k'); hold off;
    figure(3);
    plot(eir,eipsi,'.-b');
  end
  
function [ef] = linExtrap(f,x,ex)

  slope = (f(end) - f(end-1)) / (x(end) - x(end-1));
  ef = [f(1:end-1), f(end) + (ex(numel(x):end) - ex(numel(x)))*slope];
  
