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

//using namespace std;


TString ToString(int num){
	ostringstream start;
	start<<num;
	TString start1=start.str();
	return start1;	
}


void HistoDrawer(const std::string f_name, float acquisition_time, TH1F* hist_point){

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

	hist_point->Scale(1000./acquisition_time);
  
	
};

//Given a pointer to a histogram, a fitting function with a Gaussian plus something and a fitting interval returns the value of the mean
//of the fitted Gaussian and its statistical error in the variables pointed by the last 2 arguments.
void Peak_finder(TH1F * hist_pt, TF1 * fit_func, int xmin, int xmax, double * peak, double * peak_value, unsigned bin_size){
	
	int hist_bin = 0;
	//int binmax = hist_pt->GetMaximumBin(); 
	//double x = hist_pt->GetXaxis()->GetBinCenter(binmax);
	//*peak = x;
	//*peak_err = 8;
	hist_pt -> Fit(fit_func, "L0", "R", xmin, xmax); //Fit in range with maximum likelihood without drawing
	*peak = fit_func -> GetParameter(1); //Get the mean of the Gaussian
	hist_bin = hist_pt -> GetXaxis() -> GetBinCenter(*peak);
	cout << hist_bin << endl;
	*peak_value = hist_pt -> GetBinContent(hist_bin/bin_size/bin_size);
	cout << *peak_value << endl;
	}

	





int main() {

//Definisco le variabili

	float		low_b =-0.5;
	float		high_b = pow(2.0,13.0)+0.5;
	unsigned	bin_size = pow(2.0,2.0);
	
	int xmin[11] = {5400, 5300, 5200, 5000, 5000, 5000, 5300, 5200, 5000, 5000, 5000};
	int xmax[11] = {5800, 5700, 5600, 5600, 5600, 5600, 5700, 5600, 5600, 5600, 5600};


float	times[11] = {183223, 180635, 180462, 180585, 180453, 180427, 180495, 180464, 180455, 180486, 180391};
double	theta[11] = {0, 2, 4, 6, 8, 10, -2, -4, -6, -8, -10};
double  theta_err[11] = {0.};
string	F_names[11]={"dati/3G41_angcal_1s_Co_0d.log", "dati/3G42_angcal_1s_Co_2d.log", "dati/3G43_angcal_1s_Co_4d.log",
					 "dati/3G44_angcal_1s_Co_6d.log", "dati/3G45_angcal_1s_Co_8d.log", "dati/3G46_angcal_1s_Co_10d.log",
					 "dati/3G47_angcal_1s_Co_m2d.log", "dati/3G48_angcal_1s_Co_m4d.log", "dati/3G49_angcal_1s_Co_m6d.log",
					 "dati/3G50_angcal_1s_Co_m8d.log", "dati/3G51_angcal_1s_Co_m10d.log"};
TH1F*	histarray[11];
double	peaks[11];
double	peaks_value[11];
double	peaks_value_err[11];
TF1 * 	fit_func[11];

//float	times[4]={210434,211795,212251,210548};
//int		theta[4]={10,20,-10,-20};
//string	F_names[4]={"dati/1V4_Angular_Dispersion_10d_CO.log","dati/1V5_Angular_Dispersion_20d_CO.log","dati/1V10_Angular_Dispersion_m10d_CO.log",
				 //"dati/1V11_Angular_Dispersion_m20d_CO.log"};
//TH1F*	histarray[4];

//Apro il Canvas

   
   TCanvas *c_angl = new TCanvas("c_angl","Distribuzione in impulso a vari angoli",1);   
   TCanvas *c_angl_dist = new TCanvas("c_angl_dist","Distribuzione di eventi angolare",1); 
   
   c_angl->cd();

int k=0;
for(k=0;k<11;k++){
	TString numstr=ToString(theta[k]);
	TString histoname="histo_theta_"+numstr;
	histarray[k] = new TH1F(histoname, histoname, int((high_b-low_b)/bin_size), low_b, high_b);
	//cout<< "Cusuuuuu1"<<endl;
	//cout<< F_names[k] << " " << times[k] << " " << histarray[k] << endl;
	HistoDrawer(F_names[k], times[k], histarray[k]);
	//cout<< "Cusuuuuu3"<<endl;
	histarray[k]->SetLineColor(k+1);
	histarray[k]->GetXaxis()->SetTitle("Canale");
	histarray[k]->GetYaxis()->SetTitle("Eventi");
	histarray[k]->SetStats(0);
	if (k==0) histarray[k]->Draw();
	else histarray[k]->Draw("same");
	TString func_name="func_theta_"+numstr;
	fit_func[k] = new TF1(func_name, "[Constant]*exp(-0.5*((x-[Mean])/[Sigma])*((x-[Mean])/[Sigma]))",
						  xmin[k], xmax[k]); 
	if ((k == 4) || (k == 5) || (k == 9) || (k ==10))	fit_func[k] -> SetParameter(0,1);
	else fit_func[k] -> SetParameter(0,12);
	fit_func[k] -> SetParameter(1,5400);
	fit_func[k] -> SetParameter(2,100);						
	Peak_finder(histarray[k], fit_func[k], xmin[k], xmax[k], &peaks[k], &peaks_value[k], bin_size); 
	peaks_value_err[k] = TMath::Sqrt(peaks_value[k]);
	fit_func[k] -> Draw("same");
}
	c_angl_dist->cd();
	
	TGraphErrors * gr1 = new TGraphErrors(11, theta, peaks_value, theta_err, peaks_value_err);
	gr1 -> Draw("A*");
	
	return 0;
}


