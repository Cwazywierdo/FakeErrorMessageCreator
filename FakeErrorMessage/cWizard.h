#pragma once

#include "wx/wx.h"
#include "wx/filepicker.h"
#include "cApp.h"
#include "CreateShortcut.h"
#include <string>
#include "wx/generic/statbmpg.h"

class cWizard : public wxFrame
{
public:
	cWizard();

private:
	void CreatePreview(wxCommandEvent& evt);
	void CreateShortcut(wxCommandEvent& evt);
	void SetShortcutIcon(wxString path);
	void SetIconFromSelection(wxCommandEvent& evt);

	wxTextCtrl* m_pTitleCtrl = nullptr;
	wxTextCtrl* m_pMessageCtrl = nullptr;
	wxGenericStaticBitmap* m_pBmpCtrl = nullptr;
	wxString m_iconPath = wxEmptyString;
};

