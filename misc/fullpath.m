%
% FULLPATH Return the full-path associated with given path.
%
% fpath = fullpath(path). returned fpath is either the full-path of path or
% [] when path does not exist.

function fpath = fullpath(path)
  
  d = dir(path);
  if numel(d) == 0; fpath = []; return; end
  path0 = pwd(); cd(path);
  fpath = pwd(); cd(path0);    
