#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "TH1.h"
#include "TF1.h"
#include <TCanvas.h>
#include <TRandom3.h>  
#include <TMinuit.h>
#include <TFile.h>
#include "TFile.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TMath.h"
#include "TMatrixDSym.h"
#include "TFitResult.h"
#include "TString.h"
#include <fstream>
#include <string>
#include <cmath>

//Converts an int to a string, not really useful until now
TString ToString(int num){
	ostringstream start;
	start << num;
	TString start1 = start.str();
	return start1;	
}

//Takes a pointer to an histogram and fills it with the hexadecimal data from a file 
void Hist_creator(const std::string f_name, TH1F * hist_pt){
	
		int	tvalue = 0;
	
		std::ifstream myfile;
		myfile.open(f_name);
		if ( (myfile.rdstate() & std::ifstream::failbit ) != 0 )
		std::cerr << "Error opening 'test.txt'\n";

		while(!myfile.eof()){       
			myfile >> std::hex >> tvalue; //convert from hex to dec 
			hist_pt -> Fill(tvalue);   
		};    
	myfile.close();
	}

//Given a pointer to a histogram, a fitting function with a Gaussian plus something and a fitting interval returns the value of the mean
//of the fitted Gaussian and its statistical error in the variables pointed by the last 2 arguments.
void Peak_finder(TH1F * hist_pt, TF1 * fit_func, int xmin, int xmax, double * peak, double * peak_err){
	
	hist_pt -> Fit(fit_func, "L0", "R", xmin, xmax); //Fit in range with maximum likelihood without drawing
	*peak = fit_func -> GetParameter(1); //Get the mean of the Gaussian
	*peak_err =  fit_func -> GetParError(1); //Get the statistical error on the mean of the Gaussian
	}

