%
% MODEDEGREEL Return mode's l-degree for given ray params in bSys.
%
% Find for given binary system bSys, the mode's l-degree, LTO, for 
% given values of theta and omega. theta are the angles in radians, 
% at the surface, between the initial ray directions and the surface 
% normal and omega are the ray angular frequencies
%
% Setting recomp = false will load the return values from ./modeDegreeL.mat
%
% [LTO,RTO,theta,omega,Tprop] = modeDegreeL(bSys,theta,omega,Tprop,recomp)

function [LTO, RTO, theta, omega, Tprop] = ...
  modeDegreeL(bSys, theta, omega, Tprop, recomp)
	
  if ~recomp; load 'modeDegreeL.mat'; return; end;
  
	if bSys == 0; [bSys,theta,omega,Tprop] = defaultInputs();
	end

	RTO = innerTurningPoint(bSys, theta, omega, Tprop);
	a = sqrt((bSys.sst.Gamma1 .* bSys.sst.P) ./ bSys.sst.rho);
	aAtRTO = interp1(bSys.sst.R, a, RTO(:));
	aAtRTO = reshape(aAtRTO, size(RTO));
	omegaTag = ones(size(RTO,1),1) * omega;
	alpha = ((omegaTag .* RTO) ./ aAtRTO).^2;
	LTO = -1/2 + sqrt(1/4 + alpha);
  
  if recomp; save 'modeDegreeL.mat' LTO RTO theta omega Tprop;
  end
  
function [bSys, theta, omega, Tprop] = defaultInputs()

	starIndex = 1;
	starSetName = '../stellarStructure/io/SE/SE';
	addpath('../binarySystem');
	bSys = binarySystem(starSetName, starIndex);
	theta = (exp([0:0.1:log(25)])'-1) * pi/180;
	omega = [2.4:0.1:5.6] * 2*pi*1e-3;
  Tprop = 10^5;
