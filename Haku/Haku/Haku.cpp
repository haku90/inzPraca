#include "stdafx.h"
#include "itpp/itcomm.h"


using namespace itpp;
using namespace std;
class transmiter
{
public:
	int SF;
	int DPDCH_NData;

	int DPCCH_NBits;

	int DPCCH_SLOT0,DPCCH_SLOT1,DPCCH_SLOT2,DPCCH_SLOT3,DPCCH_SLOT4,DPCCH_SLOT5,DPCCH_SLOT6,DPCCH_SLOT7;
	int DPCCH_SLOT8,DPCCH_SLOT9,DPCCH_SLOT10,DPCCH_SLOT11,DPCCH_SLOT12,DPCCH_SLOT13,DPCCH_SLOT14;
	
	

	int HS_DPCCH_NBits;

	int E_DPDCH_NData;
	int E_DPCCH_NBits;

	bvec DPDCH;
	bvec DPCCH;
	bvec HS_DPCCH;
	bvec E_DPDCH;
	bvec E_DPCCH;

	vec  SP_DPCCH;
	vec	 SP_DPDCH;
	vec  SP_HS_DPCCH;
	vec	 SP_E_DPDCH;
	vec  SP_E_DPCCH;

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

	transmiter()
	{
		Parser p(std::string("config.txt"));
		DPCCH_NBits=p.get_int("DPCCH_NBits");
		DPDCH_NData = p.get_int("DPDCH_NData");
		HS_DPCCH_NBits=p.get_int("HS_DPCCH_NBits");
		E_DPCCH_NBits=p.get_int("E_DPCCH_NBits");
		E_DPDCH_NData=p.get_int("E_DPDCH_NData");
		SF=p.get_int("SF");
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

		DPDCH=randb(DPDCH_NData);
		HS_DPCCH=randb(HS_DPCCH_NBits);

		
	}
	~transmiter(){};
};
class receiver
{
public:
	vec R_DPCCH;
	vec R_DPDCH;

};

int _tmain(int argc, _TCHAR* argv[])
{
	transmiter tr;

	smat OVSF=wcdma_spreading_codes (tr.SF);

	/*
	for(int i=0;i<OVSF.cols();i++)
	{
		for(int j=0;j<OVSF.rows();j++)
		{
		cout<<OVSF(i,j)<<" ";
		
		}
		cout<<endl;
	}
	*/
	
	vec OVSF256_0;
	OVSF256_0.set_length(tr.SF,false);

	vec OVSF256_1;
	OVSF256_1.set_length(tr.SF,false);
	vec OVSF256_2;
	OVSF256_2.set_length(tr.SF,false);
	vec OVSF256_3;
	OVSF256_3.set_length(tr.SF,false);
	vec OVSF256_4;
	OVSF256_4.set_length(tr.SF,false);
	vec OVSF256_5;
	OVSF256_5.set_length(tr.SF,false);
	vec OVSF256_64;
	OVSF256_64.set_length(tr.SF,false);
	

	for(int i=0;i<OVSF.cols();i++)
	{
		OVSF256_0[i]=OVSF(0,i);
		OVSF256_1[i]=OVSF(1,i);
		OVSF256_2[i]=OVSF(2,i);
		OVSF256_3[i]=OVSF(3,i);
		OVSF256_4[i]=OVSF(4,i);
		OVSF256_5[i]=OVSF(5,i);
		OVSF256_64[i]=OVSF(64,i);
		
	}

	
	cout<<"DPCCH cale"<<endl;
		for(int i=0;i<tr.DPCCH_NBits;i++)
		{
			if(i%10==0 &&i>0)
			{
				cout<<endl;
			}
			cout<<tr.DPCCH(i)<<" ";
			
		}
	
	//DPDCH 
	//generowanie bitow
	//tr.DPDCH=randb(tr.DPDCH_NData);
	/*
	cout<<"bity kanal DPDCH"<<endl;
	cout<<tr.DPDCH<<endl;
	*/
	//rozpraszanie
	//tr.SP_DPDCH=tr.spreading(tr.DPDCH,OVSF256_64);
	/*
	cout<<"rozproszone DPDCH"<<endl;
	cout<<tr.SP_DPDCH<<endl;
	*/
	//HS_DPCCH
	//generowanie bitow
	//tr.HS_DPCCH=randb(tr.HS_DPCCH_NBits);
	/*
	cout<<"bity kanal HS_DPCCH"<<endl;
	cout<<tr.HS_DPCCH<<endl;
	*/
	//rozpraszanie
	//tr.SP_HS_DPCCH=tr.spreading(tr.HS_DPCCH,OVSF256_2);
	/*
	cout<<"rozproszone HS_DPCCH"<<endl;
	cout<<tr.SP_HS_DPCCH<<endl;
	*/
	//E_DPDCH
	//generowanie bitow
	//tr.E_DPDCH=randb(tr.E_DPDCH_NData);
	/*
	cout<<"bity kanal E_DPDCH"<<endl;
	cout<<tr.E_DPDCH<<endl;
	*/
	//rozpraszanie
	//tr.SP_E_DPDCH=tr.spreading(tr.E_DPDCH,OVSF256_3);
	/*
	cout<<"bity kanal E_DPDCH"<<endl;
	cout<<tr.SP_E_DPDCH<<endl;
	*/

	//E_DPCCH
	//generowanie bitow
	//tr.E_DPCCH=randb(tr.E_DPCCH_NBits);
	/*
	cout<<"bity kanal E_DPDCH"<<endl;
	cout<<tr.E_DPCCH<<endl;
	*/
	//rozpraszanie
	//tr.SP_E_DPCCH=tr.spreading(tr.E_DPCCH,OVSF256_4);
	/*
	cout<<"rozproszone E_DPDCH"<<endl;
	cout<<tr.SP_E_DPCCH<<endl;
	*/
	//DPCCH
	//generowanie bitow
	
	/*
	cout<<"bity kanalu E_DPCCH"<<endl;
	cout<<tr.DPCCH<<endl;
	*/
	//rozpraszanie
	//tr.SP_DPCCH=tr.spreading(tr.DPCCH,OVSF256_0);
	/*
	cout<<"rozproszone"<<endl;
	cout<<tr.SP_DPCCH<<endl;
	*/


		return 0;
}
