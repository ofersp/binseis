%
% initBinseisToolbox Initialize the binseis toolbox paths

clear all;
global binseisToolboxPath; binseisToolboxPath = [pwd(),'/'];
exclude = {'resources','backup','data'};
addpath misc;
addToolboxPaths('./', exclude, 'Contents.m', false);
rmpath misc;
clear;