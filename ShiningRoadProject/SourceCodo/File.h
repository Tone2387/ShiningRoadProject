#ifndef FILE_H_
#define FILE_H_
#include <iostream>
#include <Shlwapi.h>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include"MyMacro.h"

enum Extension
{
	Txt = 0,	//テキストファイル.
	Csv,		//Csvファイル.
	x,			//Xファイル.
	png,		//pngファイル.
	Error,		//エラー.
};
struct FileSet
{
	std::string					sPath;			//ファイルパス.
	Extension					en_Extension;	//拡張子判定用.
	std::vector<std::string>	sDataAry;		//データの配列.
	int							iLineMax;		//配列の最大数.
};
class clsFile
{
public:
	clsFile();
	~clsFile();

	//最初に通して.
	void Init(const char* fileName);

	//ファイル書き込み.
	void Write(char* FilePath_A, char* FilePath);

	//ファイル書き込み.
	void Write(char* FilePath, int NewLine = NULL);

	//ファイル書き込み.
	template<typename T>
	void Write(char* FilePath, std::vector<T> DataAry);

	//ファイル読込.
	void Read();

	//ファイルを開く.
	void Open(HWND hWnd){
		ShellExecute(hWnd, NULL, m_Data.sPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}

	//配列の最大数.
	int GetDataArrayNumMax(){
		return m_Data.iLineMax;
	}

	//拡張子がどれか.
	Extension GetExtension(){
		return m_Data.en_Extension;
	}

	//絶対パスを返すよ.
	std::string GetFilePath(){
		return m_Data.sPath;
	}

	//参照で返すよ.
	template<class T>
	void GetDataArray(T* Data, int Array_Num)
	{
		std::stringstream is;
		T tmp;
		is.str(m_Data.sDataAry[Array_Num]);
		is >> tmp;
		*Data = tmp;
		is.clear(std::stringstream::goodbit);
	}

	//値渡し.
	template<class T>
	std::vector<T> GetDataArray()
	{
		std::stringstream is;
		vector<T> tmp;
		for (size_t i = 0; i < m_Data.sDataAry.size(); i++)
		{
			is.str(m_Data.sDataAry[i]);
			tmp.push_back(is);
			is.clear(std::stringstream::goodbit);
		}
		return Tmp;
	}

	//intで値渡し.
	int iGetDataArray(int ArrayNum)
	{
		std::stringstream is;
		int tmp;
		is.str(m_Data.sDataAry[ArrayNum]);
		is >> tmp;
		is.clear(std::stringstream::goodbit);
		return tmp;
	}

private:

	//文字列置換.
	std::string ReplaceString
		(
		  std::string String1   //置き換え対象.
		, std::string String2   //置き換える内容.
		, std::string String3   //検索対象.
		, std::string String4 = "NULL"  //上の検索対象との間を消す.
		);								//なければNULL

	//指定の文字で文字列を区切る.
	template <typename List>
	void split(const std::string& s, const std::string& delim, List& result);

	//データの区分け.
	void FileDataSplit(std::string Data);

	//拡張子判断.
	void ExtensionJudgment();

	//ファイルのデータ.
	FileSet	m_Data;
};

#endif //#ifndef FILE_H_