#include "stdafx.h"
#include "UndoStack.h"

// ջ��ʼ��
CUndoStack::CUndoStack()
{
	m_nCapacity = MAX_UNDO_NUMBER;
	for (int i = 0; i<m_nCapacity; i++)
	{
		m_Array[i] = NULL;
	}
	m_nSizeOfStack = 0;
	m_nTopOfStack = 0;
	m_nBottomOfStack = 0;
}

// ��ջ�ݻ�
CUndoStack::~CUndoStack()
{
	while (m_nSizeOfStack > 0) Pop();
	m_nSizeOfStack = 0;
	m_nTopOfStack = 0;
	m_nBottomOfStack = 0;
}

// �ж�Ϊ��
int CUndoStack::IsEmpty()
{
	return m_nSizeOfStack == 0;
}

// �ж�Ϊ��
int CUndoStack::IsFull()
{
	return m_nSizeOfStack == m_nCapacity;
}

// ��ջ���������ǿ��Խ���
void CUndoStack::Push(ElementType X)
{
	m_nTopOfStack++;
	m_nSizeOfStack++;
	// ��ջδ��
	if (m_nSizeOfStack <= m_nCapacity)
	{
		if (m_nTopOfStack == m_nCapacity)
			m_nTopOfStack = 0;
		m_Array[m_nTopOfStack] = X;
	}
	else
	{
		m_nSizeOfStack = m_nCapacity;
		if (m_nTopOfStack == m_nCapacity)
			m_nTopOfStack = 0;
		ElementType TempCell = m_Array[m_nBottomOfStack];
		m_nBottomOfStack++;
		if (m_nBottomOfStack == m_nCapacity)
			m_nBottomOfStack = 0;
		m_Array[m_nTopOfStack] = X;
		SAFE_DELETE(TempCell);
	}
}

// ��ջ������ջ�����ڼ�С
void CUndoStack::Pop()
{
	if (IsEmpty())
		return;
	SAFE_DELETE(m_Array[m_nTopOfStack]);
	m_nTopOfStack--;
	if (m_nTopOfStack == -1)
		m_nTopOfStack = m_nCapacity-1;
	m_nSizeOfStack--;
}

// ȡ��ջ��Ԫ��, ջ��С
ElementType CUndoStack::Top()
{
	if(!IsEmpty())
	{
		ElementType top =  m_Array[m_nTopOfStack];
		m_nTopOfStack--;
		if (m_nTopOfStack == -1)
			m_nTopOfStack = m_nCapacity-1;
		m_nSizeOfStack--;
		return top;
	}
	return NULL;
}

// ����ջ��Ԫ��ָ��
ElementType CUndoStack::GetTop()
{
	return m_Array[m_nTopOfStack];
}

// ����ջ��Ԫ��
void CUndoStack::SetTop(ElementType X)
{
	m_Array[m_nTopOfStack] = X;
}