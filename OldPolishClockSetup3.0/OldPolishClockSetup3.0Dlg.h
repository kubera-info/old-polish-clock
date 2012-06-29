
// OldPolishClockSetup3.0Dlg.h : header file
//

#pragma once
#include <Shobjidl.h>
#include "afxwin.h"
#include "Dict.h"

enum SETUP_STAGE
{
	SETUP_LANGUAGE_SEL,
	SETUP_INFO,
	SETUP_LICENCE,
	SETUP_INSTALL,
	SETUP_FINISH,
	SETUP_QUESTION
};

void SetErrorCommentExternal( const std::wstring & pComment );

// COldPolishClockSetup30Dlg dialog
class COldPolishClockSetup30Dlg : public CDialog
{
	// Construction
public:
	COldPolishClockSetup30Dlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_OLDPOLISHCLOCKSETUP30_DIALOG };
	void SetLanguageFrame( const std::wstring & pFrameLabel );
	void LoadRTF( const std::wstring & pFilename, CRichEditCtrl & pRtf2 );
	void ErrorComment( const std::wstring & pComment );
	void SetDescription( const std::wstring & pComment );
	void SetOKCancel();
	void SetYesNo();
	void SetProgressState( const TBPFLAG pFlag );
	void SetRadio( const std::wstring & p_szFirst, const std::wstring & p_szSecond, const std::wstring & p_szThird );

	// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnSetCursor(
		CWnd* pWnd,
		UINT nHitTest,
		UINT message 
		);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void OnI18N();
	void OnI18NEx( MONAD_DEF_LANG pLang );

public:
	afx_msg void OnBnClickedRadioPol();
	afx_msg void OnBnClickedRadioRus();
	afx_msg void OnBnClickedRadioEng();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnThreadDone( WPARAM, LPARAM );
	afx_msg LRESULT OnErrorMessage( WPARAM, LPARAM );
	afx_msg LRESULT OnTaskbarBtnCreated ( WPARAM wParam, LPARAM lParam );

private:
	void DisableTaskbar();
	void HideAllCtrls();
	static const UINT m_uTaskbarBtnCreatedMsg;
	CComPtr<ITaskbarList3> m_pTaskbarList;
	bool bInitTaskbar;
	CBitmap mBitmap;
	CBitmap mLogos;
	CRichEditCtrl mRtf;
	CRichEditCtrl mRtf2;
	volatile SETUP_STAGE mStage;
};
