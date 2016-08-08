%
% OPALEXTRACTTABLES Extracts opacity tables to produce data/tables/opal.bin
% See README.md in this directory for further details.
function opalExtractTables()

  global binseisToolboxPath;
  opalTablesPath = [binseisToolboxPath,'data/tables.external/opal/'];
  origDir = pwd();

  cd(opalTablesPath);
  extractSubtableAF();  
  extractSubtableIR();
  cleanup();
  cd(origDir);
  fprintf(1,'Done.\n');

function cleanup()

  delete('g7.02.tron');

function [fpinp,fpout] = fixOpalTable(fpinp,fpout,irTable)

  for i=1:4
    l = fgetl(fpinp);
    if ~ischar(l); error('Failed to parse subtable'); end
  end
  l(1:5) = 'NaN  ';
  numColumns = length(allStrToken(l,' '));
  fwrite(fpout,[l,sprintf('\n')],'char');
  if irTable
    l = fgetl(fpinp);
    if ~ischar(l); error('Failed to parse subtable'); end
  end
  
  while true
    l = fgetl(fpinp);
    if ~ischar(l); break; end;
    if isempty(l); break; end;
    t = allStrToken(l,' ');
    if length(t) > numColumns
      error('Failed to parse subtable'); end
    fwrite(fpout,l,'char');    
    if length(t) < numColumns
      for i=1:(numColumns-length(t))
        fwrite(fpout,'    NaN','char');end      
    end
    fwrite(fpout,sprintf('\n'),'char');
  end

function extractSubtableAF()

  fprintf(1,'Extracting AF table...\n');
  if ~exist('f05.g93.tar.gz','file')
    error('Failed to find file f05.g93.tar.gz'); end
  system('tar xvzf f05.g93.tar.gz g7.02.tron');
  fprintf(1,'\b\b\b\b\b\b\b\b\b\b\b');
  
  if ~exist('f05.g93.tar.gz','file')
    error('Failed to find X=70, Z=0.02 table in f05.g93.tar.gz'); end

  fpinp = fopen('g7.02.tron','r');
  fpout = fopen('afX70Z02.txt','w');
  [fpinp,fpout] = fixOpalTable(fpinp, fpout, false);
  fclose(fpinp);
  fclose(fpout);

function extractSubtableIR()

  subTableStr = 'TABLE # 73     $G&N''93';
  fprintf(1,'Extracting IR table...\n');  
  if ~exist('GN93hz','file')
    error('Failed to find file GN93hz'); end
  
  fpinp = fopen('GN93hz','r');
  fpout = fopen('irX70Z02.txt','w');
  [fpinp,subTableFound] = findSubtableIR(fpinp,subTableStr);
  if ~subTableFound
    error('Failed to find X=70, Z=0.02 table in GN93hz'); end   
  [fpinp,fpout] = fixOpalTable(fpinp, fpout, true);  
  fclose(fpinp);
  fclose(fpout);
  
function [fpinp,subTableFound] = findSubtableIR(fpinp,subTableStr)
  
  subTableFound = false;
  while ~subTableFound
    l = fgetl(fpinp);
    if ~ischar(l); break; end;
    if length(l) >= length(subTableStr)
      if strcmp(l(1:length(subTableStr)),subTableStr)
        subTableFound = true; end
    end
  end
  