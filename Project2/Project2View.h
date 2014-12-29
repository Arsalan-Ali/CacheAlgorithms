
// Project2View.h : interface of the CProject2View class
//

#pragma once
#include "Frames.h"
#include "InputStr.h"
class CProject2View : public CScrollView
{
protected: // create from serialization only
	CProject2View();
	DECLARE_DYNCREATE(CProject2View)

// Attributes
public:
	CProject2Doc* GetDocument() const;

// Operations
public:
	CFrames* m_pframes;
	CInputStr* m_pinputvalues;
	int m_NOFrames;
	int m_NOInputs;
	CAnimationController m_animatectrl;
	CAnimationColor* m_panimateclr;
	CAnimationRect* m_panimaterect;
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CProject2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAlgorithmFifo();
	afx_msg void OnAlgorithmLru();
	afx_msg void OnAlgorithmOpt();
	afx_msg void OnUpdateAlgorithmFifo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAlgorithmLru(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAlgorithmOpt(CCmdUI *pCmdUI);
	afx_msg void OnSetvalues();
	afx_msg void OnStart();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in Project2View.cpp
inline CProject2Doc* CProject2View::GetDocument() const
   { return reinterpret_cast<CProject2Doc*>(m_pDocument); }
#endif

