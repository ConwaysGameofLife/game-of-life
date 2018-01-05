#include "lifegameframe.h"

#include <wx/wx.h>

class LifeGameScreenSaverApp : public wxApp {
public:
    bool OnInit() override {
        auto* frame = new LifeGameFrame();
        frame->Show(true);
        return true;
    }
};

IMPLEMENT_APP(LifeGameScreenSaverApp)
