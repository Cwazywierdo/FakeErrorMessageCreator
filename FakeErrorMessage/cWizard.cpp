#include "cWizard.h"

cWizard::cWizard() : wxFrame(nullptr, wxID_ANY, "Message Creation Wizard") {
	SetIcon(wxICON(MAINICON));

	wxBoxSizer* pMainSizer = new wxBoxSizer(wxVERTICAL);

	// A panel is used because it allows tab traversal
	wxPanel* pMainPanel = new wxPanel(this);
	wxBoxSizer* pPanelSizer = new wxBoxSizer(wxVERTICAL);

#pragma region title sizer
	wxBoxSizer* pTitleSizer = nullptr;
	pTitleSizer = new wxBoxSizer(wxHORIZONTAL);
	m_pTitleCtrl = new wxTextCtrl(pMainPanel, wxID_ANY, "Error");
	m_pTitleCtrl->SetHint("Window Title");
	// expand the title control horizontally, and add extra padding to the right
	pTitleSizer->Add(m_pTitleCtrl, wxSizerFlags(1).Border(wxRIGHT, 30));

	// don't expand the title sizer vertically, expand it horizontally, and add 5px padding on all sides
	pPanelSizer->Add(pTitleSizer, wxSizerFlags(0).Expand().Border(wxALL, 5));
#pragma endregion

#pragma region message sizer
	wxBoxSizer* pMessageSizer = new wxBoxSizer(wxHORIZONTAL);
	m_pMessageCtrl = new wxTextCtrl(pMainPanel, wxID_ANY, "An unexpected error occured.", wxDefaultPosition, wxSize(200, 100), wxTE_MULTILINE);
	m_pMessageCtrl->SetHint("Message to display");
	// expand the message control horizontally and vertically
	pMessageSizer->Add(m_pMessageCtrl, wxSizerFlags(1).Expand());

	// expand the message sizer vertically, horizontally, and add a 5px padding on all sides
	pPanelSizer->Add(pMessageSizer, wxSizerFlags(1).Expand().Border(wxALL, 5));
#pragma endregion

#pragma region bottom sizer
	// bottom sizer
	wxBoxSizer* pBottomSizer = new wxBoxSizer(wxHORIZONTAL);
	
#pragma region browse button sizer
	wxBoxSizer* pBrowseSizer = new wxBoxSizer(wxVERTICAL);

	// "icon:" text
	wxStaticText* pIconText = new wxStaticText(pMainPanel, wxID_ANY, "icon:");
	pBrowseSizer->Add(pIconText, wxSizerFlags(0).Border(wxBOTTOM, 5));
	// file picker
	wxButton* pIconButton = new wxButton(pMainPanel, wxID_ANY, "Browse");
	pIconButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cWizard::SetIconFromSelection, this);
	pBrowseSizer->Add(pIconButton, wxSizerFlags(0));

	pBottomSizer->Add(pBrowseSizer, wxSizerFlags(0).Center().Border(wxRIGHT, 5));
#pragma endregion

#pragma region icon preview
	wxImage::AddHandler(new wxICOHandler);
	m_pBmpCtrl = new wxGenericStaticBitmap(pMainPanel, wxID_ANY, wxBitmapBundle(wxImage()), wxDefaultPosition, wxSize(64, 64), wxBORDER);

	// Set the icon to this executables icon by default
	WCHAR targetPath[MAX_PATH];
	GetModuleFileNameW(NULL, targetPath, MAX_PATH);
	SetShortcutIcon(targetPath);

	m_pBmpCtrl->SetScaleMode(wxStaticBitmap::Scale_AspectFit);
	pBottomSizer->Add(m_pBmpCtrl, wxSizerFlags(0).Center());
#pragma endregion

#pragma region create buttons sizer 
	wxBoxSizer* pButtonsSizer = new wxBoxSizer(wxVERTICAL);
	wxButton* pPreviewButton = new wxButton(pMainPanel, wxID_ANY, "Preview");
	pPreviewButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cWizard::CreatePreview, this);
	pButtonsSizer->Add(pPreviewButton, wxSizerFlags(1).Expand());
	wxButton* pCreateButton = new wxButton(pMainPanel, wxID_ANY, "Create");
	pCreateButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cWizard::CreateShortcut, this);
	pButtonsSizer->Add(pCreateButton, wxSizerFlags(1).Expand());
	pBottomSizer->Add(pButtonsSizer, wxSizerFlags(1).Expand().Border(wxALL, 5));
#pragma endregion

	pPanelSizer->Add(pBottomSizer, wxSizerFlags(0).Expand().Border(wxALL, 5));
#pragma endregion

	pMainPanel->SetSizerAndFit(pPanelSizer);

	pMainSizer->Add(pMainPanel, wxSizerFlags(1).Expand());
	SetSizerAndFit(pMainSizer);
}

void cWizard::CreatePreview(wxCommandEvent& evt) {
	CreateErrorMessage(m_pTitleCtrl->GetValue(), m_pMessageCtrl->GetValue());
}

void cWizard::CreateShortcut(wxCommandEvent& evt) {
	wxFileDialog saveFileDialog(this,
		"Save shortcut file...", 
		wxEmptyString,
		wxEmptyString, 
		"Shortcut file (*lnk)|*.lnk", 
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	std::wstring path = saveFileDialog.GetPath().ToStdWstring();
	std::wstring title = m_pTitleCtrl->GetValue().ToStdWstring();
	std::wstring message = m_pMessageCtrl->GetValue().ToStdWstring();
	std::wstring iconPath = m_iconPath.ToStdWstring();

	// Ensure that the extension is .lnk if the user manually
	// added a .exe extension in the save dialogue
	if (path.substr(path.length() - 4, 4) != ".lnk") {
		path += L".lnk";
	}

	CreateShortcutFile(path, title, message, iconPath);
}

void cWizard::SetShortcutIcon(wxString path) {
	// Get the icon for the item at the end of the path
	WORD index = 0;
	HICON hicon = ExtractAssociatedIcon(NULL, (LPWSTR)path.wc_str(), &index);

	// Create a wxIcon from the HICON extracted from the path
	wxIcon icon;
	bool res = icon.CreateFromHICON(WXHICON(hicon));
	// If the wxIcon is successfully created...
	if (res) {
		// Set the bitmap control's image to the icon
		wxBitmapBundle bmpBundle = wxBitmapBundle(icon);
		m_pBmpCtrl->SetBitmap(bmpBundle);
		// Setting the control's bitmap automatically changes the size
		m_pBmpCtrl->SetSize(64, 64);
	}
	// Always set the saved icon path to the passed icons path
	m_iconPath = path;
}

void cWizard::SetIconFromSelection(wxCommandEvent& evt) {
	wxFileDialog chooseIconDialog(this,
		"Choose an icon...",
		wxEmptyString,
		wxEmptyString,
		"Executable and Icon files (*.exe;*.ico)|*.exe;*.ico",
		wxFLP_OPEN | wxFLP_FILE_MUST_EXIST);

	if (chooseIconDialog.ShowModal() == wxID_CANCEL)
		return;

	SetShortcutIcon(chooseIconDialog.GetPath());
}