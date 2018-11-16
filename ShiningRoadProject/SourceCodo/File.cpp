#include "File.h"

//������𐔒l�ɂ��悤�Ƃ��Ȃ����̃`�F�b�N�Ŏg��.
#include <cctype>
#include <algorithm>

#include <sstream>
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;

namespace{

	const unsigned int uiRESERVE_SIZE_ROW = 64;//�x�N�^�[�̏����ő�T�C�Y( �s ).
	const unsigned int uiRESERVE_SIZE_COL = 32;//�x�N�^�[�̏����ő�T�C�Y( �s�̃J���}��؂�̐� ).

	const char cDELIMITER = ',';//��؂蕶��.
	
	const string sERROR = "THIS_IS_ERROR";//�G���[�p������.

	//�������p�f�[�^�쐬( �v�b�V���o�b�N�p ).
	const vector<string> vsINIT_LINE;

}

clsFILE::clsFILE() :
	m_bUsing( false )
{
}

clsFILE::~clsFILE()
{
	Close();
}



//----- �ǂݍ��݊֌W -----//.
bool clsFILE::Open( const string &sFileName )
{
	if( m_bUsing ){
		assert( !m_bUsing );//�g�p����open�����.
		return false;
	}

	m_sFileName = sFileName;

	//�������m��.
	m_DataSet.reserve( uiRESERVE_SIZE_ROW );

	//�t�@�C�����J��.
	ifstream ifs( m_sFileName );
	{
		//open�`�F�b�N.
		if( !ifs ){
			cout << "�t�@�C�����Ђ炯�Ȃ�������" << endl;
			cin.get();
			return false;
		}

		string sBuff;//���̃f�[�^.
		unsigned int uiIndexRow = 0;//���s�ڂ���\���ϐ�( index ).
		//��s�Ât�@�C�����ׂĂ�ǂݍ���.
		while( true )
		{
			//�t�@�C���̏I�[�܂ŗ�����I��.
			if( ifs.eof() ){
				break;
			}
			
			m_DataSet.push_back( vsINIT_LINE );					//��s���₷.
			m_DataSet[uiIndexRow].reserve( uiRESERVE_SIZE_COL );//�������m��.
			
			getline( ifs, sBuff );					//���̃f�[�^�Ɉ�s��˂�����.
			m_DataSet[uiIndexRow].push_back( "" );	//�{�f�[�^�ɂ�������������f�[�^��˂����ޗP�\�����.
			m_DataSet[uiIndexRow][0] = sBuff;		//�˂�����.
			m_DataSet[uiIndexRow] = Split( m_DataSet[uiIndexRow][0], cDELIMITER );//��s�Ƃ��ē˂����񂾃f�[�^����؂蕶������ɕ�������.

			//�]���ȃ��������|�C����.
			m_DataSet[uiIndexRow].shrink_to_fit();

			uiIndexRow ++;
		}
	}

	//�]���ȃ��������|�C����.
	m_DataSet.shrink_to_fit();

	m_bUsing = true;

	return true;
}


void clsFILE::Close()
{
	for( unsigned int i=0; i<m_DataSet.size(); i++ ){
		m_DataSet[i].clear();
		m_DataSet[i].shrink_to_fit();
	}
	m_DataSet.clear();
	m_DataSet.shrink_to_fit();

	m_sFileName = "";
	
	m_bUsing = false;
}



string clsFILE::GetDataString( const int iRow, const int iCol ) const
{
	if( !m_bUsing ){
		assert( m_bUsing );//�g�p������Ȃ���΃A�T�[�g.
		return sERROR;
	}
	//�͈͓�?.
	if( iRow >= static_cast<int>( m_DataSet.size() ) ){
		return sERROR;
	}
	for( unsigned int i=0; i<m_DataSet.size(); i++ ){
		if( iCol >= static_cast<int>( m_DataSet[i].size() ) ){
			return sERROR;
		}
	}
	//�}�C�i�X�͂͂���( ���G���[�l�ł͂Ȃ� ).
	if( iRow < 0 && iRow != MyFile::ERR_LINE ){	
		return sERROR;
	}
	if( iCol < 0 && iCol != MyFile::ERR_LINE ){	
		return sERROR;
	}

	//�S����Ԃ�.
	if( iRow == MyFile::ERR_LINE ){
		string sData;
		//�S���A��.
		for( unsigned int i=0; i<m_DataSet.size(); i++ ){
			//���s( �ŏ��͖��� ).
			if( i ){
				sData += '\n';
			}
			for( unsigned int j=0; j<m_DataSet[i].size(); j++ ){
				sData += m_DataSet[i][j];
//				sData += cDELIMITER;	//��؂蕶��.
			}
		}
		return sData;
	}

	//�w��s��Ԃ�.
	if( iCol == MyFile::ERR_LINE ){
		string sData;
		//��s�A��.
		for( unsigned int i=0; i<m_DataSet[iRow].size(); i++ ){
			sData += m_DataSet[iRow][i];
//			sData += cDELIMITER;
		}
		return sData;
	}

	//�w�蕔����Ԃ�.
	return m_DataSet[iRow][iCol];
}

