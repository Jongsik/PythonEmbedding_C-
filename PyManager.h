#pragma once
#include <iostream>

#ifdef _DEBUG
#define _DEBUG_WAS_DEFINED 1
#undef _DEBUG
#endif

#include <Python.h>

#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG1

#endif

using namespace std;

class PyManager
{
public:
	PyManager();
	~PyManager();

public:
	void SetPath(string strProgram, string strCondaPath, string strEnvsName, string strDefaultModulePath);

private:
	string m_strProgramName;
	string m_strCondaPath;
	string m_strEnvsName;
	string m_strDefaultMudulePath;
	string m_strPythonHomePath;

public:
	void SendSimpleQuery(int nRows, ...);
	void testfunction();

private:
	template<typename T, typename... Types>
	bool PyCallFunction(string strModuleName, string strFunction, string strModulePath, T szText, Types... args);
};

