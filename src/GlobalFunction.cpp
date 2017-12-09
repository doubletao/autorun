#include "StdAfx.h"
#include "GlobalFunction.h"
#include "imagehlp.h"
#include <vector>
using namespace std;

#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "imagehlp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CGlobalFunction::GetNewGUID()
{
	GUID guid = GUID_NULL;
	::CoCreateGuid(&guid);
	CString strGUID;
	GuidToString( guid, strGUID );
	strGUID.MakeUpper();
	return strGUID;
}

HRESULT CGlobalFunction::GuidToString( const GUID& guid, CString& str )
{
	GUID guidNo = guid;
#ifdef UNICODE
	RPC_WSTR pUuidString;
#else
	BYTE* pUuidString;
#endif
	UuidToString(&guidNo, &pUuidString);
	str =(LPCTSTR)pUuidString;
	str.MakeUpper();

	if (RPC_S_OK == RpcStringFree(&pUuidString))
		return S_OK;

	return S_FALSE;
}

void CGlobalFunction::GetPathAndName( const CString &strSrcFilePath, CString &strPath, CString &strName )
{
	strPath.Empty();
	strName.Empty();

	int nPos = strSrcFilePath.ReverseFind(_T('\\'));
	if(nPos < 0)
	{
		//未发现反斜杠时，可能只有文件名而无路径，可以继续获取文件名
		strPath.Empty();
	}
	else
	{
		strPath = strSrcFilePath.Left(nPos);
		strName = strSrcFilePath.Right(strSrcFilePath.GetLength() - nPos - 1);
	}

	nPos = strName.ReverseFind(_T('.'));
	if(nPos < 0)
	{
		//未发现点号(.)，视为无后缀的文件名，不处理
	}
	else
	{
		strName = strName.Left(nPos);
	}
}

CString CGlobalFunction::GetUnDuplicateFileName( const CString &strSrcFilePath )
{
	CString strName = GetFileNameWithExt(strSrcFilePath);
	CString strPath = GetFilePath(strSrcFilePath);
	CString strPureName = GetFileName(strName);
	int nCount = 1;
	while(IsFileExists(strPath + _T("\\") + strName))
	{
		CString strExt = GetFileNameExt(strName);
		strName.Format(_T("%s(%d)%s"), strPureName, nCount++, strExt);
	}
	return strPath + _T("\\") + strName;
}

BOOL CGlobalFunction::IsFileExists( const CString &strSrcFilePath )
{
	if(strSrcFilePath.IsEmpty())
		return FALSE;

	return  (_taccess(strSrcFilePath, 00) == 0);
}

std::string CGlobalFunction::ConverCStringToStdString( CString & cstrValue )
{
	string strRet;
	strRet.clear();
	int nSize = ::WideCharToMultiByte(CP_UTF8, 0, cstrValue.GetBuffer(), -1, NULL, 0, NULL, NULL);
	char * szcSrc = new char[nSize];
	memset(szcSrc, 0, sizeof(char) * nSize);
	::WideCharToMultiByte(CP_UTF8, 0, cstrValue.GetBuffer(), -1, szcSrc, nSize, NULL, NULL);
	strRet = szcSrc;
	delete[] szcSrc;
	szcSrc = NULL;
	return strRet;
}

CString CGlobalFunction::ConvertStdStringToCString( const string & strValue )
{
	CString cstrRet;
	cstrRet.Empty();
	int nLength = ::MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, NULL,0);
	TCHAR * szText1 = new TCHAR[nLength+1];
	::MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, szText1, nLength+1);
	cstrRet = szText1;
	delete[] szText1;
	szText1 = NULL;
	return cstrRet;
}

CString CGlobalFunction::GetFileName( CString strFullFileName )
{
	CString strFileName, strFileTemp;
	int nLength = strFullFileName.GetLength();
	int nIndex = strFullFileName.ReverseFind('\\');
	strFileTemp = strFullFileName.Mid(nIndex + 1);
	strFileName = strFileTemp;
	int nPos = strFileTemp.ReverseFind(_T('.'));
	if (nPos >= 0 && nPos < strFileTemp.GetLength())
	{
		strFileName = strFileTemp.Mid(0, nPos);
	}
	return strFileName;
}

CString CGlobalFunction::GetFileNameExt( const CString strSrcFileName )
{
	int nPos = strSrcFileName.ReverseFind(_T('.'));
//	if(nPos < 0)
	{
		
		if (PathIsDirectory(strSrcFileName))
		{
			return _T("文件夹");
		}
		else
		{
			if (nPos < 0)
			{
				return _T("");
			}
			CString strExt = strSrcFileName.Right(strSrcFileName.GetLength() - nPos);
			return strExt;
		}

	}
	
}

CString CGlobalFunction::GetFileNameWithExt( CString strFullFileName )
{
	CString strFileName;
	int nLength = strFullFileName.GetLength();
	int nIndex = strFullFileName.ReverseFind('\\');
	strFileName = strFullFileName.Mid(nIndex + 1);

	return strFileName;
}

CString CGlobalFunction::FMT( LPCTSTR lpszFormat, ... )
{
	CString str;

	va_list argList;
	va_start(argList, lpszFormat);
	str.FormatV(lpszFormat, argList);
	va_end(argList);

	return str;
}

