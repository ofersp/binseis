%
% COORDSRAY Generate equi-spaced coords between two points.
%
% [X,Y,Z] = coordsRay(r0, r1, spaceStep)

function [X,Y,Z] = coordsRay(r0, r1, spaceStep)

  deltar = r1 - r0;
  l = norm(r1 - r0);
  numSamples = round(l / spaceStep);
  dr = deltar / (numSamples - 1);
  
  if dr(1)==0; X=ones(1, numSamples)*r0(1); else X=r0(1):dr(1):r1(1); end;
  if dr(2)==0; Y=ones(1, numSamples)*r0(2); else Y=r0(2):dr(2):r1(2); end;
  if dr(3)==0; Z=ones(1, numSamples)*r0(3); else Z=r0(3):dr(3):r1(3); end;
