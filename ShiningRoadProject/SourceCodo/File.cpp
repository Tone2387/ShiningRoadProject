#include"File.h"

//Using宣言.
using namespace std;

//ファイル読込み関数.
clsFile::clsFile()
{
	m_Data.en_Extension = Error;
	m_Data.iLineMax		= NULL;
}

clsFile::~clsFile()
{
	m_Data.sDataAry.clear();
	m_Data.sDataAry.shrink_to_fit();
}

//ファイル書き込み.
void clsFile::Write(char* FilePath, char* Content){
	ofstream ofs(FilePath);
	if (!ofs) {
		cerr << "ファイルオープンに失敗" << endl;
		exit(1);
	}
	ofs << Content << "\n" << endl;
}

//ファイル書き込み.
template<typename T>
void clsFile::Write(char* FilePath, vector<T> DataAry)
{
	ofstream ofs(FilePath);
	if (!ofs) {
		cerr << "ファイルオープンに失敗" << endl;
		exit(1);
	}
	for (size_t i = 0; i < DataAry.size(); i++)
	{
		ofs << DataAry[i] << "," << endl;
	}
}

//ファイル書き込み.
void clsFile::Write(char* FilePath, int NewLine){
	ofstream ofs(FilePath);
	if (!ofs) {
		cerr << "ファイルオープンに失敗" << endl;
		exit(1);
	}
	for (size_t i = 0; i < m_Data.sDataAry.size(); i++)
	{
		if ((1 + i) % NewLine == 0)
		{
			ofs << m_Data.sDataAry[i] << "," << endl;
		}
		else{
			ofs << m_Data.sDataAry[i] << ",";
		}
	}
}

void clsFile::Read()
{
	ifstream ifs(m_Data.sPath);

	//ファイル読込前に前のデータを削除.
	if (m_Data.sDataAry.empty())
	{
		m_Data.sDataAry.clear();
		m_Data.sDataAry.shrink_to_fit();
	}
	if (ifs)
	{
		//データを削除
		string buf((istreambuf_iterator<char>(ifs)),
					istreambuf_iterator<char>());
		switch (m_Data.en_Extension)
		{
			case Txt:
			case Csv:
			FileDataSplit(buf);
			//データの最大数
			m_Data.iLineMax = m_Data.sDataAry.size();
			break;
		default:
			//読めないファイル.
			break;
		}
	}
}

//データの区分け.
void clsFile::FileDataSplit(string Data)
{
	vector<string> sTmpArray;
	//余計なコメントや改行等を削除してカンマに.
	while (Data.find('[') != Data.npos){
		Data = ReplaceString(Data, "", "[", "]");
	}
	while (Data.find('\t') != Data.npos){
		Data = ReplaceString(Data, ",", "\t");
	}
	while (Data.find('\n') != Data.npos){
		Data = ReplaceString(Data, ",", "\n");
	}
	while (Data.find('\"') != Data.npos){
		Data = ReplaceString(Data, ",", "\"");
	}

	//文字列区分け.
	split(Data, ",", sTmpArray);

	//NULLチェックして中身がなければ削除.
	for (int i = sTmpArray.size() - 1; i >= 0; i--)
	{
		if (sTmpArray[i].empty())
		{
			sTmpArray.erase(sTmpArray.begin() + i);
		}
	}
	sTmpArray.swap(m_Data.sDataAry);
}

//拡張子判断.
void clsFile::ExtensionJudgment()
{
	size_t PathSize = NULL;
	string tmp;
	PathSize = m_Data.sPath.find(".");
	tmp = m_Data.sPath.substr(PathSize + 1, m_Data.sPath.size());

	if (tmp == "txt" || tmp == "TXT"){
		m_Data.en_Extension = Txt;
	}
	else if (tmp == "csv" || tmp == "CSV"){
		m_Data.en_Extension = Csv;
	}
	else if (tmp == "x" || tmp == "X"){
		m_Data.en_Extension = x;
	}
	else if (tmp == "png" || tmp == "PNG"){
		m_Data.en_Extension = png;
	}
	else{
		//エラー（対応していない）
		m_Data.en_Extension =  Error;
	}
}

//文字列置換.
string clsFile::ReplaceString
(
  string String1   // 置き換え対象.
, string String2   // 置き換える内容.
, string String3   // 検索対象.
, string String4   // 上の検索対象との間を消す.
)
{
	if (String4 == "NULL"){
		string::size_type  Pos(String1.find(String3));

		while (Pos != string::npos)
		{
			String1.replace(Pos, String3.length(), String2);
			Pos = String1.find(String3, Pos + String2.length());
		}
	}
	else
	{
		string::size_type  Pos1(String1.find(String3));

		string::size_type  Pos2(String1.find(String4));

		string::size_type  Pos3(String1.find(String4) - String1.find(String3));

		int tmp1;
		int tmp2;
		int tmp3;

		tmp1 = String1.find(String3);
		tmp2 = String1.find(String4);
		tmp3 = tmp2 - tmp1 + 1;
		String1.replace(tmp1, tmp3, String2);
	}
	return String1;
}

template <typename List>
void clsFile::split(const string& s, const string& delim, List& result)
{
	result.clear();

	int SizeMax;
	SizeMax = 0;

	for (unsigned int j = 0; j < s.size(); j++)
	{
		if (s.compare(j, 1, delim) == 0)
		{
			SizeMax++;
		}
	}

	//最大数+1でちょうどよい.
	result.reserve(SizeMax + 1);

	using string = string;
	string::size_type pos = 0;

	while (pos != string::npos)
	{
		string::size_type p = s.find(delim, pos);
		if (p == string::npos)
		{
			result.push_back(s.substr(pos));
			break;
		}
		else {
			result.push_back(s.substr(pos, p - pos));
		}
		pos = p + delim.size();
	}
}

void clsFile::Init(const char* fileName)
{
	m_Data.sPath = fileName;
	ExtensionJudgment();
}