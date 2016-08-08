#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

// the following constants are in the MKS unit system

const long double pi = 3.14159265358979323846264338327950;
const long double two_pi = 2*pi;
const long double four_pi = 4*pi;
const long double four_pi_o3 = four_pi/3;

const float G = 6.673e-11F;
const double c = 2.99792458e08;
    
const double h = 6.62606876e-34;
const double k_B = 1.3806503e-23;
const double sigma = 2*pow(pi,5)*pow(k_B,4)/(15*c*c*pow(h,3));
const double a_rad = 4*sigma/c;
const double a_rad_o3 = a_rad/3;
    
const double m_H = 1.673532499e-27;

const double AU	= 1.4959787066e11;
const float M_Sun	= 1.9891e30F;
const float S_Sun	= 1.365e3F;
const float L_Sun	= four_pi*AU*AU*S_Sun;
const float R_Sun = 6.95508e8F;

// the following constants are in the CGS unit system 

const double m_H_cgs = 1.673532499e-24;
const double M_Sun_cgs = 1.9891e+33;

// the following constants are in the {Rsun, Msun, sec} unit system 

const double G_sol = 3.94522115e-07;
const double k_B_sol = 1.4349038956073e-71;

#endif
