
#include "stdafx.h"
#include "Filter.h"

CFilter::CFilter()
{
	m_nFilterWidth = 3;
	m_nFilterLength = 9;
	m_bFilterType = 0;
	m_pFilter = NULL;
}

CFilter::CFilter(int FilterWidth)
{
	m_nFilterWidth = FilterWidth;
	m_nFilterLength = FilterWidth*FilterWidth;
	m_bFilterType = (FilterWidth+1)%2;
	m_pFilter = new double[m_nFilterLength];
}

CFilter::CFilter(const CFilter &Filter)
{
	m_nFilterWidth = Filter.m_nFilterWidth;
	m_nFilterLength = m_nFilterWidth*m_nFilterWidth;
	m_bFilterType = (m_nFilterWidth+1)%2;
	SAFE_DELETE(m_pFilter);
	m_pFilter = new double[m_nFilterLength];
	for (int i = 0; i<m_nFilterLength; i++)
		m_pFilter[i] = Filter.m_pFilter[i];
}

CFilter::~CFilter()
{
	SAFE_DELETE(m_pFilter);
}

//����һ������ΪFilterWidth���˲���
BOOL CFilter::Create(int FilterWidth)
{
	m_nFilterWidth = FilterWidth;
	m_nFilterLength = FilterWidth*FilterWidth;
	m_bFilterType = (FilterWidth+1)%2;
	SAFE_DELETE(m_pFilter);
	m_pFilter = new double[m_nFilterLength];
	return TRUE;
}

BOOL CFilter::Create(CFilter *Filter)
{
	m_nFilterWidth = Filter->m_nFilterWidth;
	m_nFilterLength = m_nFilterWidth*m_nFilterWidth;
	m_bFilterType = (m_nFilterWidth+1)%2;
	SAFE_DELETE(m_pFilter);
	m_pFilter = new double[m_nFilterLength];
	for (int i = 0; i<m_nFilterLength; i++)
		m_pFilter[i] = Filter->m_pFilter[i];
	return TRUE;
}

// ��ͼ�񴴽��˲�����dataSrc��ͼ������,2-5��ͼ����Ϣ,curChannel����ǰͨ��,curRow����ǰ���ĵ��������,curCol����ǰ���ĵ��������,curSize���˲�����С
BOOL CFilter::Create(BYTE* dataSrc, int& ImageWidth, int& ImageHeight, int& ImageChannel, long& ImageRowlen, int curChannel, int curRow, int curCol, int curSize)
{
	m_nFilterWidth = curSize;
	m_nFilterLength = curSize*curSize;
	m_bFilterType = (curSize+1)%2;
	SAFE_DELETE(m_pFilter);
	m_pFilter = new double[m_nFilterLength];

	int R = curSize/2;
	for(int k = 0, y = curRow-R+m_bFilterType; y<=curRow+R; y++)
	{
		for(int x = curCol-R+m_bFilterType; x<=curCol+R; x++)
		{
			if (0<=x && x<ImageWidth && 0<=y && y<ImageHeight)
			{
				BYTE* pixel = dataSrc + x*ImageChannel + y*ImageRowlen;
				m_pFilter[k] = *(pixel+curChannel);
			}
			else
			{
				m_pFilter[k] = 0;
			}
			k++;
		}
	}
	return TRUE;
}

// ��һά���鴴��һ���˲���
BOOL CFilter::Create(double* Data, int Width)
{
	m_nFilterWidth = Width;
	m_nFilterLength = Width*Width;
	m_bFilterType = (Width+1)%2;
	SAFE_DELETE(m_pFilter);
	m_pFilter = new double[m_nFilterLength];
	for (int i = 0; i<m_nFilterLength; i++)
		m_pFilter[i] = Data[i];
	return TRUE;
}

// ��ͼ���޸��˲�����dataSrc��ͼ������,2-5��ͼ����Ϣ,curChannel����ǰͨ��,curRow����ǰ���ĵ��������,curCol����ǰ���ĵ��������
BOOL CFilter::Assign(BYTE* dataSrc, int& ImageWidth, int& ImageHeight, int& ImageChannel, long& ImageRowlen, int curChannel, int curRow, int curCol)
{
	int R = m_nFilterWidth/2;
	for(int s = 0, y = curRow-R+m_bFilterType; y<=curRow+R; y++, s++)
	{
		for(int t = 0, x = curCol-R+m_bFilterType; x<=curCol+R; x++, t++)
		{
			if (0<=x && x<ImageWidth && 0<=y && y<ImageHeight)
			{
				BYTE* pixel = dataSrc + x*ImageChannel + y*ImageRowlen;
				Visit(t, s) = *(pixel+curChannel);
			}
			else
			{
				Visit(t, s) = 0;
			}
		}
	}
	return TRUE;
}

//��ȡ�˲������Ԫ
double CFilter::FindMax()
{
	double MAX = -1;
	for (int i = 0; i<m_nFilterLength; i++)
	{
		if (m_pFilter[i]>MAX)
		{
			MAX = m_pFilter[i];
		}
	}
	return MAX;
}

//��ȡ�˲�����СԪ
double CFilter::FindMin()
{
	double MIN = 256;
	for (int i = 0; i<m_nFilterLength; i++)
	{
		if (m_pFilter[i]<MIN)
		{
			MIN = m_pFilter[i];
		}
	}
	return MIN;
}

//��ȡ�˲�����ֵ
double CFilter::FindMed()
{
	double* Temp = new double[m_nFilterLength], SWAP;
	for (int i = 0; i<m_nFilterLength; i++)
	{//����һ��
		Temp[i] = m_pFilter[i];
	}
	//�Կ������������Ի�ȡ��ֵ
	for (int i = 0; i<m_nFilterLength; i++)
	{
		for (int j = i+1; j<m_nFilterLength; j++)
		{
			if (Temp[j]<Temp[i])
			{
				SWAP = Temp[j];
				Temp[j] = Temp[i];
				Temp[i] = SWAP;
			}
		}
	}
	double MED1 = Temp[m_nFilterLength/2-m_bFilterType];
	double MED2 = Temp[m_nFilterLength/2];
	delete [] Temp;
	return (MED1+MED2)/2;
}

// ���ؿ����
double CFilter::Multiply(CFilter* Filter)
{
	double Result = 0;
	for (int i = 0; i<m_nFilterLength; i++)
		Result += m_pFilter[i] * Filter->m_pFilter[i];
	return Result;
}

//��ȡ�˲���x��y�е�ֵ
double& CFilter::Visit(int x, int y)
{
	return m_pFilter[x + y*m_nFilterWidth];
}
