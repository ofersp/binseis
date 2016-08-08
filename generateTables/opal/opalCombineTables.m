%
% OPALCOMBINETABLES Combines AF and IR opacity tables.
% See README.md in this directory for further details.
function opalCombineTables()

  global binseisToolboxPath;
  opalTablesPath = [binseisToolboxPath,'data/tables.external/opal/'];
  origDir = pwd();
  fprintf(1,'Combining AF and IR tables...\n');
  cd(opalTablesPath);
  [irLogTi,irLogRi,iri,afLogTi,afLogRi,afi] = ...
    buildOpacityTables(false,true);
  combineTables(false,true,true,irLogTi,irLogRi,iri,afLogTi,afLogRi,afi);
  cleanup();
  cd(origDir);  
  fprintf(1,'Done.\n');

function cleanup()

  delete('irX70Z02.txt');
  delete('afX70Z02.txt');  
  delete('../../tables/irX70Z02.bin');
  delete('../../tables/afX70Z02.bin');  
  
function [irLogTi,irLogRi,iri,afLogTi,afLogRi,afi] = ...
  buildOpacityTables(showPlots, writeTables)

  if ~exist('irX70Z02.txt','file')
    error('Failed to find file irX70Z02.txt'); end
  if ~exist('afX70Z02.txt','file')
    error('Failed to find file afX70Z02.txt'); end
  
  ir = load('irX70Z02.txt');
  af = load('afX70Z02.txt');

  irLogT = ir(2:end,1);
  irLogR = ir(1,2:end);
  ir = ir(2:end,2:end);

  afLogT = flipud(af(2:end,1));
  afLogR = af(1,2:end);
  af = flipud(af(2:end,2:end));

  if showPlots
    figure(1); imagesc(irLogT, irLogR, ir);
    figure(2); imagesc(afLogT, afLogR, af);
    figure(3); surf(irLogR, irLogT, ir); hold on;
    figure(3); surf(afLogR, afLogT, af); hold off;
  end

  afLogRi = afLogR(1):0.25:afLogR(end);
  afLogTi = afLogT(1):0.005:afLogT(end);
  [afLogRiMG, afLogTiMG] = meshgrid(afLogRi, afLogTi);
  afi = interp2(afLogR, afLogT, af, afLogRiMG, afLogTiMG, 'linear');

  irLogRi = irLogR(1):0.25:irLogR(end);
  irLogTi = irLogT(1):0.005:irLogT(end);
  [irLogRiMG, irLogTiMG] = meshgrid(irLogRi, irLogTi);
  iri = interp2(irLogR, irLogT, ir, irLogRiMG, irLogTiMG, 'linear');

  if writeTables % save to binary files
    
    irfp = fopen('../../tables/irX70Z02.bin','wb');
    fwrite(irfp, size(iri,1), 'int');
    fwrite(irfp, size(iri,2), 'int');
    fwrite(irfp, irLogT(1), 'double');
    fwrite(irfp, irLogT(end), 'double');
    fwrite(irfp, irLogR(1), 'double');
    fwrite(irfp, irLogR(end), 'double');
    fwrite(irfp, iri, 'double');
    fclose(irfp);

    affp = fopen('../../tables/afX70Z02.bin','wb');
    fwrite(affp, size(afi,1), 'int');
    fwrite(affp, size(afi,2), 'int');
    fwrite(affp, afLogT(1), 'double');
    fwrite(affp, afLogT(end), 'double');
    fwrite(affp, afLogR(1), 'double');
    fwrite(affp, afLogR(end), 'double');
    fwrite(affp, afi, 'double');
    fclose(affp);
  end
  
  if showPlots
    figure(4); imagesc(irLogTi, irLogRi, iri);
    figure(5); imagesc(afLogTi, afLogRi, afi);
    figure(6); surf(irLogRi, irLogTi, iri); hold on;
    figure(6); surf(afLogRi, afLogTi, afi); hold off;
  end

%
% [combLogT,combLogR,combLogKappa] = ...
%   combineTables(showPlots,writeTables, ...
%     irLogTi,irLogRi,iri,afLogTi,afLogRi,afi)

function [combLogT,combLogR,combLogKappa] = ...
  combineTables(showPlots,writeTables,interpOverlap, ...
    irLogTi,irLogRi,iri,afLogTi,afLogRi,afi)

  av = irLogTi(1);
  bv = 3.95;

  aiaf = find(afLogTi == av);
  aiir = find(irLogTi == av);
  biaf = find(afLogTi == bv);
  biir = find(irLogTi == bv);

  combLogR = irLogRi;
  combLogT = [afLogTi(1:aiaf), irLogTi(aiir+1:end)];
  combLogKappa = zeros(numel(combLogT), numel(combLogR));
  
  if ~interpOverlap    
    combLogKappa(1:aiaf,:) = afi(1:aiaf,:);
    combLogKappa(aiaf+1:end,:) = iri(aiir+1:end,:);
  else    
    combLogKappa(1:aiaf,:) = afi(1:aiaf,:);
    combLogKappa(aiaf+1:end,:) = iri(aiir+1:end,:);

    step = pi / (biaf - aiaf);
    K = (1 + sin(-pi/2:step:pi/2)') / 2;
    K = K * ones(1,numel(combLogR));

    combLogKappa(aiaf:biaf,:) = ...
      afi(aiaf:biaf,:) .* (1-K) + ...
      iri(aiir:biir,:) .* (K);
  end
  
  if writeTables
    fp = fopen('../../tables/opalX70Z02.bin','wb');
    fwrite(fp, size(combLogKappa,1), 'int');
    fwrite(fp, size(combLogKappa,2), 'int');
    fwrite(fp, combLogT(1), 'double');
    fwrite(fp, combLogT(end), 'double');
    fwrite(fp, combLogR(1), 'double');
    fwrite(fp, combLogR(end), 'double');
    fwrite(fp, combLogKappa, 'double');
    fclose(fp);
  end

  if showPlots
    figure(7); surf(combLogKappa);
  end
