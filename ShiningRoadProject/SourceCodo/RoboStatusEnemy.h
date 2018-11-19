#ifndef ENEMY_ROBO_STATE_H_
#define ENEMY_ROBO_STATE_H_


class clsFILE;
#include "RoboStatusBase.h"


class clsROBO_STATUS_ENEMY : public clsROBO_STATUS
{
public:
	//引数は何行目を読み込むかを表す.
	clsROBO_STATUS_ENEMY( const int iRow = 0 );
	~clsROBO_STATUS_ENEMY();

private:
	//外部からデータを読み込む.
	void LoadFileData( const char* sFilePath ) override;

	//パーツのステータスデータを読み込む.
	void LoadPartsData( std::vector< std::unique_ptr< clsFILE > >& PartsFile );

	//読み込んだ番号のデータを取得する.
	void AttachData( const std::vector< std::unique_ptr< clsFILE > >& PartsFile );

	//ReceiveLeg()等の関数に格納するためのデータを作り、吐き出す.
	std::vector<int> CreateDataForReceive( 
		const std::vector< std::unique_ptr< clsFILE > >& PartsFile,
		const int PartsType );

private:
	//csvデータの何行目を読み込むか.
	const int m_iFILE_ROW;

};

#endif//#ifndef ENEMY_ROBO_STATE_H_