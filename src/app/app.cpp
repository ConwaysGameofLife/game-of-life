#include "lifegamepanel.h"

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

const auto APP_TITLE = wxT("Conway's Game of Life");

class LifeGameApp : public wxApp {
public:
    bool OnInit() override {
        int width = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
        int height = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
        auto* frame = new wxFrame(nullptr, -1, APP_TITLE,
                                  wxPoint(0, 0), wxSize(width, height));
        wxFileName binPath(wxStandardPaths::Get().GetExecutablePath());
        wxFileName iconPath(binPath.GetPath(), wxT("glider.png"));
        frame->SetIcon(wxIcon(iconPath.GetFullPath()));
        frame->Center();
        auto* panel = new LifeGamePanel(frame, width, height);
        auto* sizer = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(panel, 1, wxEXPAND);
        frame->SetSizer(sizer);
        frame->SetAutoLayout(true);
        frame->Show(true);
        return true;
    }
};

IMPLEMENT_APP(LifeGameApp)
