
// Project2View.cpp : implementation of the CProject2View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Project2.h"
#endif

#include "Project2Doc.h"
#include "Project2View.h"
#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int hits=0;
enum ALGO{FIFO,LRU,OPT};
static int count=1;
static ALGO CurrentAlgo=FIFO;
static ALGO AlgoInUse=FIFO;
static bool ifset=FALSE;
static bool StrtFlg=FALSE;
// CProject2View

IMPLEMENT_DYNCREATE(CProject2View, CScrollView)

BEGIN_MESSAGE_MAP(CProject2View, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_ALGORITHM_FIFO, &CProject2View::OnAlgorithmFifo)
	ON_COMMAND(ID_ALGORITHM_LRU, &CProject2View::OnAlgorithmLru)
	ON_COMMAND(ID_ALGORITHM_OPT, &CProject2View::OnAlgorithmOpt)
	ON_UPDATE_COMMAND_UI(ID_ALGORITHM_FIFO, &CProject2View::OnUpdateAlgorithmFifo)
	ON_UPDATE_COMMAND_UI(ID_ALGORITHM_LRU, &CProject2View::OnUpdateAlgorithmLru)
	ON_UPDATE_COMMAND_UI(ID_ALGORITHM_OPT, &CProject2View::OnUpdateAlgorithmOpt)
	ON_COMMAND(ID_SETVALUES, &CProject2View::OnSetvalues)
	ON_COMMAND(ID_START, &CProject2View::OnStart)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CProject2View construction/destruction

CProject2View::CProject2View()
{
	m_pframes=NULL;
	m_pinputvalues=NULL;
	m_NOFrames=0;
	m_NOInputs=0;
	m_panimateclr=NULL;
	m_panimaterect=NULL;
// Overrides

}

CProject2View::~CProject2View()
{
}

BOOL CProject2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CProject2View drawing

