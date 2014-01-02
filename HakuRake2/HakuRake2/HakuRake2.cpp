#include "stdafx.h"
#include "itpp/itcomm.h"
#include <fstream>

using namespace itpp;
using namespace std;
class mGold
{
public:
	int numG;
	bvec p1,p2,ps1,ps2,c1,c2;
	vec cv1,cv2;
	cvec c,cConjugate;
	
	mGold()
	{
	Parser p(std::string("config.txt"));
	numG=p.get_int("numG");
	p1.set_length(25);
	p2.set_length(25);
	
	ps1.set_length(numG);
	ps2.set_length(numG);
	
	c1.set_length(numG);
	c2.set_length(numG);

	cv1.set_length(numG);
	cv2.set_length(numG);
	c.set_length(numG);
	cConjugate.set_length(numG);
	p1.zeros();
	p2.zeros();
	p1[0]=1;
	p2[0]=1;
	}
	~mGold()
	{
		
	}
	void generate()
	{
		for(int i=0;i<numG;i++)
	{
		//cout<<"p1"<<endl;
		//cout<<p1<<endl;
		//cout<<"p2"<<endl;
		//cout<<p2<<endl;
		
		ps1[i]=p1[20]+p1[17]+p1[6];
		ps2[i]=p2[20]+p2[18]+p2[7];

		p1.shift_right(p1[0]);
		p2.shift_right(p2[0]);

		p1[0]=p1[21]+p1[24];
		p2[0]=p2[21]+p2[22]+p2[23]+p2[24];

		c1[i]=p1[24]+p2[24];
		c2[i]=ps1[i]+ps2[i];
		
	}
		

	for(int i=0;i<numG;i++)
	{
		(c1(i)==0) ? cv1(i)=1: cv1(i)=-1;
		(c2(i)==0) ? cv2(i)=1: cv2(i)=-1;

	
	}
	
	complex<double>temp;
	for(int i=0;i<numG;i++)
	{
		temp._Val[0]=cv1(i);
		temp._Val[1]=pow(-1,i)*cv2(2*floor(i/2.0));
		c(i)=temp;
		cConjugate[i]=c(i);
		cConjugate[i]._Val[1]=cConjugate[i]._Val[1]*(-1.0);
		

	
	}
	
	
	}
};

class transmiter
{
public:
	int SF;
	int SF_EDPDCH2,SF_EDPDCH3;

	int DPCCH_NBits;

	int DPCCH_SLOT0,DPCCH_SLOT1,DPCCH_SLOT2,DPCCH_SLOT3,DPCCH_SLOT4,DPCCH_SLOT5,DPCCH_SLOT6,DPCCH_SLOT7;
	int DPCCH_SLOT8,DPCCH_SLOT9,DPCCH_SLOT10,DPCCH_SLOT11,DPCCH_SLOT12,DPCCH_SLOT13,DPCCH_SLOT14;
	
	

	int HS_DPCCH_NBits;

	int E_DPDCH1_NData, E_DPDCH2_NData,E_DPDCH3_NData,E_DPDCH4_NData;
	int E_DPCCH_NBits;
	double betaDPCCH,betaE_DPCCH,betaE_DPDCH1,betaE_DPDCH2,betaE_DPDCH3,betaE_DPDCH4,betaE_HS_DPCCH;
	double betaDPCCHlin,betaE_DPDCH1lin,betaE_DPDCH2lin,betaE_DPDCH3lin,betaE_DPDCH4lin,
			betaE_DPCCHlin,betaE_HS_DPCCHlin;

	vec OVSF256_0,OVSF256_1,OVSF256_33,OVSF256_64;
	vec	OVSF4_1,OVSF4_3;
	vec OVSF2_1;

	bvec DPCCH,DPCCHall;
	bvec HS_DPCCH,HS_DPCCHall;
	bvec E_DPDCH1,E_DPDCH2,E_DPDCH3,E_DPDCH4,E_DPDCH1all,E_DPDCH2all,E_DPDCH3all,E_DPDCH4all;
	bvec E_DPCCH,E_DPCCHall;

	vec  SP_DPCCH;
	vec  SP_HS_DPCCH;
	vec	 SP_E_DPDCH1,SP_E_DPDCH2,SP_E_DPDCH3,SP_E_DPDCH4;
	vec  SP_E_DPCCH;


	cvec radioFrame;

	mGold gold;

	smat OVSF,OVSF4,OVSF2;

	vec spreading(bvec data,vec code)
	{
		vec result;
		vec code_neg=code*-1;
		for(int i=0; i<data.length();i++)
		{
		if(data[i]==1)
		{
			result=concat(result,code);
		}
		else
		{
			result=concat(result,code_neg);
		}
		}
		return result;
	}
	cvec scrambling(cvec data,cvec code)
	{
		cvec result;
		result.set_length(data.length());
		result.zeros();
		for(int i=0;i<data.length();i++)
		{
			result[i]=data(i)*code(i);
		}
		return result;
	}

