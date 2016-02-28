#include "runfun.h"
#include "funlib.h"

bool CheckTempdll(char *funname)
{
	
	int len = strlen(funname) - 1;
	if (len < 0)
		return false;

	char filename[255];
	sprintf_s(filename,"%s%s%s","temp\\",funname,".dll");
	
	fstream _file;
	_file.open(filename, ios::in);
	if (_file)
	{
		_file.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool CheckLibfun(char *funname)
{
	/*vector<OutFunBody> vOFBody;
	OutFunBody funbody;
	string libName("FuncSLibs.LIB");
		unsigned long filesize = getFileSize(libName.c_str());

		fstream _file;
		_file.open(libName, ios::in | ios::binary);
		unsigned long tmpsize = 0;

		_file.seekg(LIBHEADLEN, ios::beg);
		int i = 0;

		while (_file.tellg()<filesize)
		{
			funbody.begin = (int)_file.tellg();
			_file.seekg(FBHEADLEN, ios::cur);

			_file.read((char *)&tmpsize, FSIZELEN);





			_file.read(funbody.PathName, FUNNAMELEN);
			

			_file.seekg(tmpsize - FSIZELEN - FUNNAMELEN - FBHEADLEN, ios::cur);
			funbody.end = (int)_file.tellg();
			vOFBody.push_back(funbody);
		}
		_file.close();

*/
	return IsFunforLib(funname, "FuncSLibs.LIB");


	
}




string runFunDll(string funname, char *value)
{
	string filename= "temp\\"+ funname+".dll";
	
	HMODULE dll = LoadLibrary((LPCWSTR)filename.c_str());

	return filename;
}

void FuncSMode1(Listen ls, CallHead head, char *value)
{
	CallHead aswHead = {1,0,"\0"};
	//¸øasw¸³Öµ
	string tmpstr = runFunDll(head.pFuncName, value);
	aswHead.buflen = tmpstr.max_size;
	send(ls.m_Socket, (char *)&aswHead, sizeof(aswHead), 0);
	
	send(ls.m_Socket,tmpstr.c_str(),tmpstr.max_size,0)
}
void FuncSMode2(Listen ls, CallHead head, char *value)
{

}
void FuncSMode3(Listen ls, CallHead head, char *value)
{

}