BOOL CGlobalFunction::MakeSureDirectoryExists( CString strFilePath, CString & strErrInfo )
{
	BOOL bRet = FALSE;
	CString strPath;
	CString strName;
	GetPathAndName(strFilePath, strPath, strName);

	if (strPath.IsEmpty())
	{
		//不允许出现无路径的情况，当前目录也需要有./
		return FALSE;
	}

	if (strPath == _T("."))
	{
		//如果路径是.号，认定是当前路径，可以直接返回成功
		return TRUE;
	}

	//如果文件夹不存在，则创建
	//如果是".\"打头，则截取掉".\"替换为当前路径
	if (strPath.Left(2) == _T(".\\"))
	{
		strPath = strPath.Mid(2);
		CString strCurName;
		GetModuleFileName(NULL, strCurName.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
		strCurName.ReleaseBuffer();
		CString strCurPath;
		CString strCurPurName;
		GetPathAndName(strCurName, strCurPath, strCurPurName);
		strCurPurName = strPath;
		strPath.Format(_T("%s\\%s"), strCurPath, strCurPurName);
	}
	//确保以'\'结尾以创建最后一个目录
	if (strPath.GetAt(strPath.GetLength() - 1) != _T('\\'))
	{
		strPath.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//存放每一层目录字符串
	CString strTemp;//一个临时变量,存放目录字符串
	//遍历要创建的字符串
	for (int i = 0;i < strPath.GetLength(); ++i)
	{
		if (strPath.GetAt(i) != _T('\\'))
		{
			//如果当前字符不是'\\'
			strTemp.AppendChar(strPath.GetAt(i));
		}
		else
		{
			//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(_T('\\'));
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++)
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bRet = CreateDirectory(*vIter, NULL) ? true : false;
		if (!bRet)
		{
			int nErr = GetLastError();
			if (nErr == 183)
			{
				//错误号183说明路径已经存在无需创建，这种情况不视为错误
				bRet = TRUE;
				continue;
			}
			else if (nErr == 5)
			{
				//错误号5说明拒绝访问，有可能是试图创建根目录导致的，不视为错误
				if (vIter == vPath.begin())
				{
					bRet = TRUE;
					continue;
				}
			}
			else
			{
				LPVOID lpMsgBuf;
				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL
					);

				// Display the string.
				strErrInfo = (LPTSTR)lpMsgBuf;

				// Free the buffer.
				LocalFree( lpMsgBuf );
				break;
			}
		}
	}

	return bRet;
}

void CGlobalFunction::LoadBmpFileToCbitmap( const CString & strFilePath, CBitmap & bm )
{
	HBITMAP hBmp;
	hBmp = (HBITMAP)::LoadImage(NULL, strFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	int nErr = GetLastError();
	bm.Attach(hBmp);
}

CString CGlobalFunction::GetSmartFileSize( unsigned long long llFileSize )
{
	CString strValue = _T("");
	double lfFileSize = (double)llFileSize;
	lfFileSize /= 1024;
	if (lfFileSize < 1024)
		strValue.Format(_T("%0.2f KB"), lfFileSize);
	else {
		lfFileSize /= 1024;
		if (lfFileSize < 1024)
			strValue.Format(_T("%0.2f MB"), lfFileSize);
		else {
			lfFileSize /= 1024;
			strValue.Format(_T("%0.2f GB"), lfFileSize);
		}
	}
	return strValue;
}

CString CGlobalFunction::GetFilePath( CString strFullFileName )
{
	CString strPath;
	strPath.Empty();

	int nPos = strFullFileName.ReverseFind(_T('\\'));
	if(nPos < 0)
		return strPath;

	strPath = strFullFileName.Left(nPos);

	return strPath;
}

unsigned long long CGlobalFunction::GetFileSize( const CString & strFulePath )
{
	unsigned long long nRet = 0;
	CFileStatus fileStatus;
	if (CFile::GetStatus(strFulePath, fileStatus))
	{
		nRet = fileStatus.m_size;
	}
	return nRet;
}

void CGlobalFunction::SetBootStart(BOOL bStart)
{
	// 写入注册表，开机自启动
	CString sPath;
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer();
	LPSTR lpPath = (LPSTR)(LPCTSTR)sPath;
	HKEY hkey;
	DWORD cbData = (sPath.GetLength() + 1) * sizeof(TCHAR);
	LPCWSTR strKeyName = _T("U-Gateway");
	///开机启动
	if(bStart)
	{
		if(
			ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\"),
			0,
			KEY_ALL_ACCESS | KEY_WOW64_64KEY,
			&hkey
			)
			)
		{
			AfxMessageBox(_T("启动项注册失败!"));
		}
		else
		{
			if(ERROR_SUCCESS != RegSetValueEx(hkey,
				strKeyName,
				0,
				REG_SZ,
				(CONST BYTE *)lpPath,
				cbData))
			{
				AfxMessageBox(_T("启动项注册失败!"));
			}
		}
		RegCloseKey(hkey);
	}
	//取消开机启动
	else
	{
		if(
			ERROR_SUCCESS!=RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\"),
			0,
			KEY_ALL_ACCESS | KEY_WOW64_64KEY,
			&hkey
			)
			)
		{
			AfxMessageBox(_T("程序启动项注册失败!"));
		}
		else
		{
			RegDeleteValue(hkey, strKeyName);
		}
	}
}

CString CGlobalFunction::GetCurPath()
{
	CString strCurName;
	GetModuleFileName(NULL, strCurName.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	strCurName.ReleaseBuffer();
	CString strCurPath;
	CString strCurPurName;
	GetPathAndName(strCurName, strCurPath, strCurPurName);
	return strCurPath;
}

CString CGlobalFunction::GetNameOfComputer()
{
	const int BUFFERSIZE = 80;
	DWORD  bufCharCount = BUFFERSIZE;
	TCHAR buffer[BUFFERSIZE];
	if(!::GetComputerName(buffer, &bufCharCount))
		return _T("Unknown");
	else
		return buffer;
}