void CProject2View::OnDraw(CDC* pDC)
{

	
	CProject2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CMemDC dcMem(*pDC, this);
    CDC& DC = dcMem.GetDC();
	
	CRect rect(0,0,10000,5000);
    DC.FillSolidRect(rect, GetSysColor(COLOR_WINDOW));

	DC.SetBkMode(TRANSPARENT);
	CBrush* pOldBrush=dynamic_cast<CBrush*>(DC.SelectStockObject(NULL_BRUSH));
	
 

	if (StrtFlg)
	{
		switch (AlgoInUse)
		{
		case FIFO:
			DC.DrawText(_T("FIFO"),CRect((m_pinputvalues+0)->m_rect.TopLeft().x,(m_pinputvalues+0)->m_rect.TopLeft().y-60,(m_pinputvalues+m_NOInputs-1)->m_rect.BottomRight().x,(m_pinputvalues+m_NOInputs-1)->m_rect.BottomRight().y),DT_LEFT);
			break;
		case LRU:
			DC.DrawText(_T("LRU"),CRect((m_pinputvalues+0)->m_rect.TopLeft().x,(m_pinputvalues+0)->m_rect.TopLeft().y-60,(m_pinputvalues+m_NOInputs-1)->m_rect.BottomRight().x,(m_pinputvalues+m_NOInputs-1)->m_rect.BottomRight().y),DT_LEFT);
			break;
		case OPT:
			DC.DrawText(_T("OPT"),CRect((m_pinputvalues+0)->m_rect.TopLeft().x,(m_pinputvalues+0)->m_rect.TopLeft().y-60,(m_pinputvalues+m_NOInputs-1)->m_rect.BottomRight().x,(m_pinputvalues+m_NOInputs-1)->m_rect.BottomRight().y),DT_LEFT);
			break;
		default:
			break;
		}
		

		DC.Rectangle(CRect((m_pinputvalues+0)->m_rect.TopLeft().x-20,(m_pinputvalues+0)->m_rect.TopLeft().y-30,(m_pinputvalues+m_NOInputs-1)->m_rect.BottomRight().x+20,(m_pinputvalues+m_NOInputs-1)->m_rect.BottomRight().y+30));
		DC.DrawText(_T("Reference String"),CRect((m_pinputvalues+0)->m_rect.TopLeft().x,(m_pinputvalues+0)->m_rect.TopLeft().y-30,(m_pinputvalues+m_NOInputs-1)->m_rect.BottomRight().x,(m_pinputvalues+m_NOInputs-1)->m_rect.BottomRight().y),DT_LEFT);
		DC.DrawText(_T("Page Frames"),CRect((m_pframes+0)->m_prect[m_NOFrames-1].TopLeft().x,(m_pframes+0)->m_prect[m_NOFrames-1].TopLeft().y+110,(m_pframes+m_NOInputs-1)->m_prect[m_NOFrames-1].BottomRight().x,(m_pframes+m_NOInputs-1)->m_prect[m_NOFrames-1].BottomRight().y+110),DT_LEFT);

		for (int i = 0; i < m_NOInputs; i++)
		{
			DC.Rectangle((m_pinputvalues+i)->m_rect);
			
			DC.DrawText((m_pinputvalues+i)->m_input,CRect((m_pinputvalues+i)->m_rect.TopLeft().x,(m_pinputvalues+i)->m_rect.TopLeft().y+10,(m_pinputvalues+i)->m_rect.BottomRight().x,(m_pinputvalues+i)->m_rect.BottomRight().y),DT_CENTER);

		}

		for(int i=0;i<count;i++)
		{
			for(int j=0;j<m_NOFrames;j++)
			{
				DC.Rectangle((m_pframes+i)->m_prect[j]);

				

				
				
				if(!((m_pframes+i)->m_pvalue[j]=='*') && !(i==j))
				{
					if((m_pinputvalues+i)->m_hit==FALSE)
					{
						if(!((m_pframes+i)->m_prect[j]==(m_pinputvalues+i)->m_frect))
						{
							DC.DrawText((m_pframes+i)->m_pvalue[j],CRect((m_pframes+i)->m_prect[j].TopLeft().x,(m_pframes+i)->m_prect[j].TopLeft().y+10,(m_pframes+i)->m_prect[j].BottomRight().x,(m_pframes+i)->m_prect[j].BottomRight().y),DT_CENTER);
						}
						else
						{
							if(!((m_pframes+i-1)->m_pvalue[j]=='*'))
								DC.DrawText((m_pframes+i-1)->m_pvalue[j],CRect((m_pframes+i)->m_prect[j].TopLeft().x,(m_pframes+i)->m_prect[j].TopLeft().y+10,(m_pframes+i)->m_prect[j].BottomRight().x,(m_pframes+i)->m_prect[j].BottomRight().y),DT_CENTER);
						}
					}
					else
					{

						DC.DrawText((m_pframes+i)->m_pvalue[j],CRect((m_pframes+i)->m_prect[j].TopLeft().x,(m_pframes+i)->m_prect[j].TopLeft().y+10,(m_pframes+i)->m_prect[j].BottomRight().x,(m_pframes+i)->m_prect[j].BottomRight().y),DT_CENTER);
					}
				}
			}

			if((m_pinputvalues+i)->m_animateclr==FALSE)
			{
				(m_pinputvalues+i)->m_animateclr=TRUE;
				m_animatectrl.AnimateGroup(2*i);
			}

			COLORREF clr;
			clr=RGB(GetRValue(m_panimateclr[i]),GetGValue(m_panimateclr[i]),GetBValue(m_panimateclr[i]));
			DC.FillSolidRect((m_pinputvalues+i)->m_rect,clr);
			
			if(RGB(GetRValue(m_panimateclr[count-1]),GetGValue(m_panimateclr[count-1]),GetBValue(m_panimateclr[count-1]))==RGB(0,255,0))
			{
				if( ((m_pinputvalues+i)->m_hit==FALSE) )
				{
					if((m_pinputvalues+i)->m_animatemove==FALSE)
					{
						(m_pinputvalues+i)->m_animatemove=TRUE;
						m_animatectrl.AnimateGroup(2*i+1);
					}
				}
			}

			CRect TRect;
			TRect=(CRect)m_panimaterect[i];

			if((m_pinputvalues+i)->m_animatemove==TRUE)
			{
				DC.FillSolidRect(TRect,RGB(0,255,0));

			}

			DC.Rectangle(TRect);

			DC.DrawText((m_pinputvalues+i)->m_input,CRect(TRect.TopLeft().x,TRect.TopLeft().y+10,TRect.BottomRight().x,TRect.BottomRight().y),DT_CENTER);

	

			for (int a = 0; a < m_NOInputs; a++)
			{
				DC.Rectangle((m_pinputvalues+a)->m_rect);
			
				DC.DrawText((m_pinputvalues+a)->m_input,CRect((m_pinputvalues+a)->m_rect.TopLeft().x,(m_pinputvalues+a)->m_rect.TopLeft().y+10,(m_pinputvalues+a)->m_rect.BottomRight().x,(m_pinputvalues+a)->m_rect.BottomRight().y),DT_CENTER);

			}

			if((m_pinputvalues+count-1)->m_frect==(CRect)m_panimaterect[count-1])
			{
				if(!(count==m_NOInputs))
				{
					count++;
				}
			}
	
				if( ((m_pinputvalues+i)->m_hit==TRUE) )
					DC.DrawText(_T("HIT"),CRect((m_pframes+i)->m_prect[m_NOFrames-1].TopLeft().x,(m_pframes+i)->m_prect[m_NOFrames-1].TopLeft().y+70,(m_pframes+i)->m_prect[m_NOFrames-1].BottomRight().x,(m_pframes+i)->m_prect[m_NOFrames-1].BottomRight().y+120),DT_CENTER);
			
			
			if( ((m_pinputvalues+count-1)->m_hit==TRUE) )
			{

				if(RGB(GetRValue(m_panimateclr[count-1]),GetGValue(m_panimateclr[count-1]),GetBValue(m_panimateclr[count-1]))==RGB(0,255,0))
					if(!(count==m_NOInputs))
					{
						count++;
					}
			}
		}


	}

	DC.SelectObject(pOldBrush);
	if(count==m_NOInputs)
	{
		if(hits==0)
		for(int i=0;i<m_NOInputs;i++)
		{
			if( ((m_pinputvalues+i)->m_hit==TRUE) )
			{
				hits++;
			}
		}

		CString str;
		str.Format(_T("Number Of Hits = %d"),hits);
		DC.DrawText(str,CRect((m_pframes+0)->m_prect[m_NOFrames-1].TopLeft().x,(m_pframes+0)->m_prect[m_NOFrames-1].TopLeft().y+150,(m_pframes+m_NOInputs-1)->m_prect[m_NOFrames-1].BottomRight().x,(m_pframes+m_NOInputs-1)->m_prect[m_NOFrames-1].BottomRight().y+150),DT_LEFT);
		str.Format(_T("Number Of Faults = %d"),m_NOInputs-hits);
		DC.DrawText(str,CRect((m_pframes+0)->m_prect[m_NOFrames-1].TopLeft().x,(m_pframes+0)->m_prect[m_NOFrames-1].TopLeft().y+170,(m_pframes+m_NOInputs-1)->m_prect[m_NOFrames-1].BottomRight().x,(m_pframes+m_NOInputs-1)->m_prect[m_NOFrames-1].BottomRight().y+170),DT_LEFT);
	}
	// TODO: add draw code for native data here
}


