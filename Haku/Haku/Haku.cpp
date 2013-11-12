#include "stdafx.h"
#include "itpp/itcomm.h"


using namespace itpp;
using namespace std;
class transmiter
{
public:
	int DPDCH_NData;

	int DPCCH_NPilot;
	int DPCCH_NTfci;
	int DPCCH_NFbi;
	int DPCCH_NTpc;

	int HS_DPCCH_NBits;

	int E_DPDCH_NData;
	int E_DPCCH_NBits;

	bvec DPDCH;
	bvec DPCCH;
	bvec HS_DPCCH;
	bvec E_DPDCH;
	bvec E_DPCCH;

	vec	 SP_DPDCH;
	vec  SP_HS_DPCCH;

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
		DPDCH_NData = p.get_int("DPDCH_NData");
		HS_DPCCH_NBits=p.get_int("HS_DPCCH_NBits");
		E_DPCCH_NBits=p.get_int("E_DPCCH_NBits");
		E_DPDCH_NData=p.get_int("E_DPDCH_NData");
	}
	~transmiter(){};
};
void disp_vec(vec data)
{
	for(int i=0;i<data.length();i++)
	{
		cout<<data[i]<<" ";
	}
			cout<<endl;
}
void disp_bvec(bvec data)
{
	for(int i=0;i<data.length();i++)
	{
		cout<<data[i]<<" ";
	}
	cout<<endl;
}
int _tmain(int argc, _TCHAR* argv[])
{
	transmiter tr;
	int SF=4;
	smat OVSF=wcdma_spreading_codes (SF);

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
	

	vec OVSF256_1;
	OVSF256_1.set_length(SF,false);
	vec OVSF256_2;
	OVSF256_2.set_length(SF,false);
	for(int i=0;i<OVSF.cols();i++)
	{
		OVSF256_1[i]=OVSF(1,i);
		OVSF256_2[i]=OVSF(2,i);
		
	}

	//DPDCH 
	//generowanie bitow
	cout<<"bity kanal DPDCH"<<endl;
	tr.DPDCH=randb(tr.DPDCH_NData);
	disp_bvec(tr.DPDCH);
	//rozpraszanie
	cout<<"rozproszone DPDCH"<<endl;
	tr.SP_DPDCH=tr.spreading(tr.DPDCH,OVSF256_1);
	disp_vec(tr.SP_DPDCH);
	//HS_DPCCH
	//generowanie bitow
	cout<<"bity kanal HS_DPCCH"<<endl;
	tr.HS_DPCCH=randb(tr.HS_DPCCH_NBits);
	disp_bvec(tr.HS_DPCCH);
	//rozpraszanie
	cout<<"rozproszone HS_DPCCH"<<endl;
	tr.SP_HS_DPCCH=tr.spreading(tr.HS_DPCCH,OVSF256_2);
	disp_vec(tr.SP_HS_DPCCH);
	
	
	
	return 0;
}
