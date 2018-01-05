#pragma once

#include "universe.h"

#include <wx/wx.h>
#include <memory>
#include <functional>

class LifeGameFrame : public wxFrame {
public:
    LifeGameFrame(int width, int height);

private:
    void Draw();
    void Update(wxTimerEvent& e);
    void OnPaint(wxPaintEvent& e);
    void OnCreated(wxWindowCreateEvent& e);
    void OnClose(wxCloseEvent& e);
    void OnMouseScroll(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnMouseLDown(wxMouseEvent& e);

    void ZoomIn();
    void ZoomOut();

private:
    std::unique_ptr<wxBitmap> _bitmap;
    std::unique_ptr<IUniverse> _u;
    wxTimer _timer;
    int _magnifier = 1;
    int _deltaX = 0;
    int _deltaY = 0;
    wxPoint _ldown;
};
