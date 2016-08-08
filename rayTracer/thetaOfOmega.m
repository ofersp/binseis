%
% THETAOFOMEGA
%
% Given LTO, theta and omega computed by modeDegreeL(), returns the 
% functional dependence theta(omega) for a given value of the l-degree 
% lUpperBound
%
% [t,o] = thetaOfOmega(LTO, theta, omega, lUpperBound)

function [t,o] = thetaOfOmega(LTO, theta, omega, lUpperBound)

  nTheta = 1000; 
  nOmega = 5000;   
  dTheta = (theta(end) - theta(1)) / (nTheta - 1);
  dOmega = (omega(end) - omega(1)) / (nOmega - 1);  
  thetaI = (theta(1):dTheta:theta(end))';
  omegaI = (omega(1):dOmega:omega(end));

  T = theta*ones(1,numel(omega));
  O = ones(numel(theta),1)*omega;    
  
  TI = thetaI*ones(1,numel(omegaI));
  OI = ones(numel(thetaI),1)*omegaI;
  
  LTOI = interp2(O, T, LTO, OI, TI, 'spline');
  LTOIB = LTOI < lUpperBound;
  
  n = 1;
  for j=1:80:nOmega    
    t(n) = thetaI(find(LTOIB(:,j), 1, 'last'));
    o(n) = omegaI(j); 
    n = n + 1;
  end
