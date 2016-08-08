%
% CONSTANTS Sets up global MKS physical constants.

global G; G = 6.673e-11;
global c; c = 2.99792458e08;

global h; h = 6.62606876e-34;
global k_B; k_B = 1.3806503e-23;
global m_H; m_H = 1.673532499e-27;
global sigma; sigma = (2*pi^5*k_B^4)/(15*c^2*h^3);

global AU; AU = 1.4959787066e11;
global S_Sun; S_Sun = 1.365e3;
global L_Sun; L_Sun = 4*pi*AU*AU*S_Sun;
global M_Sun; M_Sun = 1.9891e30;
global R_Sun; R_Sun = 6.95508e8;
