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


TString ToString(int num){
	ostringstream start;
	start<<num;
	TString start1=start.str();
	return start1;	
}


void HistoDrawer(TString f_name, TH1F* hist_point){

	//cout << "Cusuuuu2" << endl;
	
	int	tvalue=0;
	

	std::ifstream myfile;
	myfile.open(f_name);
	//if ( (myfile.rdstate() & std::ifstream::failbit ) != 0 )
    //std::cerr << "Error opening 'test.txt'\n";
    
		while(!myfile.eof()){       
			myfile >> std::hex >> tvalue; 
			//cout << tvalue << endl;   
			hist_point->Fill(tvalue);      
		};    
	myfile.close();

	//Creo l'istogramma
	//TH1F *h_energia = new TH1F("h_energia","",int((high_b-low_b-1.0)/bin_size),low_b,high_b);


	//Riempo l'istogramma  

	//hist_point->Scale(1./acquisition_time);
  
	
};

void HistoDrawerRescaled(TString f_name, TH1F* hist_point, double scale){

	//cout << "Cusuuuu2" << endl;
	
	int	tvalue=0;
	

	std::ifstream myfile;
	myfile.open(f_name);
	//if ( (myfile.rdstate() & std::ifstream::failbit ) != 0 )
    //std::cerr << "Error opening 'test.txt'\n";
    
		while(!myfile.eof()){       
			myfile >> std::hex >> tvalue; 
			//cout << tvalue << endl;   
			hist_point->Fill(tvalue*scale);      
		};    
	myfile.close();
	
};

void Peak_finder(TH1F * hist_pt, TF1 * fit_func, double xmin, double xmax, double * peak, double * peak_err){
	
	//int binmax = hist_pt->GetMaximumBin(); 
	//double x = hist_pt->GetXaxis()->GetBinCenter(binmax);
	//*peak = x;
	//*peak_err = 8;
	hist_pt -> Fit(fit_func, "L0", "R", xmin, xmax); //Fit in range with maximum likelihood without drawing
	*peak = fit_func -> GetParameter(1); //Get the mean of the Gaussian
	*peak_err =  fit_func -> GetParError(1); //Get the statistical error on the mean of the Gaussian
	}

int main(){
	
	float		low_b=-0.5;
	float		high_b= pow(2.0,13.0)+0.5;
	unsigned	bin_size= pow(2.0,3.0);
	double 		scale;
	
	int xmin[9] = {2400, 5300, 2400, 5300, 2400, 5300, 2400, 5300, 2400};
	int xmax[9] = {3000, 5800, 3000, 5800, 3000, 5800, 3000, 5800, 3000};
	
	double peaks[9] = {0.};
	double peaks_err[9] = {0.};
	
	double first_peak;
	double second_peak;
	double first_peak_err;
	double second_peak_err;
	double first_me;
	double second_me;
	
	
	TString first_part = "dati/first_15/3V";
	TString cal = "_cal";
	TString acq = "_acq";
	TString Cs_string = "_1s_Cs_15d.log";
	TString Co_string = "_2s_Co_15d.log";
	TString histo_name = "histo_";
	TString filename;
	TString number_file;
	TString number_file_2;
	TString func_name_init = "func_name_";
	TString func_name; 
	
	TH1F * histos[9];
	TH1F * histo_sum;
	TF1 * fit_func[9];
	TF1 * func_first_peak;
	TF1 * func_second_peak;
	
	TCanvas *c_1 = new TCanvas("c_1","c_1",1);   
	TCanvas *c_2 = new TCanvas("c_2","c_2",1); 
	TCanvas *c_3 = new TCanvas("c_3","c_3",1); 
	
	c_1 -> cd();
	
	for (int i=0; i < 9; i++) {
		if (i%2 == 0){
			number_file = ToString(i+1);
			number_file_2 = ToString(i/2+1);
			cout << "Cusu" << endl;
			filename = first_part + number_file + cal + number_file_2 + Cs_string;
			histos[i] = new TH1F(histo_name + number_file, histo_name + number_file, int((high_b-low_b)/bin_size), low_b, high_b);
			HistoDrawer(filename, histos[i]);
			func_name = func_name_init + number_file;
			fit_func[i] = new TF1(func_name, "[Constant]*exp(-0.5*((x-[Mean])/[Sigma])*((x-[Mean])/[Sigma]))",
						  xmin[i], xmax[i]); 
			fit_func[i] -> SetParameter(0,400);
			fit_func[i] -> SetParameter(1,3200);
			fit_func[i] -> SetParameter(2,100);	
			Peak_finder( histos[i], fit_func[i], xmin[i], xmax[i], &peaks[i], &peaks_err[i]);
			cout << peaks[i] << endl;
			histos[i]->SetLineColor(i+1);
			if (i==0) histos[i]->Draw();
			else histos[i]->Draw("same");
		}
	}
	
	histo_sum = new TH1F("histo_sum", "histo_sum", int((high_b-low_b)/bin_size/4), int(low_b*0.6617/2700), int(high_b*0.6617/2700));
	
	c_2 -> cd();
	
	for (int j=0; j < 9; j++){
		if (j%2 == 1){
			scale = 2*0.6617/(peaks[j-1]+peaks[j+1]);
			number_file = ToString(j+1);
			number_file_2 = ToString(j/2+1);
			cout << "Cusumano" << endl;
			filename = first_part + number_file + acq + number_file_2 + Co_string;
			histos[j] = new TH1F(histo_name + number_file, histo_name + number_file, int((high_b-low_b)/bin_size/4),
								 int(low_b*0.6617/2700), int(high_b*0.6617/2700));
			HistoDrawerRescaled(filename, histos[j], scale);
			histos[j]->SetLineColor(j+1);
			if (j==0) histos[j]->Draw();
			else histos[j]->Draw("same");
			histo_sum -> Add(histos[j]);
		}
	}	
	
	c_3 -> cd();
	histo_sum -> Draw();
	func_first_peak = new TF1("func_first_peak", "[Constant]*exp(-0.5*((x-[Mean])/[Sigma])*((x-[Mean])/[Sigma]))+[m]*x+[q]",
						  1.02, 1.14); 
	func_first_peak -> SetParameter(0,140);
	func_first_peak -> SetParameter(1,1);
	func_first_peak -> SetParameter(2,0.1);	
	Peak_finder( histo_sum, func_first_peak, 1.02, 1.14, &first_peak, &first_peak_err);
	func_second_peak = new TF1("func_second_peak", "[Constant]*exp(-0.5*((x-[Mean])/[Sigma])*((x-[Mean])/[Sigma]))+[m]*x+[q]",
						  1.14, 1.3); 
	func_second_peak -> SetParameter(0,100);
	func_second_peak -> SetParameter(1,1.2);
	func_second_peak -> SetParameter(2,0.1);
	Peak_finder( histo_sum, func_second_peak, 1.14, 1.3, &second_peak, &second_peak_err);
	func_first_peak -> Draw("same");
	func_second_peak -> Draw("same");
	cout << "First peak at " << first_peak << " MeV" << endl;
	cout << "Second peak at " << second_peak << " MeV" << endl;   
	
	first_me = first_peak * 1.1732 * (1 - TMath::Cos(TMath::Pi()/12)) / (1.1732 - first_peak);
	second_me = second_peak * 1.3325 * (1 - TMath::Cos(TMath::Pi()/12)) / (1.3325 - second_peak);
	
	cout << "First electron mass at 15° " << first_me << " MeV" << endl;
	cout << "Second electron mass at 15° " << second_me << " MeV" << endl;
 	
	return 0;	
}
