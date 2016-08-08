%
% [logT6,logP12,logRho,dE12dT6v,Gamma1,Gamma2,ChiRho,ChiT] = ...
%   buildEosTable(showPlots, writeTables)

function [logT6,logP12,logRho,dE12dT6v,Gamma1,Gamma2,ChiRho,ChiT] = ...
  buildEosTable(showPlots, writeTables)

  numLogT6 = 1000;
  numLogP12 = 1000;

  EOS = load('EOS_2005/eosX70Z02.txt');
  EOS = reshape(EOS, numLogT6, numLogP12, 8);

  logT6  = squeeze(EOS(:,1,1));
  logP12 = squeeze(EOS(1,:,2));

  logRho = squeeze(EOS(:,:,3));
  dE12dT6v = squeeze(EOS(:,:,4));
  Gamma1 = squeeze(EOS(:,:,5));
  Gamma2 = squeeze(EOS(:,:,6));
  ChiRho = squeeze(EOS(:,:,7));
  ChiT = squeeze(EOS(:,:,8));
  
  if writeTables % save to binary files
    
    fp = fopen('../eosX70Z02.bin','wb');
    fwrite(fp, numel(logT6), 'int');
    fwrite(fp, numel(logP12), 'int');
    fwrite(fp, logT6(1), 'double');
    fwrite(fp, logT6(end), 'double');
    fwrite(fp, logP12(1), 'double');
    fwrite(fp, logP12(end), 'double');

    fwrite(fp, logRho, 'float');
    fwrite(fp, dE12dT6v, 'float');
    fwrite(fp, Gamma1, 'float');
    fwrite(fp, Gamma2, 'float');
    fwrite(fp, ChiRho, 'float');
    fwrite(fp, ChiT, 'float');
    fclose(fp);
  end
  
  if showPlots
    figure(1); imagesc(logT6, logP12, logRho)
    figure(2); imagesc(logT6, logP12, log10(dE12dT6v))
    figure(3); imagesc(logT6, logP12, Gamma1)
    figure(4); imagesc(logT6, logP12, Gamma2)
    figure(5); imagesc(logT6, logP12, ChiRho)
    figure(6); imagesc(logT6, logP12, ChiT)
  end