	transmiter()
	{
		Parser p(std::string("config.txt"));
		DPCCH_NBits=p.get_int("DPCCH_NBits");
		
		HS_DPCCH_NBits=p.get_int("HS_DPCCH_NBits");
		E_DPCCH_NBits=p.get_int("E_DPCCH_NBits");
		E_DPDCH1_NData=p.get_int("E_DPDCH1_NData");
		E_DPDCH2_NData=p.get_int("E_DPDCH2_NData");
		E_DPDCH3_NData=p.get_int("E_DPDCH3_NData");
		E_DPDCH4_NData=p.get_int("E_DPDCH4_NData");
		SF=p.get_int("SF");
		SF_EDPDCH2=p.get_int("SF_EDPDCH2");
		SF_EDPDCH3=p.get_int("SF_EDPDCH3");
		DPCCH_SLOT0=p.get_int("DPCCH_SLOT0");
		DPCCH_SLOT1=p.get_int("DPCCH_SLOT1");
		DPCCH_SLOT2=p.get_int("DPCCH_SLOT2");
		DPCCH_SLOT3=p.get_int("DPCCH_SLOT3");
		DPCCH_SLOT4=p.get_int("DPCCH_SLOT4");
		DPCCH_SLOT5=p.get_int("DPCCH_SLOT5");
		DPCCH_SLOT6=p.get_int("DPCCH_SLOT6");
		DPCCH_SLOT7=p.get_int("DPCCH_SLOT7");
		DPCCH_SLOT8=p.get_int("DPCCH_SLOT8");
		DPCCH_SLOT9=p.get_int("DPCCH_SLOT9");
		DPCCH_SLOT10=p.get_int("DPCCH_SLOT10");
		DPCCH_SLOT11=p.get_int("DPCCH_SLOT11");
		DPCCH_SLOT12=p.get_int("DPCCH_SLOT12");				
		DPCCH_SLOT13=p.get_int("DPCCH_SLOT13");
		DPCCH_SLOT14=p.get_int("DPCCH_SLOT14");
		 
		betaDPCCH=p.get_int("betaDPCCH");
		betaE_DPDCH1=p.get_int("betaE_DPDCH1");
		betaE_DPDCH2=p.get_int("betaE_DPDCH2");
		betaE_DPDCH3=p.get_int("betaE_DPDCH3");
		betaE_DPDCH4=p.get_int("betaE_DPDCH4");
		betaE_DPCCH=p.get_int("betaE_DPCCH");
		betaE_HS_DPCCH=p.get_int("betaE_HS_DPCCH");
	
		betaDPCCHlin=pow10(betaDPCCH/10.0);
		betaE_DPDCH1lin=pow10(betaE_DPDCH1/10.0);
		betaE_DPDCH2lin=pow10(betaE_DPDCH2/10.0);
		betaE_DPDCH3lin=pow10(betaE_DPDCH3/10.0);
		betaE_DPDCH4lin=pow10(betaE_DPDCH4/10.0);
		betaE_DPCCHlin=pow10(betaE_DPCCH/10.0);
		betaE_HS_DPCCHlin=pow10(betaE_HS_DPCCH/10.0);

		OVSF=wcdma_spreading_codes (SF);
		OVSF4=wcdma_spreading_codes(SF_EDPDCH3);
		OVSF2=wcdma_spreading_codes(SF_EDPDCH2);

		OVSF256_0.set_length(SF,false);
		OVSF256_1.set_length(SF,false);
		OVSF256_33.set_length(SF,false);
		OVSF256_64.set_length(SF,false);
		
		OVSF4_1.set_length(SF_EDPDCH3,false);
		OVSF4_3.set_length(SF_EDPDCH3,false);
		OVSF2_1.set_length(SF_EDPDCH2,false);

		for(int i=0;i<OVSF.cols();i++)
		{
		OVSF256_0[i]=OVSF(0,i);
		OVSF256_1[i]=OVSF(1,i);
		OVSF256_33[i]=OVSF(33,i);
		OVSF256_64[i]=OVSF(64,i);
		
		}
		for(int i=0;i<OVSF4.cols();i++)
		{
			OVSF4_1[i]=OVSF4(1,i);
		
		
		}

		for(int i=0;i<OVSF2.cols();i++)
		{
			OVSF2_1[i]=OVSF2(1,i);
		}
		bvec DPCCH0=dec2bin(DPCCH_SLOT0,true);
		bvec DPCCH1=dec2bin(DPCCH_SLOT1,true);
		bvec DPCCH2=dec2bin(DPCCH_SLOT2,true);
		bvec DPCCH3=dec2bin(DPCCH_SLOT3,true);
		bvec DPCCH4=dec2bin(DPCCH_SLOT4,true);
		bvec DPCCH5=dec2bin(DPCCH_SLOT5,true);
		bvec DPCCH6=dec2bin(DPCCH_SLOT6,true);
		bvec DPCCH7=dec2bin(DPCCH_SLOT7,true);
		bvec DPCCH8=dec2bin(DPCCH_SLOT8,true);
		bvec DPCCH9=dec2bin(DPCCH_SLOT9,true);
		bvec DPCCH10=dec2bin(DPCCH_SLOT10,true);
		bvec DPCCH11=dec2bin(DPCCH_SLOT11,true);
		bvec DPCCH12=dec2bin(DPCCH_SLOT12,true);
		bvec DPCCH13=dec2bin(DPCCH_SLOT13,true);
		bvec DPCCH14=dec2bin(DPCCH_SLOT14,true);
		
		DPCCH.set_length(DPCCH_NBits,false);
		bvec conDPCCH0=concat(DPCCH0,DPCCH1,DPCCH2,DPCCH3,DPCCH4);
		bvec conDPCCH1=concat(DPCCH5,DPCCH6,DPCCH7,DPCCH8,DPCCH9);
		bvec conDPCCH2=concat(DPCCH10,DPCCH11,DPCCH12,DPCCH13,DPCCH14);
		DPCCH=concat(conDPCCH0,conDPCCH1,conDPCCH2);

		gold.generate();


		
	}
	~transmiter(){};
};
class receiver
{
public:
	bvec DPCCH;
	bvec HS_DPCCH;
	bvec E_DPDCH1,E_DPDCH2,E_DPDCH3,E_DPDCH4;
	bvec E_DPCCH;

