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

int main(){
	
	float		low_b=-0.5;
	float		high_b= pow(2.0,13.0)+0.5;
	unsigned	bin_size= pow(2.0,0.0);
		
	int	tvalue = 0;
	
	TH1F * hist_pt = new TH1F("hist_pt", "hist_pt", int ((high_b-low_b)/bin_size), low_b, high_b);
	std::ifstream myfile;
	myfile.open("dati/3G41_angcal_1s_Co_0d.log");
	if ( (myfile.rdstate() & std::ifstream::failbit ) != 0 )
	std::cerr << "Error opening 'test.txt'\n";

	while(!myfile.eof()){       
		myfile >> std::hex >> tvalue; //convert from hex to dec 
		hist_pt -> Fill(tvalue);   
	};    
	myfile.close();
	
	//TF1 * fit_func_1 = new TF1("fit_func_1", "gaus", 3000, 4000);
	//hist_pt -> Fit(fit_func_1, "L0", "");
	
	hist_pt -> Draw();
	
	return 0;
	}
