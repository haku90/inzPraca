#include "stdafx.h"
#include "itpp/itcomm.h"


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
	int SF_EDPDCH;

	int DPCCH_NBits;

	int DPCCH_SLOT0,DPCCH_SLOT1,DPCCH_SLOT2,DPCCH_SLOT3,DPCCH_SLOT4,DPCCH_SLOT5,DPCCH_SLOT6,DPCCH_SLOT7;
	int DPCCH_SLOT8,DPCCH_SLOT9,DPCCH_SLOT10,DPCCH_SLOT11,DPCCH_SLOT12,DPCCH_SLOT13,DPCCH_SLOT14;
	
	

	int HS_DPCCH_NBits;

	int E_DPDCH1_NData, E_DPDCH2_NData,E_DPDCH3_NData,E_DPDCH4_NData;
	int E_DPCCH_NBits;
	double betaDPCCH,betaE_DPDCH,betaE_DPCCH,betaE_HS_DPCCH;
	double betaDPCCHlin,betaE_DPDCHlin,betaE_DPCCHlin,betaE_HS_DPCCHlin;

	vec OVSF256_0,OVSF256_1,OVSF256_33,OVSF256_64,OVSF256_65,OVSF256_66,OVSF256_67;
	vec	OVSF4_1,OVSF4_2;

	bvec DPCCH;
	bvec HS_DPCCH;
	bvec E_DPDCH1,E_DPDCH2,E_DPDCH3,E_DPDCH4;
	bvec E_DPCCH;

	vec  SP_DPCCH;
	vec  SP_HS_DPCCH;
	vec	 SP_E_DPDCH1,SP_E_DPDCH2,SP_E_DPDCH3,SP_E_DPDCH4;
	vec  SP_E_DPCCH;


	cvec radioFrame;

	mGold gold;

	smat OVSF;
	smat OVSF4;
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
		SF_EDPDCH=p.get_int("SF_EDPDCH");
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
		betaE_DPDCH=p.get_int("betaE_DPDCH");
		betaE_DPCCH=p.get_int("betaE_DPCCH");
		betaE_HS_DPCCH=p.get_int("betaE_HS_DPCCH");
	
		betaDPCCHlin=pow10(betaDPCCH/10.0);
		betaE_DPDCHlin=pow10(betaE_DPDCH/10.0);
		betaE_DPCCHlin=pow10(betaE_DPCCH/10.0);
		betaE_HS_DPCCHlin=pow10(betaE_HS_DPCCH/10.0);

		OVSF=wcdma_spreading_codes (SF);
		OVSF4=wcdma_spreading_codes(SF_EDPDCH);

		OVSF256_0.set_length(SF,false);
		OVSF256_1.set_length(SF,false);
		OVSF256_33.set_length(SF,false);
		OVSF256_64.set_length(SF,false);
		OVSF256_65.set_length(SF,false);
		OVSF256_66.set_length(SF,false);
		OVSF256_67.set_length(SF,false);

		OVSF4_1.set_length(SF_EDPDCH,false);
		OVSF4_2.set_length(SF_EDPDCH,false);

		for(int i=0;i<OVSF.cols();i++)
		{
		OVSF256_0[i]=OVSF(0,i);
		OVSF256_1[i]=OVSF(1,i);
		OVSF256_33[i]=OVSF(33,i);
		OVSF256_64[i]=OVSF(64,i);
		OVSF256_65[i]=OVSF(65,i);
		OVSF256_66[i]=OVSF(66,i);
		OVSF256_67[i]=OVSF(67,i);
		}
		for(int i=0;i<OVSF4.cols();i++)
		{
			OVSF4_1[i]=OVSF4(1,i);
			OVSF4_2[i]=OVSF4(2,i);
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
	transmiter tr;
	AWGN_Channel awg;
	receiver rec;
	
	
	//DPCCH
	tr.SP_DPCCH=tr.spreading(tr.DPCCH,tr.OVSF256_0);
	//HS-DPCCH
	tr.HS_DPCCH=randb(tr.HS_DPCCH_NBits);
	tr.SP_HS_DPCCH=tr.spreading(tr.HS_DPCCH,tr.OVSF256_33);
	//E-DPCCH
	tr.E_DPCCH=randb(tr.E_DPCCH_NBits);
	tr.SP_E_DPCCH=tr.spreading(tr.E_DPCCH,tr.OVSF256_1);
	//E-DPDCH
	tr.E_DPDCH1=randb(tr.E_DPDCH1_NData);
	tr.E_DPDCH2=randb(tr.E_DPDCH2_NData);
	tr.E_DPDCH3=randb(tr.E_DPDCH3_NData);
	tr.E_DPDCH4=randb(tr.E_DPDCH4_NData);
	tr.SP_E_DPDCH1=tr.spreading(tr.E_DPDCH1,tr.OVSF256_64);
	tr.SP_E_DPDCH2=tr.spreading(tr.E_DPDCH2,tr.OVSF4_1);
	tr.SP_E_DPDCH3=tr.spreading(tr.E_DPDCH3,tr.OVSF4_1);
	tr.SP_E_DPDCH4=tr.spreading(tr.E_DPDCH4,tr.OVSF4_1);
	//beta

	tr.SP_DPCCH=tr.SP_DPCCH*tr.betaDPCCHlin;
	tr.SP_HS_DPCCH=tr.SP_HS_DPCCH*tr.betaE_HS_DPCCHlin;
	tr.SP_E_DPCCH=tr.SP_E_DPCCH*tr.betaE_DPCCHlin;
	tr.SP_E_DPDCH1=tr.SP_E_DPDCH1*tr.betaE_DPDCHlin;
	tr.SP_E_DPDCH2=tr.SP_E_DPDCH2*tr.betaE_DPDCHlin;
	tr.SP_E_DPDCH3=tr.SP_E_DPDCH3*tr.betaE_DPDCHlin;
	tr.SP_E_DPDCH4=tr.SP_E_DPDCH4*tr.betaE_DPDCHlin;	
	
	//radio frame
	tr.radioFrame.set_length(tr.gold.numG);
	for(int i=0;i<tr.gold.numG;i++)
	{
		tr.radioFrame[i]._Val[0]=tr.SP_E_DPCCH[i]+tr.SP_E_DPDCH1[i]+tr.SP_E_DPDCH3[i];
		tr.radioFrame[i]._Val[1]=tr.SP_DPCCH[i]+tr.SP_HS_DPCCH[i]+tr.SP_E_DPDCH2[i]+tr.SP_E_DPDCH4[i];
	
	}
	
	
	cvec radioFrameScrambling,radioFrameDescr,channel;
	radioFrameScrambling=tr.scrambling(tr.radioFrame,tr.gold.c);
	//kanal
	//awg.set_noise();
	channel=awg(radioFrameScrambling);
	
	//odbiornik
	radioFrameDescr=rec.descrambling(channel,tr.gold.cConjugate);
	rec.rRecive=real(radioFrameDescr);
	rec.iRecive=imag(radioFrameDescr);

	rec.DPCCH=rec.despreading(tr.DPCCH_NBits,tr.OVSF256_0,true);
	rec.E_DPCCH=rec.despreading(tr.E_DPCCH_NBits,tr.OVSF256_1,false);
	rec.HS_DPCCH=rec.despreading(tr.HS_DPCCH_NBits,tr.OVSF256_33,true);
	rec.E_DPDCH1=rec.despreading(tr.E_DPDCH1_NData,tr.OVSF256_64,false);

	rec.E_DPDCH2=rec.despreading(tr.E_DPDCH2_NData,tr.OVSF4_1,true);
	rec.E_DPDCH3=rec.despreading(tr.E_DPDCH3_NData,tr.OVSF4_1,false);
	rec.E_DPDCH4=rec.despreading(tr.E_DPDCH4_NData,tr.OVSF4_1,true);

	BERC br;
	br.count(tr.DPCCH,rec.DPCCH);
	cout<<"bledy DPCCH"<<endl;
	cout<<br.get_errors()<<endl;
	cout<<"BER DPCCH"<<endl;
	cout<<br.get_errorrate()<<endl;

	br.clear();
	br.count(tr.E_DPCCH,rec.E_DPCCH);
	cout<<"bledy E-DPCCH"<<endl;
	cout<<br.get_errors()<<endl;
	cout<<"BER E-DPCCH"<<endl;
	cout<<br.get_errorrate()<<endl;

	br.clear();
	br.count(tr.HS_DPCCH,rec.HS_DPCCH);
	cout<<"bledy HS-DPCCH"<<endl;
	cout<<br.get_errors()<<endl;
	cout<<"BER HS-DPCCH"<<endl;
	cout<<br.get_errorrate()<<endl;

	br.clear();
	br.count(tr.E_DPDCH1,rec.E_DPDCH1);
	cout<<"bledy E-DPDCH1"<<endl;
	cout<<br.get_errors()<<endl;
	cout<<"BER E-DPDCH1"<<endl;
	cout<<br.get_errorrate()<<endl;

	br.clear();
	br.count(tr.E_DPDCH2,rec.E_DPDCH2);
	cout<<"bledy E-DPDCH2"<<endl;
	cout<<br.get_errors()<<endl;
	cout<<"BER E-DPDCH2"<<endl;
	cout<<br.get_errorrate()<<endl;

	br.clear();
	br.count(tr.E_DPDCH3,rec.E_DPDCH3);
	cout<<"bledy E-DPDCH3"<<endl;
	cout<<br.get_errors()<<endl;
	cout<<"BER E-DPDCH3"<<endl;
	cout<<br.get_errorrate()<<endl;

	br.clear();
	br.count(tr.E_DPDCH4,rec.E_DPDCH4);
	cout<<"bledy E-DPDCH4"<<endl;
	cout<<br.get_errors()<<endl;
	cout<<"BER E-DPDCH4"<<endl;
	cout<<br.get_errorrate()<<endl;
	
	
	return 0;
}