	vec	 iRecive;
	vec	 rRecive;
	receiver(){};
	~receiver(){};
	bvec despreading(int lenData,vec code,bool img)
	{
		int lenCode=code.length();
		mat matrix;
		bvec result;
		vec dspread;
		result.set_length(lenData);
		dspread.set_length(lenData);
		matrix.set_size(lenData,lenCode);
		dspread.zeros();
		for(int i=0;i<lenData;i++)
		{
			for(int j=0;j<lenCode;j++)
			{
				if(img==true)
				{
					matrix(i,j)=iRecive((i*lenCode)+j);
				}
				else
				{
					matrix(i,j)=rRecive((i*lenCode)+j);
				}
			}
		}
		vec temp;
		for(int i=0;i<lenData;i++)
		{
			temp=matrix.get_row(i);
		for(int j=0;j<lenCode;j++)
		{
			dspread[i]+=(temp(j)*code(j));
		}
		(dspread[i]>0) ? result[i]=1 : result[i]=0;
		}

		return result;
	}
	cvec descrambling(cvec data,cvec code)
	{
		cvec result;
		result.set_length(data.length());
		result.zeros();
		for(int i=0;i<data.length();i++)
		{
			result[i]=data(i)*code(i);
		}
		return result;
	}


};

int _tmain(int argc, _TCHAR* argv[])
{
	Parser p(std::string("config.txt"));

	transmiter tr;
	AWGN_Channel awg;
	receiver rec;
	GlobalRNG_randomize();
	BERC brHS_DPCCH,brDPCCH,brE_DPCCH,brE_DPDCH1,brE_DPDCH2,brE_DPDCH3,brE_DPDCH4;
	vec berHS_DPCCH,berDPCCH,berE_DPCCH,berE_DPDCH1,berE_DPDCH2,berE_DPDCH3,berE_DPDCH4;
	vec EbN0dB,EbN0,N0;
	ofstream out;
	int numOfIteration=p.get_int("numOfIteration");
	int numOfErrors=100;
	double Eb;
	EbN0dB=linspace(-20,2,12);

	EbN0=pow(10,EbN0dB/10.0);
	Eb=tr.betaDPCCHlin+tr.betaE_DPCCHlin+tr.betaE_DPDCH1lin+tr.betaE_DPDCH2lin+tr.betaE_DPDCH3lin
		+tr.betaE_DPDCH4lin+tr.betaE_HS_DPCCHlin;
	N0=Eb*pow(EbN0,-1.0);

	berHS_DPCCH.set_size(EbN0dB.length(),false);
	berDPCCH.set_size(EbN0dB.length(),false);
	berE_DPCCH.set_size(EbN0dB.length(),false);
	berE_DPDCH1.set_size(EbN0dB.length(),false);
	berE_DPDCH2.set_size(EbN0dB.length(),false);
	berE_DPDCH3.set_size(EbN0dB.length(),false);
	berE_DPDCH4.set_size(EbN0dB.length(),false);

	TDL_Channel tdlChanel;
	CHANNEL_PROFILE chProfile;
	chProfile= ITU_Vehicular_A;
	double samplingTime=(0.5)/(38.4e6);
	tdlChanel.set_channel_profile(chProfile,samplingTime);
	
	
	vec powerdB;
	ivec delayPath;
	tdlChanel.get_channel_profile(powerdB,delayPath);
	cout<<delayPath<<endl;
	cvec path1,path2,path3,path4,path5,path6;
	
	cvec radioFrameScrambling,radioFrameALL,radioFrameDescr,radioFrameChannel1,radioFrameChannel2;
	cmat coffeChannel;
	tdlChanel.init();
	cvec mulPath1,mulPath2,mulPath3,mulPath4,mulPath5,mulPath6,sum;
	mulPath1.set_length(38400);
	mulPath2.set_length(38400);
	mulPath3.set_length(38400);
	mulPath4.set_length(38400);
	mulPath5.set_length(38400);
	mulPath6.set_length(38400);
	sum.set_length(38400);
	for(int i=0;i<EbN0dB.length();i++)
	{
			brDPCCH.clear();
			brHS_DPCCH.clear();
			brE_DPCCH.clear();
			brE_DPDCH1.clear();
			brE_DPDCH2.clear();
			brE_DPDCH3.clear();
			brE_DPDCH4.clear();
			cout<<"Eb N0: "<<EbN0dB(i)<<endl;

			tr.E_DPCCHall.set_size(0);
			tr.DPCCHall.set_size(0);
			tr.HS_DPCCHall.set_size(0);
			tr.E_DPDCH1all.set_size(0);
			tr.E_DPDCH2all.set_size(0);
			tr.E_DPDCH3all.set_size(0);
			tr.E_DPDCH4all.set_size(0);
	
			for(int k=0;k<numOfIteration;k++)
			{
			cout<<"Num of Iteration: "<<k+1<<endl;
			//DPCCH
			tr.SP_DPCCH=tr.spreading(tr.DPCCH,tr.OVSF256_0);
			tr.DPCCHall=concat(tr.DPCCHall,tr.DPCCH);
			//HS-DPCCH
			tr.HS_DPCCH=randb(tr.HS_DPCCH_NBits);
			tr.HS_DPCCHall=concat(tr.HS_DPCCHall,tr.HS_DPCCH);
			tr.SP_HS_DPCCH=tr.spreading(tr.HS_DPCCH,tr.OVSF256_33);
			//E-DPCCH
			tr.E_DPCCH=randb(tr.E_DPCCH_NBits);
			tr.E_DPCCHall=concat(tr.E_DPCCHall,tr.E_DPCCH);
			tr.SP_E_DPCCH=tr.spreading(tr.E_DPCCH,tr.OVSF256_1);
			//E-DPDCH
			tr.E_DPDCH1=randb(tr.E_DPDCH1_NData);
			tr.E_DPDCH2=randb(tr.E_DPDCH2_NData);
			tr.E_DPDCH3=randb(tr.E_DPDCH3_NData);
			tr.E_DPDCH4=randb(tr.E_DPDCH4_NData);
			tr.E_DPDCH1all=concat(tr.E_DPDCH1,tr.E_DPDCH1all);
			tr.E_DPDCH2all=concat(tr.E_DPDCH2,tr.E_DPDCH2all);
			tr.E_DPDCH3all=concat(tr.E_DPDCH3,tr.E_DPDCH3all);
			tr.E_DPDCH4all=concat(tr.E_DPDCH4,tr.E_DPDCH4all);
			tr.SP_E_DPDCH1=tr.spreading(tr.E_DPDCH1,tr.OVSF2_1);
			tr.SP_E_DPDCH2=tr.spreading(tr.E_DPDCH2,tr.OVSF2_1);
			tr.SP_E_DPDCH3=tr.spreading(tr.E_DPDCH3,tr.OVSF4_1);
			tr.SP_E_DPDCH4=tr.spreading(tr.E_DPDCH4,tr.OVSF4_1);
			//beta
	
			tr.SP_DPCCH=tr.SP_DPCCH*tr.betaDPCCHlin;
			tr.SP_HS_DPCCH=tr.SP_HS_DPCCH*tr.betaE_HS_DPCCHlin;
			tr.SP_E_DPCCH=tr.SP_E_DPCCH*tr.betaE_DPCCHlin;
			tr.SP_E_DPDCH1=tr.SP_E_DPDCH1*tr.betaE_DPDCH1lin;
			tr.SP_E_DPDCH2=tr.SP_E_DPDCH2*tr.betaE_DPDCH2lin;
			tr.SP_E_DPDCH3=tr.SP_E_DPDCH3*tr.betaE_DPDCH3lin;
			tr.SP_E_DPDCH4=tr.SP_E_DPDCH4*tr.betaE_DPDCH4lin;	
	
			//radio frame
			tr.radioFrame.set_length(tr.gold.numG);
			for(int i=0;i<tr.gold.numG;i++)
			{
				tr.radioFrame[i]._Val[0]=tr.SP_E_DPCCH[i]+tr.SP_E_DPDCH1[i]+tr.SP_E_DPDCH3[i];
				tr.radioFrame[i]._Val[1]=tr.SP_DPCCH[i]+tr.SP_HS_DPCCH[i]+tr.SP_E_DPDCH2[i]+tr.SP_E_DPDCH4[i];
	
			}
			
			                       
	
			
			radioFrameScrambling=tr.scrambling(tr.radioFrame,tr.gold.c);
			radioFrameALL=concat(radioFrameALL,radioFrameScrambling);
						
			}

			//kanal
			awg.set_noise(N0(i));
			radioFrameChannel1=awg(radioFrameALL);
		
			tdlChanel(radioFrameChannel1,radioFrameChannel2,coffeChannel);
			
			//odbiornik
			coffeChannel=conj(coffeChannel);
			path1.set_length(38400*numOfIteration);
			path2.set_length(38400*numOfIteration);
			path3.set_length(38400*numOfIteration);
			path4.set_length(38400*numOfIteration);
			path5.set_length(38400*numOfIteration);
			path6.set_length(38400*numOfIteration);
			path1.zeros();
			path2.zeros();
			path3.zeros();
			path4.zeros();
			path5.zeros();
			path6.zeros();

				for(int i=0;i<38400*numOfIteration;i++)
				{
					path1(i)=radioFrameChannel2(i);
					if(delayPath(1)+i<38400*numOfIteration)
					{
						path2(i)=radioFrameChannel2(i+delayPath(1));
					}
					if(delayPath(2)+i<38400*numOfIteration)
					{
						path3(i)=radioFrameChannel2(i+delayPath(2));
					}
					if(delayPath(3)+i<38400*numOfIteration)
					{
						path4(i)=radioFrameChannel2(i+delayPath(3));
					}
					if(delayPath(4)+i<38400*numOfIteration)
					{
						path5(i)=radioFrameChannel2(i+delayPath(4));
					}
					if(delayPath(5)+i<38400*numOfIteration)
					{
						path6(i)=radioFrameChannel2(i+delayPath(5));
					}
				}
				
				cvec path1Descrambling,path2Descrambling,path3Descrambling,path4Descrambling,path5Descrambling,path6Descrambling;
				vec path1DescramblingReal,path2DescramblingReal,path3DescramblingReal,path4DescramblingReal,path5DescramblingReal,path6DescramblingReal;
				vec path1DescramblingImag,path2DescramblingImag,path3DescramblingImag,path4DescramblingImag,path5DescramblingImag,path6DescramblingImag;
				vec correctionFactorPath1,correctionFactorPath2,correctionFactorPath3,correctionFactorPath4,correctionFactorPath5,correctionFactorPath6;
				correctionFactorPath1.set_length(numOfIteration*38400);
				correctionFactorPath2.set_length(numOfIteration*38400);
				correctionFactorPath3.set_length(numOfIteration*38400);
				correctionFactorPath4.set_length(numOfIteration*38400);
				correctionFactorPath5.set_length(numOfIteration*38400);
				correctionFactorPath6.set_length(numOfIteration*38400);

				path1Descrambling.set_length(numOfIteration*38400);
				path2Descrambling.set_length(numOfIteration*38400);
				path3Descrambling.set_length(numOfIteration*38400);
				path4Descrambling.set_length(numOfIteration*38400);
				path5Descrambling.set_length(numOfIteration*38400);
				path6Descrambling.set_length(numOfIteration*38400);

				for(int z=0;z<numOfIteration*38400;z++)
				{
					
					correctionFactorPath1[i]=abs(coffeChannel(i,0))*EbN0(i);
					correctionFactorPath2[i]=abs(coffeChannel(i,1))*EbN0(i);
					correctionFactorPath3[i]=abs(coffeChannel(i,2))*EbN0(i);
					correctionFactorPath4[i]=abs(coffeChannel(i,3))*EbN0(i);
					correctionFactorPath5[i]=abs(coffeChannel(i,4))*EbN0(i);
					correctionFactorPath6[i]=abs(coffeChannel(i,5))*EbN0(i);
				}

				//Descrambling
				for(int z=0;z<numOfIteration*38400;z++)
				{
					path1Descrambling[z]=path1(z)*tr.gold.cConjugate(z%38400);
					path2Descrambling[z]=path2(z)*tr.gold.cConjugate(z%38400);
					path3Descrambling[z]=path3(z)*tr.gold.cConjugate(z%38400);
					path4Descrambling[z]=path4(z)*tr.gold.cConjugate(z%38400);
					path5Descrambling[z]=path5(z)*tr.gold.cConjugate(z%38400);
					path6Descrambling[z]=path6(z)*tr.gold.cConjugate(z%38400);
				}
				
				path1DescramblingReal=real(path1Descrambling);
				path2DescramblingReal=real(path2Descrambling);
				path3DescramblingReal=real(path3Descrambling);
				path4DescramblingReal=real(path4Descrambling);
				path5DescramblingReal=real(path5Descrambling);
				path6DescramblingReal=real(path6Descrambling);

				path1DescramblingImag=imag(path1Descrambling);
				path2DescramblingImag=imag(path2Descrambling);
				path3DescramblingImag=imag(path3Descrambling);
				path4DescramblingImag=imag(path4Descrambling);
				path5DescramblingImag=imag(path5Descrambling);
				path6DescramblingImag=imag(path6Descrambling);

				vec path1HS_DPCCH,path2HS_DPCCH,path3HS_DPCCH,path4HS_DPCCH,path5HS_DPCCH,path6HS_DPCCH;
				vec path1DPCCH,path2DPCCH,path3DPCCH,path4DPCCH,path5DPCCH,path6DPCCH;
				vec path1E_DPCCH,path2E_DPCCH,path3E_DPCCH,path4E_DPCCH,path5E_DPCCH,path6E_DPCCH;
				vec path1E_DPDCH1,path2E_DPDCH1,path3E_DPDCH1,path4E_DPDCH1,path5E_DPDCH1,path6E_DPDCH1;
				vec path1E_DPDCH2,path2E_DPDCH2,path3E_DPDCH2,path4E_DPDCH2,path5E_DPDCH2,path6E_DPDCH2;
				vec path1E_DPDCH3,path2E_DPDCH3,path3E_DPDCH3,path4E_DPDCH3,path5E_DPDCH3,path6E_DPDCH3;
				vec path1E_DPDCH4,path2E_DPDCH4,path3E_DPDCH4,path4E_DPDCH4,path5E_DPDCH4,path6E_DPDCH4;

				path1HS_DPCCH.set_length(numOfIteration*38400);
				path2HS_DPCCH.set_length(numOfIteration*38400);
				path3HS_DPCCH.set_length(numOfIteration*38400);
				path4HS_DPCCH.set_length(numOfIteration*38400);
				path5HS_DPCCH.set_length(numOfIteration*38400);
				path6HS_DPCCH.set_length(numOfIteration*38400);

				path1DPCCH.set_length(numOfIteration*38400);
				path2DPCCH.set_length(numOfIteration*38400);
				path3DPCCH.set_length(numOfIteration*38400);
				path4DPCCH.set_length(numOfIteration*38400);
				path5DPCCH.set_length(numOfIteration*38400);
				path6DPCCH.set_length(numOfIteration*38400);

				path1E_DPCCH.set_length(numOfIteration*38400);
				path2E_DPCCH.set_length(numOfIteration*38400);
				path3E_DPCCH.set_length(numOfIteration*38400);
				path4E_DPCCH.set_length(numOfIteration*38400);
				path5E_DPCCH.set_length(numOfIteration*38400);
				path6E_DPCCH.set_length(numOfIteration*38400);

				path1E_DPDCH1.set_length(numOfIteration*38400);
				path2E_DPDCH1.set_length(numOfIteration*38400);
				path3E_DPDCH1.set_length(numOfIteration*38400);
				path4E_DPDCH1.set_length(numOfIteration*38400);
				path5E_DPDCH1.set_length(numOfIteration*38400);
				path6E_DPDCH1.set_length(numOfIteration*38400);

				path1E_DPDCH2.set_length(numOfIteration*38400);
				path2E_DPDCH2.set_length(numOfIteration*38400);
				path3E_DPDCH2.set_length(numOfIteration*38400);
				path4E_DPDCH2.set_length(numOfIteration*38400);
				path5E_DPDCH2.set_length(numOfIteration*38400);
				path6E_DPDCH2.set_length(numOfIteration*38400);

				path1E_DPDCH3.set_length(numOfIteration*38400);
				path2E_DPDCH3.set_length(numOfIteration*38400);
				path3E_DPDCH3.set_length(numOfIteration*38400);
				path4E_DPDCH3.set_length(numOfIteration*38400);
				path5E_DPDCH3.set_length(numOfIteration*38400);
				path6E_DPDCH3.set_length(numOfIteration*38400);

				path1E_DPDCH4.set_length(numOfIteration*38400);
				path2E_DPDCH4.set_length(numOfIteration*38400);
				path3E_DPDCH4.set_length(numOfIteration*38400);
				path4E_DPDCH4.set_length(numOfIteration*38400);
				path5E_DPDCH4.set_length(numOfIteration*38400);
				path6E_DPDCH4.set_length(numOfIteration*38400);


				for(int i=0;i<numOfIteration*38400;i++)
				{
					//HS_DPCCH
					path1HS_DPCCH[i]=path1DescramblingImag(i)*tr.OVSF256_33(i%256)*correctionFactorPath1(i);
					path2HS_DPCCH[i]=path2DescramblingImag(i)*tr.OVSF256_33(i%256)*correctionFactorPath2(i);
					path3HS_DPCCH[i]=path3DescramblingImag(i)*tr.OVSF256_33(i%256)*correctionFactorPath3(i);
					path4HS_DPCCH[i]=path4DescramblingImag(i)*tr.OVSF256_33(i%256)*correctionFactorPath4(i);
					path5HS_DPCCH[i]=path5DescramblingImag(i)*tr.OVSF256_33(i%256)*correctionFactorPath5(i);
					path6HS_DPCCH[i]=path6DescramblingImag(i)*tr.OVSF256_33(i%256)*correctionFactorPath6(i);
					//DPCCH
					path1DPCCH[i]=path1DescramblingImag(i)*tr.OVSF256_0(i%256)*correctionFactorPath1(i);
					path2DPCCH[i]=path2DescramblingImag(i)*tr.OVSF256_0(i%256)*correctionFactorPath2(i);
					path3DPCCH[i]=path3DescramblingImag(i)*tr.OVSF256_0(i%256)*correctionFactorPath3(i);
					path4DPCCH[i]=path4DescramblingImag(i)*tr.OVSF256_0(i%256)*correctionFactorPath4(i);
					path5DPCCH[i]=path5DescramblingImag(i)*tr.OVSF256_0(i%256)*correctionFactorPath5(i);
					path6DPCCH[i]=path6DescramblingImag(i)*tr.OVSF256_0(i%256)*correctionFactorPath6(i);
					//E_DPCCH
					path1E_DPCCH[i]=path1DescramblingReal(i)*tr.OVSF256_1(i%256)*correctionFactorPath1(i);
					path2E_DPCCH[i]=path2DescramblingReal(i)*tr.OVSF256_1(i%256)*correctionFactorPath2(i);
					path3E_DPCCH[i]=path3DescramblingReal(i)*tr.OVSF256_1(i%256)*correctionFactorPath3(i);
					path4E_DPCCH[i]=path4DescramblingReal(i)*tr.OVSF256_1(i%256)*correctionFactorPath4(i);
					path5E_DPCCH[i]=path5DescramblingReal(i)*tr.OVSF256_1(i%256)*correctionFactorPath5(i);
					path6E_DPCCH[i]=path6DescramblingReal(i)*tr.OVSF256_1(i%256)*correctionFactorPath6(i);
					//E_DPDCH1
					path1E_DPDCH1[i]=path1DescramblingReal(i)*tr.OVSF2_1(i%2)*correctionFactorPath1(i);
					path2E_DPDCH1[i]=path2DescramblingReal(i)*tr.OVSF2_1(i%2)*correctionFactorPath2(i);
					path3E_DPDCH1[i]=path3DescramblingReal(i)*tr.OVSF2_1(i%2)*correctionFactorPath3(i);
					path4E_DPDCH1[i]=path4DescramblingReal(i)*tr.OVSF2_1(i%2)*correctionFactorPath4(i);
					path5E_DPDCH1[i]=path5DescramblingReal(i)*tr.OVSF2_1(i%2)*correctionFactorPath5(i);
					path6E_DPDCH1[i]=path6DescramblingReal(i)*tr.OVSF2_1(i%2)*correctionFactorPath6(i);
					//E_DPDCH2
					path1E_DPDCH2[i]=path1DescramblingImag(i)*tr.OVSF2_1(i%2)*correctionFactorPath1(i);
					path2E_DPDCH2[i]=path2DescramblingImag(i)*tr.OVSF2_1(i%2)*correctionFactorPath2(i);
					path3E_DPDCH2[i]=path3DescramblingImag(i)*tr.OVSF2_1(i%2)*correctionFactorPath3(i);
					path4E_DPDCH2[i]=path4DescramblingImag(i)*tr.OVSF2_1(i%2)*correctionFactorPath4(i);
					path5E_DPDCH2[i]=path5DescramblingImag(i)*tr.OVSF2_1(i%2)*correctionFactorPath5(i);
					path6E_DPDCH2[i]=path6DescramblingImag(i)*tr.OVSF2_1(i%2)*correctionFactorPath6(i);
					//E_DPDCH3
					path1E_DPDCH3[i]=path1DescramblingReal(i)*tr.OVSF4_1(i%4)*correctionFactorPath1(i);
					path2E_DPDCH3[i]=path2DescramblingReal(i)*tr.OVSF4_1(i%4)*correctionFactorPath2(i);
					path3E_DPDCH3[i]=path3DescramblingReal(i)*tr.OVSF4_1(i%4)*correctionFactorPath3(i);
					path4E_DPDCH3[i]=path4DescramblingReal(i)*tr.OVSF4_1(i%4)*correctionFactorPath4(i);
					path5E_DPDCH3[i]=path5DescramblingReal(i)*tr.OVSF4_1(i%4)*correctionFactorPath5(i);
					path6E_DPDCH3[i]=path6DescramblingReal(i)*tr.OVSF4_1(i%4)*correctionFactorPath6(i);
					//E_DPDCH3
					path1E_DPDCH4[i]=path1DescramblingImag(i)*tr.OVSF4_1(i%4)*correctionFactorPath1(i);
					path2E_DPDCH4[i]=path2DescramblingImag(i)*tr.OVSF4_1(i%4)*correctionFactorPath2(i);
					path3E_DPDCH4[i]=path3DescramblingImag(i)*tr.OVSF4_1(i%4)*correctionFactorPath3(i);
					path4E_DPDCH4[i]=path4DescramblingImag(i)*tr.OVSF4_1(i%4)*correctionFactorPath4(i);
					path5E_DPDCH4[i]=path5DescramblingImag(i)*tr.OVSF4_1(i%4)*correctionFactorPath5(i);
					path6E_DPDCH4[i]=path6DescramblingImag(i)*tr.OVSF4_1(i%4)*correctionFactorPath6(i);
				}

				vec sumHS_DPCCH,sumE_DPCCH,sumDPCCH,sumE_DPDCH1,sumE_DPDCH2,sumE_DPDCH3,sumE_DPDCH4;
				sumDPCCH.set_length(numOfIteration*38400);
				sumE_DPCCH.set_length(numOfIteration*38400);
				sumHS_DPCCH.set_length(numOfIteration*38400);
				sumE_DPDCH1.set_length(numOfIteration*38400);
				sumE_DPDCH2.set_length(numOfIteration*38400);
				sumE_DPDCH3.set_length(numOfIteration*38400);
				sumE_DPDCH4.set_length(numOfIteration*38400);
				
				for(int z=0;z<numOfIteration;z++)
				{
					sumHS_DPCCH[z]=path1HS_DPCCH(z)+path2HS_DPCCH(z)+path3HS_DPCCH(z)+path4HS_DPCCH(z)+path5HS_DPCCH(z)+path6HS_DPCCH(z);
					sumDPCCH[z]=path1DPCCH(z)+path2DPCCH(z)+path3DPCCH(z)+path4DPCCH(z)+path5DPCCH(z)+path6DPCCH(z);
					sumE_DPCCH[z]=path1E_DPCCH(z)+path2E_DPCCH(z)+path3E_DPCCH(z)+path4E_DPCCH(z)+path5E_DPCCH(z)+path6E_DPCCH(z);
					sumE_DPDCH1[z]=path1E_DPDCH1(z)+path2E_DPDCH1(z)+path3E_DPDCH1(z)+path4E_DPDCH1(z)+path5E_DPDCH1(z)+path6E_DPDCH1(z);
					sumE_DPDCH2[z]=path1E_DPDCH2(z)+path2E_DPDCH2(z)+path3E_DPDCH2(z)+path4E_DPDCH2(z)+path5E_DPDCH2(z)+path6E_DPDCH2(z);
					sumE_DPDCH3[z]=path1E_DPDCH3(z)+path2E_DPDCH3(z)+path3E_DPDCH3(z)+path4E_DPDCH3(z)+path5E_DPDCH3(z)+path6E_DPDCH3(z);
					sumE_DPDCH4[z]=path1E_DPDCH4(z)+path2E_DPDCH4(z)+path3E_DPDCH4(z)+path4E_DPDCH4(z)+path5E_DPDCH4(z)+path6E_DPDCH4(z);
				}
				vec HS_DPCCH,E_DPCCH,DPCCH,E_DPDCH1,E_DPDCH2,E_DPDCH3,E_DPDCH4;
				bvec bHS_DPCCH,bE_DPCCH,bDPCCH,bE_DPDCH1,bE_DPDCH2,bE_DPDCH3,bE_DPDCH4;
				E_DPCCH.set_length(150*numOfIteration);
				DPCCH.set_length(150*numOfIteration);
				HS_DPCCH.set_length(150*numOfIteration);
				E_DPDCH1.set_length(19200*numOfIteration);
				E_DPDCH2.set_length(19200*numOfIteration);
				E_DPDCH3.set_length(9600*numOfIteration);
				E_DPDCH4.set_length(9600*numOfIteration);

				bE_DPCCH.set_length(150*numOfIteration);
				bDPCCH.set_length(150*numOfIteration);
				bHS_DPCCH.set_length(150*numOfIteration);
				bE_DPDCH1.set_length(19200*numOfIteration);
				bE_DPDCH2.set_length(19200*numOfIteration);
				bE_DPDCH3.set_length(9600*numOfIteration);
				bE_DPDCH4.set_length(9600*numOfIteration);

				E_DPCCH.zeros();
				DPCCH.zeros();
				HS_DPCCH.zeros();
				E_DPDCH1.zeros();
				E_DPDCH2.zeros();
				E_DPDCH3.zeros();
				E_DPDCH4.zeros();

				for(int k=0;k<150*numOfIteration;k++)
				{
					for(int w=0;w<256;w++)
					{
						E_DPCCH[k]=E_DPCCH[k]+sumE_DPCCH[w];
						DPCCH[k]=DPCCH[k]+sumDPCCH[w];
						HS_DPCCH[k]=HS_DPCCH[k]+sumHS_DPCCH[w];
						
					}
				
					(E_DPCCH[k]>0) ? bE_DPCCH[k]=1 : bE_DPCCH[k]=0;
					(DPCCH[k]>0) ? bDPCCH[k]=1 : bDPCCH[k]=0;
					(HS_DPCCH[k]>0) ? bHS_DPCCH[k]=1 : bHS_DPCCH[k]=0;
				}
				for(int k=0;k<19200*numOfIteration;k++)
				{
					for(int w=0;w<2;w++)
					{
						E_DPDCH1[k]=E_DPDCH1[k]+sumE_DPDCH1[w];
						E_DPDCH2[k]=E_DPDCH2[k]+sumE_DPDCH2[w];
					}
					(E_DPDCH1[k]>0) ? bE_DPDCH1[k]=1 : bE_DPDCH1[k]=0;
					(E_DPDCH2[k]>0) ? bE_DPDCH2[k]=1 : bE_DPDCH2[k]=0;
				}

				for(int k=0;k<9600*numOfIteration;k++)
				{
					for(int w=0;w<2;w++)
					{
						E_DPDCH3[k]=E_DPDCH3[k]+sumE_DPDCH3[w];
						E_DPDCH4[k]=E_DPDCH4[k]+sumE_DPDCH4[w];
					}
					(E_DPDCH3[k]>0) ? bE_DPDCH3[k]=1 : bE_DPDCH3[k]=0;
					(E_DPDCH4[k]>0) ? bE_DPDCH4[k]=1 : bE_DPDCH4[k]=0;

				}



			brDPCCH.count(tr.DPCCHall,bDPCCH);
			brHS_DPCCH.count(tr.HS_DPCCHall,bHS_DPCCH);
			brE_DPCCH.count(tr.E_DPCCHall,bE_DPCCH);
			brE_DPDCH1.count(tr.E_DPDCH1all,bE_DPDCH1);
			brE_DPDCH2.count(tr.E_DPDCH2all,bE_DPDCH2);
			brE_DPDCH3.count(tr.E_DPDCH3all,bE_DPDCH3);
			brE_DPDCH4.count(tr.E_DPDCH4all,bE_DPDCH4);

			berDPCCH(i)=brDPCCH.get_errorrate();
			berHS_DPCCH(i)=brHS_DPCCH.get_errorrate();
			berE_DPCCH(i)=brE_DPCCH.get_errorrate();
			berE_DPDCH1(i)=brE_DPDCH1.get_errorrate();
			berE_DPDCH2(i)=brE_DPDCH2.get_errorrate();
			berE_DPDCH3(i)=brE_DPDCH3.get_errorrate();
			berE_DPDCH4(i)=brE_DPDCH4.get_errorrate();
				
	}
	//stats
	out.open("BER.csv", ios_base::out | ios_base::trunc);
	out<<"Eb/N0"<<';'<<"BER DPCCH"<<';'<<"BER HS_DPCCH"<<';'<<"BER E_DPCCH"<<';'<<"BER E_DPDCH1"<<';'
		<<"BER E_DPDCH2"<<';'<<"BER E_DPDCH3"<<';'<<"BER E_DPDCH4"<<endl;

	for(int i=0;i<EbN0dB.length();i++)
	{
	
		out<<EbN0dB(i)<<';'<<berDPCCH(i)<<';'<<berHS_DPCCH(i)<<';'<<berE_DPCCH(i)<<';'<<berE_DPDCH1(i)<<';'
			<<berE_DPDCH2(i)<<';'<<berE_DPDCH3(i)<<';'<<berE_DPDCH4(i)<<endl;
	}
	out.close();
	
	return 0;
}
