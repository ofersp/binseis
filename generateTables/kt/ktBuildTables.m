%
% KTBUILDTABLES Generates the Kippenhahn-Thomas tables data/tables/kt.bin
% See README.md in this directory for further details.
function ktBuildTables()

  global binseisToolboxPath;
  tablesPath = [binseisToolboxPath,'data/tables'];
  origDir = pwd();
  
  fprintf(1, 'Building KT tables...\n');
  cd(tablesPath);
  ktBuildTablesInternal();
  cd(origDir);
  fprintf(1,'Done.\n');
  
function ktBuildTablesInternal()

  [psi,V,S,r,avgG1,avgG2,avgG3,avgInvG1,avgInvG2,avgInvG3] = ktFunctions();  
  [eir,maxR,eiu,eiv,eiz,eiw,eifp,eift,eipsi] = ...
    ktDerived(psi,r,S,avgG1,avgG2,avgG3,avgInvG1,avgInvG2,avgInvG3,false);
  
  % output both a mat file (used by this toolbox) and a bin file (used by
  % stellarStructure).
  
  save kt.mat eir maxR eiu eiv eiz eiw eifp eift eipsi;
  
  fp = fopen('kt.bin','wb');
  fwrite(fp, numel(eir), 'int');
  fwrite(fp, eir(1), 'double');
  fwrite(fp, eir(end), 'double');
  fwrite(fp, maxR, 'double'); % note that in ktTables.cpp, maxR is called midR
  fwrite(fp, eiu, 'double');
  fwrite(fp, eiv, 'double');
  fwrite(fp, eiz, 'double');
  fwrite(fp, eiw, 'double');
  fwrite(fp, eifp, 'double');
  fwrite(fp, eift, 'double');
  fclose(fp);
  