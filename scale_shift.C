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
void Hist_creator(TString f_name, TH1F * hist_pt){
	
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
	
	//int binmax = hist_pt->GetMaximumBin(); 
	//double x = hist_pt->GetXaxis()->GetBinCenter(binmax);
	//*peak = x;
	//*peak_err = 8;
	hist_pt -> Fit(fit_func, "L0", "R", xmin, xmax); //Fit in range with maximum likelihood without drawing
	*peak = fit_func -> GetParameter(1); //Get the mean of the Gaussian
	*peak_err =  fit_func -> GetParError(1); //Get the statistical error on the mean of the Gaussian
	}

int main(){
	
	float		low_b=-0.5; //Minimum value of the first bin
	float		high_b= pow(2.0,13.0)+0.5; //Maximum value of the last bin
	unsigned	bin_size= pow(2.0,0.0); //Size of the bin, change the second number to scale the size
	
	int xmin = 3000;
	int xmax = 3400;
	
	//double peaks[36] = {0};
	//double peaks_err[36] = {0}; 
	//double time_i[36] = {0, 46, 107, 160, 213, 264, 315, 364, 411, 464, 514, 567, 618, 666, 717, 770, 822, 869, 925, 979, 1029, 1080, 1133,
						 //1187, 1240, 1297, 1347, 1396, 1443, 1500, 1550, 1601, 1651, 1697, 1745, 1793};
	//double time_err[36] = {0};
	//TString first_part = "dati/scale_shift/3M";
	//TString second_part = "_termocal_1s_Cs_90d.log";
	//TString file_number;
	//TString file_name;
	//TString func_name[36];
	//TString func_name_init = {"fit_func_"};
	
	//TH1F * histos[36];
	//TF1 * fit_func[36];
	
	double peaks[26] = {0};
	double peaks_err[26] = {0}; 
	double time_i[26] = {0, 47, 89, 131, 174, 220, 265, 307, 350, 400, 444, 490, 540, 600, 1000, 1059, 1115, 1168, 1225, 1280, 1336, 1391,
						 1449, 1500, 1557, 1605};
	double time_err[26] = {0};
	TString first_part = "dati/scale_shift_1/3G";
	TString second_part = "_termocal_1s_Cs_90d.log";
	TString file_number;
	TString file_name;
	TString func_name[26];
	TString func_name_init = {"fit_func_"};
	
	TH1F * histos[26];
	TF1 * fit_func[26];
	
	//double peaks[13] = {0};
	//double peaks_err[13] = {0}; 
	//double time_i[13] = {0, 59, 104, 144, 187, 222, 262, 298, 337, 374, 410, 455, 490};
	//double time_err[13] = {0};
	//TString first_part = "dati/scale_shift_2/3G";
	//TString second_part = "_termocal_1s_Co_90d.log";
	//TString file_number;
	//TString file_name;
	//TString func_name[13];
	//TString func_name_init = {"fit_func_"};
	
	//TH1F * histos[13];
	//TF1 * fit_func[13];
	
	//xmin = 5300;
	//xmax = 6200;
	
	for (int i = 0; i < 26; i++){
		file_number = ToString(i+1);
		file_name = first_part + file_number + second_part;
		cout << file_name << endl;
		histos[i] = new TH1F(file_number, file_number, int ((high_b-low_b)/bin_size), low_b, high_b);
		Hist_creator(file_name, histos[i]);
		func_name[i] = func_name_init + file_number;
		fit_func[i] = new TF1(func_name[i], "[Constant]*exp(-0.5*((x-[Mean])/[Sigma])*((x-[Mean])/[Sigma]))",
								xmin, xmax); 
		fit_func[i] -> SetParameter(0,400);
		fit_func[i] -> SetParameter(1,3200);
		fit_func[i] -> SetParameter(2,100);						
		Peak_finder(histos[i], fit_func[i], xmin, xmax, &peaks[i], &peaks_err[i]);
	}
	
	TGraphErrors* gr1 = new TGraphErrors(26, time_i, peaks, time_err, peaks_err);
	gr1 -> Draw();
	
	return 0;
}
