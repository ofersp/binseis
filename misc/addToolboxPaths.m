%
% ADDTOOLBOXPATHS Recursively add toolbox paths to the the matlab path.
%
% addToolboxPaths(base, exclude, toolboxFiles, verbose) Adds the full paths 
% of all un-excluded toolbox subdirectories of base to the matlab path.
%
% Inputs:
%   base is the base path from which toolbox directories are searched
%   exclude is a cell array of directory names which will not be followed
%   toolboxFiles is a cell array of filenames that identify a toolbox dir
%
% Defaults:
%   basePath = pwd()
%   excludeDirs = {}
%   toolboxFiles = {'Contents.m'}
%   verbose = false

function addToolboxPaths(base, exclude, toolboxFiles, verbose)

  if ~exist('base','var'); base = pwd(); end
  if ~exist('exclude','var'); exclude = {}; end
  if ~exist('toolboxFiles','var'); toolboxFiles = {'Contents.m'}; end  
  if ~exist('verbose','var'); verbose = false; end  
  fullBase = fullpath(base);
  if numel(fullBase) == 0; error('base path not found'); end;
  recursivelyAddPath(fullBase, exclude, toolboxFiles, verbose);

function recursivelyAddPath(path, exclude, toolboxFiles, verbose)

  pathAdded = false; 
  currDir = dir(path);
  if numel(currDir) < 2; return; end
  
  for i=3:numel(currDir)    
    if currDir(i).isdir
      if sum(strcmp(currDir(i).name, exclude)) == 0
        childPath = [path '/' currDir(i).name];
        if verbose; fprintf(1, 'entering %s\n', childPath); end
        recursivelyAddPath(childPath, exclude, toolboxFiles, verbose);
      end
    else
      if ~pathAdded && sum(strcmp(currDir(i).name, toolboxFiles)) > 0
        if verbose; fprintf(1, 'adding   %s\n', path); end
        addpath(path);
        pathAdded = true;
      end
    end
  end
  