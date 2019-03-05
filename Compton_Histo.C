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

	//hist_point->Scale(1./acquisition_time);
  
	
};



	





int main() {

//Definisco le variabili

	float		low_b=-0.5;
	float		high_b= pow(2.0,13.0)+0.5;
	unsigned	bin_size= pow(2.0,0.0);


float	times[9]={215143,210434,211795,208405,202817,204464,210440,212251,210548};
int		theta[9]={0,10,20,5,2,-2,-5,-10,-20};
string	F_names[9]={"dati/1V3_Angular_Dispersion_0d_CO.log","dati/1V4_Angular_Dispersion_10d_CO.log","dati/1V5_Angular_Dispersion_20d_CO.log","dati/1V6_Angular_Dispersion_5d_CO.log",
				 "dati/1V7_Angular_Dispersion_2d_CO.log","dati/1V8_Angular_Dispersion_m2d_CO.log","dati/1V9_Angular_Dispersion_m5d_CO.log","dati/1V10_Angular_Dispersion_m10d_CO.log",
				 "dati/1V11_Angular_Dispersion_m20d_CO.log"};
TH1F*	histarray[9];

//float	times[4]={210434,211795,212251,210548};
//int		theta[4]={10,20,-10,-20};
//string	F_names[4]={"dati/1V4_Angular_Dispersion_10d_CO.log","dati/1V5_Angular_Dispersion_20d_CO.log","dati/1V10_Angular_Dispersion_m10d_CO.log",
				 //"dati/1V11_Angular_Dispersion_m20d_CO.log"};
//TH1F*	histarray[4];

//Apro il Canvas

   
   TCanvas *c_angl = new TCanvas("c_angl","Distribuzione in impulso a vari angoli",1);   
   TCanvas *c_coinc = new TCanvas("c_coinc","Distribuzione a 20° con coincidenza",1);
	c_angl->cd();

int k=0;
for(k=0;k<9;k++){
	TString numstr=ToString(theta[k]);
	TString histoname="histo_theta_"+numstr;
	histarray[k] = new TH1F(histoname, histoname, int((high_b-low_b-1.0)/bin_size), low_b, high_b);
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
}
	c_coinc->cd();   
	TH1F * hist_coinc = new TH1F("hist_coinc", "Distribuzione a 20° con coincidenza", int((high_b-low_b-1.0)/bin_size), low_b, high_b);
	HistoDrawer("dati/2M7_global_test_2s_20d_CO.log",1,hist_coinc);
	hist_coinc->SetStats(0);
	hist_coinc->Draw();
	return 0;
}


