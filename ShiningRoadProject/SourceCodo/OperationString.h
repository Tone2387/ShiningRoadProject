#ifndef OPERATION_STRING_H_
#define OPERATION_STRING_H_

#include <string>
#include <sstream>

//�����񑀍�N���X.
class clsOPERATION_STRING
{
public:
	clsOPERATION_STRING();
	~clsOPERATION_STRING();


	//�����ƘA��.
	//�A�� : consolidated.
	//�e���v���[�g�̎󂯎��^���Œ肷��.
	std::string ConsolidatedNumber( const std::string& s, const int				num, const char cFillZeroNum = -1 ) const;
	std::string ConsolidatedNumber( const std::string& s, const unsigned int	num, const char cFillZeroNum = -1 ) const;
	std::string ConsolidatedNumber( const std::string& s, const short			num, const char cFillZeroNum = -1 ) const;
	std::string ConsolidatedNumber( const std::string& s, const unsigned short	num, const char cFillZeroNum = -1 ) const;
	std::string ConsolidatedNumber( const std::string& s, const char			num, const char cFillZeroNum = -1 ) const;
	std::string ConsolidatedNumber( const std::string& s, const unsigned char	num, const char cFillZeroNum = -1 ) const;


	//�}���`�o�C�g������( char* )���烏�C�h�o�C�g������( wchat_t* )���쐬.
	//wchar_t�^�������new���Ă���̂ŁA�g�p��͕K��delete[]���邱��.
	wchar_t* CreateWcharPtrFromCharPtr( const char *c ) const;

	//���C�h�o�C�g������( wchat_t* )����}���`�o�C�g������( char* )���쐬.
	//char�^�������new���Ă���̂ŁA�g�p��͕K��delete[]���邱��.
	char* CreateCharPtrFromWcharPtr( const wchar_t *wc ) const;

	//#ifdef Hiyoshi
	const int GetNumDigit(const unsigned int iNum);
	//#endif//#ifdef Hiyoshi

private:

	//�����ƘA��.
	//�A�� : consolidated.
	template< class T >//��O�����͉����ɂ����?�ɂ����ĉ�������\���B(����Ȃ�����0�Ŗ��߂�).
	std::string ConsolidatedNumberProduct( std::string s, const T num, const char cFillZeroNum ) const
	{
		ostringstream ss;
		int iTmpNum = static_cast<int>( num );

		//0���߂��s��Ȃ��Ȃ疳��.
		if( cFillZeroNum != -1 ){
			//�A�����鐔���̌���.
			int iDigit = to_string( iTmpNum ).length();

			//����Ȃ�����0�Ŗ��߂�.
			for( char i=0; i<cFillZeroNum - iDigit; i++ ){
				ss << 0;
			}
		}

		ss << iTmpNum;//�����𕶎����( int����Ȃ��Ǝ��̂��N���邳 ).

		s += ss.str();

		return s;
	};
};

#endif//#ifndef OPERATION_STRING_H_