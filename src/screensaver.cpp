#include "lifegameframe.h"

#include <wx/wx.h>

class LifeGameScreenSaverApp : public wxApp {
public:
    bool OnInit() override {
        int width = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
        int height = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
        auto* frame = new LifeGameFrame(width, height);
        frame->Show(true);
        return true;
    }
};

IMPLEMENT_APP(LifeGameScreenSaverApp)
