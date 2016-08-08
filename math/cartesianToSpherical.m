%
% CARTESIANTOSPHERICAL Transform cartesian coord arrays to spherical ones
%
% [r,theta,phi] = cartesianToSpherical(x,y,z)

function [r,theta,phi] = cartesianToSpherical(x,y,z)

  rho = sqrt(x.^2 + y.^2);
  r = sqrt(x.^2 + y.^2 + z.^2);
  phi = atan2(y,x);
  theta = atan2(rho,z);
