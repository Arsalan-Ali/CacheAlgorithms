#include "stdafx.h"
#include "InputStr.h"


CInputStr::CInputStr(void)
{
	m_input='\0';
	m_rect=CRect(0,0,0,0);
	m_frect=CRect(0,0,0,0);
	m_hit=FALSE;
	m_animateclr=FALSE;
	m_animatemove=FALSE;
}


CInputStr::~CInputStr(void)
{
}
