/*!
 * \file ChildFrm.h
 * \date 2023/01/15 11:36
 *
 * \author Hedong
 * \Contact: zenghedong@outlook.com
 *
 * \brief: ChildFrm.h : interface of the CChildFrame class
 *
 * \TODO: long description
 *
 * \note
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

