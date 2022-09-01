#pragma once

#include "wx/wx.h"
#include "cWizard.h"


class cApp : public wxApp
{
public:
	virtual bool OnInit();
};

void CreateErrorMessage(wxString title, wxString message);