int clsFILE::GetDataInt(
	const int iRow, 
	const int iCol ) const
{
	string sBuff = GetDataString( iRow, iCol );//������擾.	

	//�u���ׂĂ������v�ł͂Ȃ� || �G���[�Ȃ��.
	if( !all_of( sBuff.cbegin(), sBuff.cend(), isdigit ) ||
		sBuff == sERROR )
	{
		//�G���[��Ԃ�.
		return MyFile::ERR_LINE;
	}

	int iNum = stoi( sBuff );	//������𐔎��ɂ���.
	return iNum;
}

float clsFILE::GetDataFloat(
	const int iRow, 
	const int iCol ) const
{
	string sBuff = GetDataString( iRow, iCol );//������擾.	

	//�u���ׂĂ������v�ł͂Ȃ� || �G���[�Ȃ��.
	if( /*!all_of( sBuff.cbegin(), sBuff.cend(), isdigit ) ||*/
		sBuff == sERROR )
	{
		//�G���[��Ԃ�.
		return static_cast<float>( MyFile::ERR_LINE );
	}

	float fNum = stof( sBuff );	//������𐔎��ɂ���.
	return fNum;
}

double clsFILE::GetDataDouble(
	const int iRow, 
	const int iCol ) const
{
	string sBuff = GetDataString( iRow, iCol );//������擾.	

	//�u���ׂĂ������v�ł͂Ȃ� || �G���[�Ȃ��.
	if( /*!all_of( sBuff.cbegin(), sBuff.cend(), isdigit ) ||*/
		sBuff == sERROR )
	{
		//�G���[��Ԃ�.
		return static_cast<double>( MyFile::ERR_LINE );
	}

	double dNum = stod( sBuff );	//������𐔎��ɂ���.
	return dNum;
}


//���s���邩.
unsigned int clsFILE::GetSizeRow() const
{
	return m_DataSet.size();
}

//���̍s�͉��񂠂邩.
unsigned int clsFILE::GetSizeCol( unsigned int uiRow ) const
{
	//�͈͊O�͋����Ȃ�.
	if( uiRow >= m_DataSet.size() ) return 0;
	return m_DataSet[uiRow].size();
}


//�����񕪊�( �������̕�����𕪊����ĕԂ� )( �������͋�؂蕶�� ).
vector< string > clsFILE::Split( const string &sStr, const char cSep ) const
{
	vector< string > vsOut;
	stringstream ss( sStr );
	string sBuff;

	//���߂Ƀ�����������Ă���.
	vsOut.reserve( uiRESERVE_SIZE_COL );

	//�����ɂ�����������؂�Ƃ��ĕ�������.
	while( std::getline( ss, sBuff, cSep ) ){
		vsOut.push_back( sBuff );
	}

	//�]���ȃ��������|�C����.
	vsOut.shrink_to_fit();

	return vsOut;
}



//----- �f���o���֌W -----//.
//CSV�ɓf���o��.
bool clsFILE::OutPutCsv( const FILE_DATA &data ) const
{
	//�J��.
	ofstream ofs( m_sFileName );

	//�J����?.
	if( !ofs ){
		cout << "�Ђ炯�Ȃ�������" << endl;
		cin.get();
		return false;
	}

	//�o�͗p������쐬.
	string OutPut = ConcForOutPut( data );
	ofs << OutPut;//endl������ƁA���̃t�@�C���N���X�̎d�l��A�ǂݍ��ނƂ��ɂ������Ȃ��ƂɂȂ�( ���s���������̍s���o���Ă��܂� ).

	return true;
}

//�o�͗p������쐬.
//�A��.Concatenation : �A��
string clsFILE::ConcForOutPut( const FILE_DATA &data ) const
{
	string OutPut;
	for( unsigned int i=0; i<data.size(); i++ ){
		//�ŏ��͖���.
		if( i ){
			OutPut += "\n";//���s����.
		}

		//����for����CSV�t�@�C����s����A�����Ă���.
		for( unsigned int j=0; j<data[i].size(); j++ ){
			OutPut += data[i][j];//�A��.
			OutPut += cDELIMITER;//��؂蕶��.
		}
	}

	return OutPut;
}


//OutPutCsv�̈����̘g�Â���.
void clsFILE::CreateFileDataForOutPut( FILE_DATA &Outdata, const int iRow, const int iCol ) const
{
	//������.
	for( unsigned int i=0; i<Outdata.size(); i++ ){
		Outdata[i].clear();
		Outdata[i].shrink_to_fit();
	}
	Outdata.clear();
	Outdata.shrink_to_fit();


	Outdata.resize( iRow );
	for( unsigned int i=0; i<Outdata.size(); i++ ){
		Outdata[i].resize( iCol, "" );
	}

}
