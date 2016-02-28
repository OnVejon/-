
#include "funlib.h"







bool getFunName(char *funName,char* pathName)
{
	
	int len = strlen(pathName)-1;
	if (len < 0)
		return false;
	while (len>=0&& pathName[len]!='.')
	{
		len--;
	}
	if (len < 0)len=0;
	if (pathName[len] == '.')pathName[len++] = '\0';
	int i = 0;
	while (pathName[len] != '\0')
	{
		funName[i++] = pathName[len];
		pathName[len++] = '\0';
	}
	funName[i] = '\0';
	return true;
}

bool IsFile(char *filename)
{
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

bool writeLibHead(char *libName)
{
	fstream _file;
	_file.open(libName, ios::binary| ios::out);
	if (_file)
	{
		_file << "Function Server Libs v1.0\r\n\x1a" ;//"Func block";
		_file.close();
		return true;
	}
	return false;
	
}
bool catFile(char *foutfile,char *finfile)
{
	ifstream fin(finfile, ios::_Nocreate | ios::binary);
	if (!fin) {
		cout << "File open error!\n";
		return false;
	}
	ofstream fout(foutfile, ios::binary|ios::app);
	char c[1024];
	while (!fin.eof())
	{
		fin.read(c, 1024);
		fout.write(c, fin.gcount());
	}
	fin.close();
	fout.close();
	return true;
}


bool writeLibFun(char *libName,InFunBody funbody)
{
	fstream _file;
	_file.open(libName, ios::binary|ios::app);
	if (_file)
	{
		_file << "Func block" ;//;

		
		
		 char *c=(char *)&(funbody.Funsize);
		
		_file.write(c, FSIZELEN)  ;
			
		int i = 0,j=0;
		while (i < FUNNAMELEN)
		{
			if (funbody.FunName[i] != '\0'&&j == 0)
			{
				_file << funbody.FunName[i] ;
			}
			else
			{
				_file << '\0' ;
				j = 1;
			}
			i++;
		}
	
		_file.close();

		catFile(libName, funbody.FunFileName);
	}
	return true;
}



unsigned long getFileSize(const char *filename)
{
	fstream _file;
	_file.open(filename, ios::in | ios::binary);

	//获取文件大小
	_file.seekg(0, ios::end);

	int nFileLen = (int)_file.tellg();
	_file.close();
	return nFileLen;
}


bool IsFunforLib(char *funname, char *libName)
{
	OutFunBody *funbody = new OutFunBody[100];
	int len = ListFunforLib(funbody, libName);
	char tmpfname[FUNNAMELEN];
	int i = 0;
	while (i < len)
	{
		sprintf_s(tmpfname, "%s%c%s", funbody[i].PathName, '.', funbody[i].FunName);
		if (strcmp(funname, tmpfname) == 0)
		{
			delete[] funbody;
			return true;
		}
		i++;
	}
	delete[] funbody;
	return false;
}

int ListFunforLib(OutFunBody funbody[], char *libName)
{
	unsigned long filesize = getFileSize(libName);

	fstream _file;
	_file.open(libName, ios::in | ios::binary);
	unsigned long tmpsize = 0;

	_file.seekg(LIBHEADLEN, ios::beg);
	int i = 0;

	while (_file.tellg()<filesize)
	{
		funbody[i].begin = (int)_file.tellg() ;
		_file.seekg(FBHEADLEN, ios::cur);

		_file.read((char *)&tmpsize, FSIZELEN);
		

		

		
		_file.read(funbody[i].PathName, FUNNAMELEN);
		if (!getFunName(funbody[i].FunName, funbody[i].PathName))return i-1;

		_file.seekg(tmpsize -  FSIZELEN - FUNNAMELEN- FBHEADLEN, ios::cur);
		funbody[i].end = (int)_file.tellg();
		i++;
	}
	_file.close();


	return i;


}


bool addFun(char * libName, char * funName,char * funFileName)
{
	if (!IsFile(libName) )
	{
		writeLibHead(libName);
	}

	if (!IsFile(funFileName))
	{
		cout << "no dll file" << endl;
		return false;
	}

	if (IsFunforLib(funName, libName))
	{
		delFun(libName, funName);

	}

	InFunBody funbody;
	strcpy(funbody.FunName ,funName);
	strcpy(funbody.FunFileName,funFileName);
	funbody.Funsize = getFileSize(funFileName) + 255 + 14;
	writeLibFun(libName, funbody);

	return true;
}

bool reFunName(char *libName, char *funName, char * newFunName)
{
	OutFunBody *funbody = new OutFunBody[100];
	int len = ListFunforLib(funbody, libName);
	char tmpfname[FUNNAMELEN];

	int i = 0;
	while (i < len)
	{
		sprintf_s(tmpfname, "%s%c%s", funbody[i].PathName, '.', funbody[i].FunName);
		if (strcmp(funName, tmpfname) == 0)
		{
			fstream file;
			file.open(libName, ios::binary|ios::out|ios::in);
			if (!file)
			{
				return false;
			}
			char *tmp = new char[getFileSize(libName)];
			file.read(tmp, getFileSize(libName));
			int k = strlen(newFunName);
			for (int j = 0; j < 255; j++)
			{
				if (j < k)
				{
					tmp[funbody[i].begin + FBHEADLEN + FSIZELEN  + j] = newFunName[j];
				}
				else
				{
					tmp[funbody[i].begin + FBHEADLEN + FSIZELEN  + j] = '\0';
				}
				
			}
			file.seekp(0, ios::beg);
			file.write(tmp, file.gcount());
			file.close();
			delete[] tmp;
			delete[] funbody;
			return true;
		}
		i++;
	}
	delete[] funbody;
	return false;
}

bool showFun(char * libName)
{
	OutFunBody *funbody = new OutFunBody[100];
	int len = ListFunforLib(funbody, libName);
	int i = 0;
	printf("目录	函数名	开始地址	结束地址\n");
	while (i<len)
	{
		printf("%s	%s	%d	%d\n", funbody[i].PathName, funbody[i].FunName, funbody[i].begin, funbody[i].end);
		i++;
	}
	delete[] funbody;
	return true;
}

bool delFun(char * libName, char * funName)
{
	OutFunBody *funbody = new OutFunBody[100];
	int len = ListFunforLib(funbody, libName);
	char tmpfname[FUNNAMELEN],newLibfile[FUNNAMELEN];
	sprintf_s(newLibfile, "%s.bak", libName);
	
	int i = 0;
	while (i < len)
	{
		sprintf_s(tmpfname, "%s%c%s", funbody[i].PathName, '.', funbody[i].FunName);
		if (strcmp(funName, tmpfname) == 0)
		{
			if (IsFile(newLibfile))
			{
				remove(newLibfile);
			}
			rename(libName, newLibfile);
			
			fstream _outfile,_infile;
			_outfile.open(libName, ios::out | ios::binary);
			_infile.open(newLibfile, ios::in | ios::binary);
			char *tmpchar = new char[funbody[i].begin];
			_infile.read(tmpchar, funbody[i].begin);
			_outfile.write(tmpchar, _infile.gcount());

			_infile.seekg(funbody[i].end-funbody[i].begin, ios::cur);

			while (!_infile.eof())
			{
				_infile.read(tmpchar, funbody[i].begin);
				_outfile.write(tmpchar, _infile.gcount());
			}
			_outfile.close();
			_infile.close();
			delete[] tmpchar;
		}
		i++;
	}
	
	delete[] funbody;
	return true;
}

