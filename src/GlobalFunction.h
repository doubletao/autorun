#pragma once
#include <string>
using namespace std;

class CGlobalFunction
{
public:
	static CString GetNewGUID();
	static HRESULT GuidToString(const GUID& guid, CString& str);
	static void GetPathAndName(const CString &strSrcFilePath, CString &strPath, CString &strName);//根据文件全名获取文件路径及文件名
	static CString GetUnDuplicateFileName(const CString &strSrcFilePath);//根据指定文件名获取一个不重名的文件名(通过添加(n)的形式来避免重名)
	static BOOL IsFileExists(const CString &strSrcFilePath);//判定文件是否存在
	static string ConverCStringToStdString(CString & cstrValue);//CString与string之间的转换
	static CString ConvertStdStringToCString(const string & strValue);//CString与string之间的转换
	static CString GetFilePath(CString strFullFileName);//获取文件路径
	static CString GetFileName(CString strFullFileName);//获取不带扩展名的文件名
	static CString GetFileFielName(CString strFullFileName);//获取文件夹名字
	static CString GetFileNameWithExt(CString strFullFileName);//获取带扩展名的文件名
	static CString GetFileNameExt(const CString strSrcFileName);//获取文件扩展名.
	static CString FMT(LPCTSTR lpszFormat, ...);//把不定长的参数格式化
	static BOOL MakeSureDirectoryExists(CString strFilePath, CString & strErrInfo);//确保文件路径存在，如果不存在则创建，返回true表示路径存在或重建成功
	static void LoadBmpFileToCbitmap(const CString & strFilePath, CBitmap & bm);//将指定BMP文件装载到指定Cbitmap
	static CString GetSmartFileSize(unsigned long long llFileSize);//获取文件大小的文本描述
	static unsigned long long GetFileSize(const CString & strFulePath);//获取指定文件大小
	static void SetBootStart(BOOL bStart);
	static CString GetCurPath();
	static CString GetNameOfComputer();
};
