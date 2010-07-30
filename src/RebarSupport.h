/**
 *	@file	RebarSupport.h
 *	@brief	ReBar �̃��b�N�p.
 */

#ifndef __REBARSUPPORT_H_
#define __REBARSUPPORT_H_



class CReBarSupport {
private:
	CReBarCtrl	m_Rebar;

public:
	CReBarSupport(HWND hRebar) : m_Rebar(hRebar) { }


	BOOL SetBandsLock(BOOL bLock)
	{
		ATLASSERT( ::IsWindow(m_Rebar.m_hWnd) );

		int 		  BandCount = m_Rebar.GetBandCount();
		REBARBANDINFO rbbi		= { 0 };

		for (int i = 0; i < BandCount; i++) {
			rbbi.cbSize = sizeof (REBARBANDINFO);
			rbbi.fMask	= RBBIM_STYLE;

			m_Rebar.GetBandInfo(i, &rbbi);

			if (bLock) {
				//RBBS_NOGRIPPER��ݒ肷��O��RBBS_GRIPPERALWAYS���K�v
				if ( !(rbbi.fStyle & RBBS_GRIPPERALWAYS) ) {
					rbbi.fStyle &= ~RBBS_NOGRIPPER;
					rbbi.fStyle |=	RBBS_GRIPPERALWAYS;
					m_Rebar.SetBandInfo(i, &rbbi);
				}

				rbbi.fStyle &= ~RBBS_GRIPPERALWAYS;
				rbbi.fStyle |=	RBBS_NOGRIPPER;
			} else {
				rbbi.fStyle &= ~RBBS_NOGRIPPER;
				rbbi.fStyle |=	RBBS_GRIPPERALWAYS;
			}

			m_Rebar.SetBandInfo(i, &rbbi);
		}

		return TRUE;
	}
};



#endif		//__REBARSUPPORT_H_
