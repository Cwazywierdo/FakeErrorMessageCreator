#include "CApp.h"

wxIMPLEMENT_APP(cApp);

bool cApp::OnInit() {
	if (argc != 3) {
		// launch shortcut creation wizard
		cWizard* wizard = new cWizard();
		wizard->Show();
	}
	else {
		// create fake error message
		CreateErrorMessage(argv[1], argv[2]);
	}

	return true;
}

void CreateErrorMessage(wxString title, wxString message) {
	// a dummy frame is needed or else the program hangs after closing the message dialog
	wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "dummy");
	wxMessageDialog dialog(frame, message, title, wxOK | wxICON_ERROR);
	dialog.ShowModal();
	frame->Close(true);
}