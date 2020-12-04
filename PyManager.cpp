#include "pch.h"
#include "PyManager.h"

PyManager::PyManager() {
	m_strProgramName = "";
	m_strCondaPath = "";
	m_strEnvsName = "";
	m_strDefaultMudulePath = "";
	m_strPythonHomePath = "";
}

PyManager::~PyManager() {

}

void PyManager::SetPath(string strProgramName, string strCondaPath, string strEnvsName, string strDefaultModulePath) {
	m_strProgramName = strProgramName;
	m_strCondaPath = strCondaPath;
	m_strEnvsName = strEnvsName;
	m_strDefaultMudulePath = strDefaultModulePath;
	m_strPythonHomePath = strCondaPath;

	if (m_strCondaPath.substr(m_strCondaPath.size() - 1, 1) != "/") {
		m_strPythonHomePath += "/envs/";
	}
	else {
		m_strPythonHomePath += "envs/";
	}

	m_strPythonHomePath += strEnvsName;
}

void PyManager::SendSimpleQuery(int nRows, ...) {
	wchar_t szBufferText[2048] = { 0, };

	string strQuery;

	va_list vargs;
	va_start(vargs, nRows);

	for (int i = 0; i < nRows; i++) {
		strQuery = va_arg(vargs, string);

		if (strQuery.substr(strQuery.size() - 1, 1) != "\n") {
			strQuery += "\n";
		}

		PyRun_SimpleString(strQuery.c_str());
	}

	va_end(vargs);
}

int realsum(int count, ...)
{
	va_list ap;
	va_start(ap, count);

	int sum = 0;
	for (int i = 0; i < count; ++i) {
		int n = va_arg(ap, int);
		sum += n;
		printf("%d\n", n);
	}


	va_end(ap);

	return sum;
}

template<typename T, typename... Types>
bool PyManager::PyCallFunction(string strModuleName, string strFunction, string strModulePath, T szText, Types... args) {
	bool bRes = false;

	PyObject *pName, *pModule, *pFunc, *pValue , *pVariable;
	
	wchar_t* program = Py_DecodeLocale(m_strProgramName.c_str(), NULL);
	Py_SetProgramName(program);

	CString strHomePath = CString(m_strPythonHomePath.c_str());
	Py_SetPythonHome(strHomePath);

	Py_Initialize();

	string _strModulePath;
	if (strModulePath.empty()) {
		_strModulePath = m_strDefaultMudulePath;
	}
	else {
		_strModulePath = strModulePath;
	}

	CString strQuery = CString(_strModulePath.c_str());
	strQuery.Format(L"sys.path.append('%s') \n", strQuery);
	std::string stQuery = std::string(CT2CA(strQuery.operator LPCWSTR()));
	SendSimpleQuery(2, (string)"import sys \n" , (string)stQuery);

	pName = PyUnicode_FromString(strModuleName.c_str());
	pModule = PyImport_Import(pName);
	if (pModule != NULL) {
		pFunc = PyObject_GetAttrString(pModule, strFunction.c_str());
		if (pFunc && PyCallable_Check(pFunc)) 
		{
			if (szText == "") {
				pVariable = NULL;
			}else{
				pVariable = Py_BuildValue(szText, std::forward<Types>(args)...);
			}

			pValue = PyObject_CallObject(pFunc, pVariable);
			if (pValue != NULL)
			{
				//PyObject* ptemp = PyList_GetItem(pValue, 0);
				PyObject* objectsRepresentation = PyObject_Repr(pValue);
				PyObject* str = PyUnicode_AsEncodedString(objectsRepresentation, "utf-8", "~E~");

				const char* return_val = PyBytes_AsString(str);
				int nReturn = (int)strtod(return_val, NULL);

				printf("Return Value : %d", nReturn);
			}

			bRes = true;
		}
	}

	Py_Finalize();

	return bRes;
}

void PyManager::testfunction() {
	//PyCallFunction("testPy", "test", "", "ii", 0, 123);
	PyCallFunction("gan", "GAN", "ii", 1, 2);
}