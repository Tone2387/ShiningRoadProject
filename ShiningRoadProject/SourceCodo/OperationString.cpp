#include "OperationString.h"

#define LOCAL_OF_THIS_PROJECT "japanese"
using namespace std;

namespace{


}

clsOPERATION_STRING::clsOPERATION_STRING()
{
	//���P�[���̎w��.
	setlocale( LC_ALL, LOCAL_OF_THIS_PROJECT );
}

clsOPERATION_STRING::~clsOPERATION_STRING()
{
}



//�����ƘA��.
//�A�� : consolidated.
string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const int num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};

string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const unsigned int num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};

string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const short num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};

string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const unsigned short num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};

string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const char num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};

string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const unsigned char num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};


//�}���`�o�C�g������( char* )���烏�C�h�o�C�g������( wchat_t* )���쐬.
//wchar_t�^�������new���Ă���̂ŁA�g�p��͕K��delete[]���邱��.
wchar_t* clsOPERATION_STRING::CreateWcharPtrFromCharPtr( const char *c ) const
{
	//�z��̃T�C�Y�m��.
	size_t NewSize = strlen( c ) + 1;	//�������ł���( null�����܂� ).
	wchar_t *wc = new wchar_t[ NewSize ];

	//char����wchar��.
	size_t ConvChars = 0;
	mbstowcs_s( &ConvChars, wc, NewSize, c, _TRUNCATE );

	//new�����ςȂ��Ȃ̂ł����Ǝg�p���delete[]���Ă�.
	return wc;
}

//���C�h�o�C�g������( wchat_t* )����}���`�o�C�g������( char* )���쐬.
//char�^�������new���Ă���̂ŁA�g�p��͕K��delete[]���邱��.
char* clsOPERATION_STRING::CreateCharPtrFromWcharPtr( const wchar_t *wc ) const
{
	//�z��̃T�C�Y�m��.
	size_t NewSize = wcslen( wc );	//�������ł���.
	NewSize *= sizeof( wchar_t );	//��������Ȃ��Ɓu���o�C�g���v�̒l�ɂȂ�Ȃ�.
	NewSize += 1;					//( null�����܂߂� ).
	char *c = new char[ NewSize ];

	//char����wchar��.
	size_t ConvChars = 0;
	wcstombs_s( &ConvChars, c, NewSize, wc, _TRUNCATE );
	
	//new�����ςȂ��Ȃ̂ł����Ǝg�p���delete[]���Ă�.
	return c;
}

//#ifdef HIYOSHI
const int clsOPERATION_STRING::GetNumDigit(const unsigned int iNum)
{
	int iResult = 0;//����.
	int iDigit = 1;//����.
	unsigned int iNumTmp = iNum;//����.

	const unsigned int iDecimal = 10;//�\�i��.

	while (iResult == 0)
	{
		//10�ȏォ?.
		if (iNumTmp < iDecimal)
		{
			//�����m��.
			iResult = iDigit;
		}

		else
		{
			//���𑝂₷.
			iNumTmp = iNumTmp / iDecimal;
			++iDigit;
		}
	}

	return iResult;
}
//endif