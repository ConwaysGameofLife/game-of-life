#pragma once

#include "universe.h"

#include <wx/wx.h>
#include <memory>
#include <functional>

class LifeGameFrame : public wxFrame {
public:
    LifeGameFrame();

private:
    void Draw();
    void Update(wxTimerEvent& e);
    void OnPaint(wxPaintEvent& e);
    void OnCreated(wxWindowCreateEvent& e);
    void OnClose(wxCloseEvent& e);

private:
    std::unique_ptr<wxBitmap> _bitmap;
    std::unique_ptr<IUniverse> _u;
    wxTimer _timer;
};
