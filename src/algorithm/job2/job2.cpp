/*
 * ************** job2.cpp *******************************
 *
 * Owner: Thomas Klemm
 *
 * 13.07.2001
*/

#include<iostream.h>
#include"../../basics/matrix.hpp"
#include"../../basics/global.hpp"
#include"../../lisa_dt/ctrlpara.hpp"
#include"../../lisa_dt/schedule.hpp"
#include"../../lisa_dt/ptype.hpp"
#include"../../lisa_dt/lvalues.hpp"
#include<fstream.h>
#include"../../basics/except.hpp"
#include"../../lisa_dt/mo.hpp"

int n,m;

int main(int argc, char *argv[]) 
{

    // Auskommentieren, falls die Fehlermeldungen weitergesendet werden sollen
    //  G_ExceptionList.set_output_to_cout();   

    Lisa_ProblemType * lpr = new Lisa_ProblemType;
    Lisa_ControlParameters * sp = new Lisa_ControlParameters;
    Lisa_Values * my_werte=new Lisa_Values;
    
    ifstream i_strm(argv[1]);
    ofstream o_strm(argv[2]);

    // read problem description 
    i_strm >> (*lpr);

    // read control parameters: 
    i_strm >> (*sp);

    // read problem instance:
    i_strm >> (*my_werte);
    
    // LiSA  schedule object for storing results
    Lisa_Schedule * out_schedule = new Lisa_Schedule(my_werte->get_n(),
						     my_werte->get_m());
    out_schedule->make_LR();
    

//J2-Algorithmus 
 
  n=my_werte->get_n(); 
  m=my_werte->get_m(); 
  Lisa_Matrix<bool> S(n,m); 
  Lisa_Matrix<double> P(n,m); 
  Lisa_Matrix<int> M(n,m), J(n,m), PR(n,m); 
  P=(*my_werte->PT); 
  S=(*my_werte->SIJ);
  my_werte->MO->write_rank(&M);
  Lisa_Vector<int> I1(n), I2(n), I12(n), I21(n); 
 
  int a=0; int b=0; int c=0; int d=0; 
  int i=0; int j=0;
	 


//Zul�ssigkeit der Bearbeitungszeitenmatrix 
 
	for (i=0;i<n;i++) 
		{if (P[i][0]==0)	 
			{if (S[i][0]==true) 
			{cout << "WARNING: SIJ bzgl PT unzulaessig!" << endl; 
			 cout << "WARNING: Ergebnisse falsch!!!!!!!" << endl;
			 break;} 
			} 
		if (P[i][1]==0) 
			{if (S[i][1]==true) 
			{cout << "WARNING: SIJ bzgl PT unzulaessig!" << endl;
			 cout << "WARNING: Ergebnisse falsch!!!!!!!" << endl; 
			break;} }
		} 

	 for (i=0;i<n;i++)
		{if (S[i][0]==false)
			{if (S[i][1]==false) 
				{cout << "WARNING: SIJ an beiden Maschinen Null bei Job " << i+1 << endl; 
				 break;}}} 


  //Mengeneinteilungen 
 
  for (i=0; i<n; i++) 
	{ 
	if (M[i][0]<M[i][1]) 
	   { 
	    if (M[i][0]==0) {I2[b]=i; b++;} 
	    else	    {I12[c]=i; c++;} 
	   } 
	else	 
	   { 
	    if (M[i][1]==1) {I21[d]=i; d++;} 
	    else	    {I1[a]=i; a++;} 
	   } 
	} 

 
  //Johnson Schritt 
 
  Lisa_Matrix<double> P12(c,m), P21(d,m);
  Lisa_Matrix<int> PR12(c,m), PR21(d,m); 
  int x=0; 
 
  for (i=0; i<c; i++) 
	{x=I12[i]; P12[i][0]=P[x][0]; P12[i][1]=P[x][1];}  
	 
  for (i=0; i<d; i++) 
	{x=I21[i]; P21[i][0]=P[x][1]; P21[i][1]=P[x][0];} 


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		 
	//JOHNSONSCHE REGEL der F2-Algorithmus f�r 
        //P12 mit n=c und P21 mit n=d 
	//Ergebniss:	PR12(c,m) und PR21(d,m)  

	int e=1;
	int f=c;
	double Max=0;

 	while (e<f+1) 
 
	 { int k=0; int l=0; 
	   for (i=0; i<c; i++) 
	       {for (int j=0; j<m; j++) 
	           {if (P12[i][j]<P12[k][l]) {k=i; l=j;} 
		    else  
			{if (Max<P12[i][j])   {Max=P12[i][j]+1;} 
			} //Minimum auf P12 bestimmt, abgespeichert bei k und l 
		    }
		}
 
  	if (l==0)					//In Listen einf�gen 
 		{PR12[k][0]=e; PR12[k][1]=e+1; e++;} 
 
	else	{PR12[k][1]=f+1; PR12[k][0]=f; f--;} 
		         	
	P12[k][0]=Max; P12[k][1]=Max;			//Streichen von Job k
 	    						// PR12 bestimmt

	}


	e=1;
	f=d;
	Max=0;

 	while (e<f+1) 
 
	 { int k=0;int l=0; 
	   for (i=0; i<d; i++) 
	       {for (int j=0; j<m; j++) 
	           {if (P21[i][j]<P21[k][l]) {k=i; l=j;} 
		    else  
			{if (Max<P21[i][j])   {Max=P21[i][j]+1;} 
			} 
		   } //Minimum auf P21 bestimmt, abgespeichert bei k und l 
		} 
 
  	if (l==0)					//In Listen einf�gen 
 		{PR21[k][1]=e; PR21[k][0]=e+1; e++;} 
 
	else	{PR21[k][0]=f+1; PR21[k][1]=f; f--;} 
		         	
	P21[k][0]=Max; P21[k][1]=Max;			//Streichen von Job k
 	}    						// PR21 bestimmt


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // Sortierung der Ergebnisse f�r Maschine 0 

	int cneu=c; 	for (i=0; i<c; i++) 
		{x=I12[i]; J[x][0]=PR12[i][0];}				//PR12 
	 
	for (i=0; i<a; i++) 	
		{x=I1[i]; J[x][0]=cneu+1; cneu++;}			//I1 
		 		 
	for (i=0; i<d; i++) 
		{x=I21[i]; J[x][0]=PR21[i][0]+cneu-1;}			//PR21

  //Sortierung der Ergebnisse f�r Maschine 1 
 
	int dneu=d;
	for (i=0; i<d; i++) 
		{x=I21[i]; J[x][1]=PR21[i][1];}				//PR21 
	 
	for (i=0; i<b; i++) 
		{x=I2[i]; J[x][1]=dneu+1; dneu++;}			//I2 
		  		 
	for (i=0; i<c; i++) 
		{x=I12[i]; J[x][1]=PR12[i][1]+dneu-1;}			//PR12 
		

	   
  //Berechnung von LR aus M und J  
 
	Lisa_Matrix<int> MJ(n,m);  
	Lisa_Vector<int> z(n), s(m); 
 
	  for (i=0;i<n;i++) 
	    	{for (j=0;j<m;j++) 
			{MJ[i][j]=M[i][j]+J[i][j];} 
		} 
	   
	  c=0; 
	  int k=1; 
	  while (c<n*m) 
		{ 
	      for (i=0;i<n;i++)	{z[i]=0;} 
	      for (j=0;j<m;j++)	{s[j]=0;} 
 
	      f=0; 
 	      for (i=0;i<n;i++) 
			{for (j=0;j<m;j++) 
		    		{if (MJ[i][j]==2) 
					{f=1; 
					  c++; 
					  z[i]=1; 
					  s[j]=1; 
					  PR[i][j]=k; 
					} 
		    		} 
			} 
	      k++; 
		if (f==0) {c=n*m;} 
		else
		     {for (i=0;i<n;i++) 
			    {for (j=0;j<m;j++) 
				{if (MJ[i][j]!=0) 
			    		{MJ[i][j]=MJ[i][j]-z[i]-s[j];} 
				} 
		      }   } 
		}	 
 
	for (i=0;i<n;i++)   
		{for (j=0; j<m; j++)  {(*out_schedule->LR)[i][j]=PR[i][j];} 
		}	  
 
    
    // write results to output file:
    o_strm << *out_schedule;
    delete out_schedule;
    delete my_werte;
    delete lpr;
}

