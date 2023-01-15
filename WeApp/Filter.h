
#pragma once

//
//*** �˲��� ***
//
class CFilter
{
public:
	int                                 m_nFilterWidth;  //�˲�����
	int                                m_nFilterLength;  //�˲�����
	double*                                  m_pFilter;  //�˲�������
	BOOL                                 m_bFilterType;  //��������-0,ż-1

public:
	CFilter();
	CFilter(int FilterWidth);
	CFilter(const CFilter &Filter);
	~CFilter();
	BOOL Create(int FilterWidth);
	BOOL Create(CFilter *Filter);
	BOOL Create(BYTE* dataSrc, int& ImageWidth, int& ImageHeight, int& ImageChannel, long& ImageRowlen, int curChannel, int curRow, int curCol, int curSize);
	BOOL Create(double* Data, int Width);
	BOOL Assign(BYTE* dataSrc, int& ImageWidth, int& ImageHeight, int& ImageChannel, long& ImageRowlen, int curChannel, int curRow, int curCol);
	double FindMax();
	double FindMin();
	double FindMed();
	double Multiply(CFilter* Filter);
	double& Visit(int x, int y);
};
