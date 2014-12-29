#pragma once
class CInputStr
{
public:
	CInputStr(void);
	~CInputStr(void);
	CString m_input;
	CRect m_rect;
	CRect m_frect;
	bool m_hit;
	bool m_animateclr;
	bool m_animatemove;
};