// CProject2View printing

BOOL CProject2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CProject2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CProject2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CProject2View diagnostics

#ifdef _DEBUG
void CProject2View::AssertValid() const
{
	CView::AssertValid();
}

void CProject2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CProject2Doc* CProject2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProject2Doc)));
	return (CProject2Doc*)m_pDocument;
}
#endif //_DEBUG


// CProject2View message handlers


void CProject2View::OnAlgorithmFifo()
{
	CurrentAlgo=FIFO;
}


void CProject2View::OnAlgorithmLru()
{
	CurrentAlgo=LRU;
}


void CProject2View::OnAlgorithmOpt()
{
	CurrentAlgo=OPT;
}


void CProject2View::OnUpdateAlgorithmFifo(CCmdUI *pCmdUI)
{
	if (CurrentAlgo==FIFO)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}


void CProject2View::OnUpdateAlgorithmLru(CCmdUI *pCmdUI)
{
	if (CurrentAlgo==LRU)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}


void CProject2View::OnUpdateAlgorithmOpt(CCmdUI *pCmdUI)
{
	if (CurrentAlgo==OPT)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}


void CProject2View::OnSetvalues()
{
	
	CInputDlg InputDlg;
	InputDlg.DoModal();

	int i=0;
	int j=0;
	

	while (TRUE)
	{
		if((InputDlg.m_inputstr[j]==(wchar_t)'.') || (InputDlg.m_inputstr[j]==(wchar_t)'\0'))
			break;

		if( (((InputDlg.m_inputstr[j]>47) &&  (InputDlg.m_inputstr[j]<58)) || ((InputDlg.m_inputstr[j]>64) &&  (InputDlg.m_inputstr[j]<91)) || ((InputDlg.m_inputstr[j]>96) &&  (InputDlg.m_inputstr[j]<123))) )
			i++;

		j++;
		
			
	}
	if(i==0 || InputDlg.m_NOFrames==0)
		{
			AfxMessageBox(_T("Input(s) Is(are) Empty or Incorrect\nPlease Input Values Again"),MB_OK);
			return;
		}
	
	m_NOInputs=i;
	m_NOFrames=InputDlg.m_NOFrames;
	m_pinputvalues=new CInputStr[m_NOInputs];

	count=1;
	hits=0;

	ifset=TRUE;

	i=0;
	j=0;

	while (TRUE)
	{
		if((InputDlg.m_inputstr[j]==(wchar_t)'.') || (InputDlg.m_inputstr[j]==(wchar_t)'\0'))
			break;

		if( (((InputDlg.m_inputstr[j]>47) &&  (InputDlg.m_inputstr[j]<58)) || ((InputDlg.m_inputstr[j]>64) &&  (InputDlg.m_inputstr[j]<91)) || ((InputDlg.m_inputstr[j]>96) &&  (InputDlg.m_inputstr[j]<123))) )
		{
			(m_pinputvalues+i)->m_input.Format(_T("%c"),InputDlg.m_inputstr[j]);
			i++;
		}

		j++;					
	}

	for(i=0;i<m_NOInputs;i++)
	{
		(m_pinputvalues+i)->m_rect=CRect(40+i*100,70,80+i*100,110);
	}

	for(i=0;i<m_NOInputs;i++)
	{
		(m_pinputvalues+i)->m_animateclr=FALSE;
		(m_pinputvalues+i)->m_animatemove=FALSE;
	}

	m_pframes=new CFrames[m_NOInputs];


	
	for(i=0;i<m_NOInputs;i++)
	{
		(m_pframes+i)->m_prect=new CRect[m_NOFrames];
		(m_pframes+i)->m_pvalue=new CString[m_NOFrames];
	}

	for(i=0;i<m_NOInputs;i++)
	{
		for(j=0;j<m_NOFrames;j++)
		{
			(m_pframes+i)->m_prect[j]=CRect(30+i*100,180+j*60,90+i*100,240+j*60);
			(m_pframes+i)->m_pvalue[j]='*';
		}
	}

	

	switch(CurrentAlgo)
	{
	case FIFO:
		{
			AlgoInUse=FIFO;
			int prvframe=0;
			int turn=0;
			for (i = 0; i < m_NOInputs; i++)
			{
				
				for(int k=0;k<prvframe;k++)
				{
					(m_pframes+i)->m_pvalue[k]=(m_pframes+(i-1))->m_pvalue[k];
				}

				for(j=0;j<m_NOFrames;j++)
				{
					if((m_pframes+i)->m_pvalue[j]==(m_pinputvalues+i)->m_input)
					{
						(m_pinputvalues+i)->m_hit=TRUE;
						break;
					}
				}

				if(i<m_NOFrames)
					prvframe++;

				bool eflag=FALSE;
				
				if(!((m_pinputvalues+i)->m_hit==TRUE))
				{
					for(j=0;j<m_NOFrames;j++)
					{
						if((m_pframes+i)->m_pvalue[j]=='*')
						{
							(m_pframes+i)->m_pvalue[j]=(m_pinputvalues+i)->m_input;
							(m_pinputvalues+i)->m_frect=(m_pframes+i)->m_prect[j];
							eflag=TRUE;
							break;
						}
					}
				}
				if(!eflag)
				{	
					if(!((m_pinputvalues+i)->m_hit==TRUE))
					{
						(m_pframes+i)->m_pvalue[turn]=(m_pinputvalues+i)->m_input;
						(m_pinputvalues+i)->m_frect=(m_pframes+i)->m_prect[turn];
						if(turn<m_NOFrames-1)
						{
							turn++;
						}
						else
						{
							turn=0;
						}
					}
				}
				

			}
			break;
		}
	case OPT:
		{
			AlgoInUse=OPT;
			int prvframe=0;
			for (i = 0; i < m_NOInputs; i++)
			{
				
				for(int k=0;k<prvframe;k++)
				{
					(m_pframes+i)->m_pvalue[k]=(m_pframes+(i-1))->m_pvalue[k];
				}

				for(j=0;j<m_NOFrames;j++)
				{
					if((m_pframes+i)->m_pvalue[j]==(m_pinputvalues+i)->m_input)
					{
						(m_pinputvalues+i)->m_hit=TRUE;
						break;
					}
				}

				if(i<m_NOFrames)
					prvframe++;

				bool eflag=FALSE;

				if(!((m_pinputvalues+i)->m_hit==TRUE))
				{
					for(j=0;j<m_NOFrames;j++)
					{
						if((m_pframes+i)->m_pvalue[j]=='*')
						{
							(m_pframes+i)->m_pvalue[j]=(m_pinputvalues+i)->m_input;
							(m_pinputvalues+i)->m_frect=(m_pframes+i)->m_prect[j];
							eflag=TRUE;
							break;
						}
					}
				}
				
				int temp[1][2]={0,0};
				if(!eflag)
				{
					

					for(j=m_NOFrames-1 ;j<=0; j--)
					{
						for(int k=i+1;k<m_NOInputs;k++)
						{
							if(!((m_pinputvalues+i)->m_hit==TRUE))
							{

								

								if((m_pframes+i)->m_pvalue[j]==(m_pinputvalues+k)->m_input)
								{
									if(temp[0][0]<k)
									{
										temp[0][0]=k;
										temp[0][1]=j;
									
									}
									break;
								}

								if(k==m_NOInputs-1)
									temp[0][1]=j;

							}


						}
					}

					if(!((m_pinputvalues+i)->m_hit==TRUE))
					{
						(m_pframes+i)->m_pvalue[temp[0][1]]=(m_pinputvalues+i)->m_input;
						(m_pinputvalues+i)->m_frect=(m_pframes+i)->m_prect[temp[0][1]];
					}
				}
			}
			break;
		}
	case LRU:
		{
			AlgoInUse=LRU;
			int prvframe=0;
			for (i = 0; i < m_NOInputs; i++)
			{
				
				for(int k=0;k<prvframe;k++)
				{
					(m_pframes+i)->m_pvalue[k]=(m_pframes+(i-1))->m_pvalue[k];
				}

				for(j=0;j<m_NOFrames;j++)
				{
					if((m_pframes+i)->m_pvalue[j]==(m_pinputvalues+i)->m_input)
					{
						(m_pinputvalues+i)->m_hit=TRUE;
						break;
					}
				}

				if(i<m_NOFrames)
					prvframe++;

				bool eflag=FALSE;

				if(!((m_pinputvalues+i)->m_hit==TRUE))
				{
					for(j=0;j<m_NOFrames;j++)
					{
						if((m_pframes+i)->m_pvalue[j]=='*')
						{
							(m_pframes+i)->m_pvalue[j]=(m_pinputvalues+i)->m_input;
							(m_pinputvalues+i)->m_frect=(m_pframes+i)->m_prect[j];
							eflag=TRUE;
							break;
						}
					}
				}
				
				int temp[1][2]={m_NOInputs,0};
				if(!eflag)
				{
					

					for(j=0 ;j < m_NOFrames; j++)
					{
						for(int k=i-1;k>=0;k--)
						{
							if(!((m_pinputvalues+i)->m_hit==TRUE))
							{
								if((m_pframes+i)->m_pvalue[j]==(m_pinputvalues+k)->m_input)
								{
									if(temp[0][0]>k)
									{
										temp[0][0]=k;
										temp[0][1]=j;
									
									}
									break;
								}

								
							}
						}
					}

					if(!((m_pinputvalues+i)->m_hit==TRUE))
					{
						(m_pframes+i)->m_pvalue[temp[0][1]]=(m_pinputvalues+i)->m_input;
						(m_pinputvalues+i)->m_frect=(m_pframes+i)->m_prect[temp[0][1]];
					}
				}
			}
			break;
		}
	default:
		;
	}

	m_panimaterect=new CAnimationRect[m_NOInputs];
	m_panimateclr=new CAnimationColor[m_NOInputs];

	m_animatectrl.SetRelatedWnd(this);
	m_animatectrl.EnableAnimationTimerEventHandler();

	for(i=0;i<m_NOInputs;i++)
	{
	m_panimateclr[i].SetID(-1,2*i);
	m_panimaterect[i].SetID(-1,2*i+1);


	m_animatectrl.AddAnimationObject(m_panimateclr+i);
	m_animatectrl.AddAnimationObject(m_panimaterect+i);
	}

	for(i=0;i<m_NOInputs;i++)
	{
		m_panimaterect[i]=(m_pinputvalues+i)->m_rect;
		m_panimateclr[i]=RGB(255,255,255);

		m_panimateclr[i].AddTransition(new CLinearTransition(0.2,0),new CLinearTransition(0.4,255),new CLinearTransition(0.6,0));
		
		if( ((m_pinputvalues+i)->m_hit==FALSE) )
		{
			m_panimaterect[i].AddTransition(new CLinearTransition(1.5,(m_pinputvalues+i)->m_frect.TopLeft().x),new CLinearTransition(1.5,(m_pinputvalues+i)->m_frect.TopLeft().y),new CLinearTransition(1.5,(m_pinputvalues+i)->m_frect.BottomRight().x),new CLinearTransition(1.5,(m_pinputvalues+i)->m_frect.BottomRight().y));
		}


	}

}


void CProject2View::OnStart()
{
	if(ifset==TRUE)
	{
		StrtFlg=TRUE;
		InvalidateRect(nullptr);
		UpdateWindow();
	}
	else
	{
		AfxMessageBox(_T("Please Set Values First"),MB_OK);
	}
}


BOOL CProject2View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CProject2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize DocSize(10000,5000);
	SetScrollSizes(MM_TEXT,DocSize,CSize(500,500),CSize(20,20));
	// TODO: Add your specialized code here and/or call the base class
}
