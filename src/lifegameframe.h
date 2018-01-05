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
    void Update();
    void OnPaint(wxPaintEvent& e);
    void OnSizeChanged(wxSizeEvent& e);
    void OnMouseScroll(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnMouseLDown(wxMouseEvent& e);
    void OnIdle(wxIdleEvent& e);

    void ZoomIn();
    void ZoomOut();

private:
    std::unique_ptr<wxBitmap> _bitmap;
    std::unique_ptr<IUniverse> _u;
    int _magnifier = 1;
    int _deltaX = 0;
    int _deltaY = 0;
    wxPoint _ldown;
    bool _startUpdate = false;
};