int main(){
	
	float		low_b=-0.5; //Minimum value of the first bin
	float		high_b= pow(2.0,13.0)+0.5; //Maximum value of the last bin
	unsigned	bin_size= pow(2.0,0.0); //Size of the bin, change the second number to scale the size
	
	string	F_names[5] = {"dati/energy_calib/3L1_calib_source_Co.log", "dati/energy_calib/3L1_calib_source_Co.log", 
						  "dati/energy_calib/3L4_calib_source_Na.log", "dati/energy_calib/3L5_calib_source_Ce.log",
						  "dati/energy_calib/3L6_calib_source_Am.log"}; //Files with raw data from different sources
	const TString Elements[5] = {"Co1", "Co2", "Na", "Ce", "Am"};					
	int xmin[5] = {5200, 5900, 2200, 2900, 240}; //Minima of fitting regions
	int xmax[5] = {5900, 6600, 2700, 3500, 350}; //Maxima of fitting regions
	double peaks_0[5] = {0.}; //The last number indicates the order of the added polynomial in the fits
	double peaks_err_0[5] = {0.};
	double peaks_1[5] = {0.};
	double peaks_err_1[5] = {0.};
	double peaks_2[5] = {0.};
	double peaks_err_2[5] = {0.};
	double real_e[5] = {1.1732, 1.3325, 0.51100, 0.6617, 0.0595409}; //Known values for the energies of the peaks
	double real_e_err[5] = {0.};
	double constant_init[5] = {60, 50, 80, 150, 500}; //Inizializing parameters to make the fit converge
	double mean_init[5] = {5500, 6200, 2450, 3200, 270};
	double sigma_init[5] = {150, 130, 100, 100, 20}; 
	TH1F * histos[5];	//Array of histograms' pointers to save the data
	TF1 * fit_func_0[5];
	TF1 * fit_func_1[5];
	TF1 * fit_func_2[5];
	double par0 = 0;
	double par1 = 0;
	double par2 = 0;
	
	   TCanvas *c1 = new TCanvas("c1","",1);
	   TCanvas *c2 = new TCanvas("c2","",1);
	   c1 -> cd();  // In the first canvas I plot the different spectra of the sources with the fits to the peaks
	
	for (int i = 0; i < 5; i++){
		
		//Create and fill the histograms
		histos[i] = new TH1F(Elements[i], Elements[i], int ((high_b-low_b-1.0)/bin_size), low_b, high_b);
		Hist_creator(F_names[i], histos[i]);
		
		//Create and initialize the fitting functions
		fit_func_0[i] = new TF1("fit_func_0_"+Elements[i], "[Constant]*exp(-0.5*((x-[Mean])/[Sigma])*((x-[Mean])/[Sigma]))",
								xmin[i], xmax[i]); 
		fit_func_1[i] = new TF1("fit_func_1_"+Elements[i], "[Constant]*exp(-0.5*((x-[Mean])/[Sigma])*((x-[Mean])/[Sigma]))+[m]*x+[q]",
								xmin[i], xmax[i]); 
		fit_func_2[i] = new TF1("fit_func_2_"+Elements[i], "[Constant]*exp(-0.5*((x-[Mean])/[Sigma])*((x-[Mean])/[Sigma]))+[a]*x*x+[m]*x+[q]",
								xmin[i], xmax[i]); 
								
		fit_func_0[i] -> SetParameter(0,constant_init[i]);
		fit_func_0[i] -> SetParameter(1,mean_init[i]);
		fit_func_0[i] -> SetParameter(2,sigma_init[i]);
		fit_func_1[i] -> SetParameter(0,constant_init[i]);
		fit_func_1[i] -> SetParameter(1,mean_init[i]);
		fit_func_1[i] -> SetParameter(2,sigma_init[i]);
		fit_func_2[i] -> SetParameter(0,constant_init[i]);
		fit_func_2[i] -> SetParameter(1,mean_init[i]);
		fit_func_2[i] -> SetParameter(2,sigma_init[i]);
		
		//if (i == 5){										//Try to better fit Americium
			//fit_func_1[i] -> SetParameter(3, -1e-2);
			//fit_func_2[i] -> SetParameter(4, -1e-2);
		//}
		
		//Actually fit the peaks
		Peak_finder(histos[i], fit_func_0[i], xmin[i], xmax[i], &peaks_0[i], &peaks_err_0[i]);
		Peak_finder(histos[i], fit_func_1[i], xmin[i], xmax[i], &peaks_1[i], &peaks_err_1[i]);
		Peak_finder(histos[i], fit_func_2[i], xmin[i], xmax[i], &peaks_2[i], &peaks_err_2[i]);
		
		//Print the results
		cout << "Peak of " << Elements[i] << " at " << peaks_0[i] << " (Order 0)" << endl;
		cout << "Peak of " << Elements[i] << " at " << peaks_1[i] << " (Order 1)" << endl;
		cout << "Peak of " << Elements[i] << " at " << peaks_2[i] << " (Order 2)" << endl;
		
		//Some graphical stuff
		fit_func_2[i] -> SetLineColor(1);
		histos[i] -> SetLineColor(i+1);
		histos[i] -> GetXaxis() -> SetTitle("Canale");
		histos[i] -> GetYaxis() -> SetTitle("Eventi");
		histos[i] -> GetYaxis() -> SetRangeUser(0,500);
		histos[i] -> SetStats(0);
		if (i==0) ;
		else if (i==1){
			histos[i] -> Draw();
			fit_func_2[0] -> Draw("same");
		}
		else histos[i] -> Draw("same");
		fit_func_2[i] -> Draw("same");
		}
		
		//Create and fit the calibration curve
		TGraphErrors* gr1 = new TGraphErrors(5, real_e, peaks_1, real_e_err, peaks_err_1);
		gr1->Fit("pol2","L");
		TF1 * my_fit_func = gr1 -> GetFunction("pol2");
		
		//Get the results
		par0 = my_fit_func->GetParameter(0);
		par1 = my_fit_func->GetParameter(1);
		par2 = my_fit_func->GetParameter(2);
		cout << "a = " << par2 << " b = " << par1 << " ; c = " << par0 << endl;
		
		//Draw the results
		c2 -> cd();
		my_fit_func -> Draw();
		gr1 -> Draw("sameAP");
	
	return 0;
	}
