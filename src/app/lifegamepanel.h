#pragma once

#include <liblife/universe.h>

#include <wx/wx.h>
#include <memory>
#include <functional>

class LifeGamePanel : public wxPanel {
public:
    LifeGamePanel(wxFrame* parent, int width, int height);

private:
    void Draw();
    void Update();
    void OnPaint(wxPaintEvent& e);
    void OnSizeChanged(wxSizeEvent& e);
    void OnMouseScroll(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnMouseLDown(wxMouseEvent& e);
    void OnIdle(wxIdleEvent& e);
    void OnKeyUp(wxKeyEvent& e);

    void ZoomIn();
    void ZoomOut();

    void Regenerate(int width, int height);
    void Toggle();

private:
    std::unique_ptr<wxBitmap> _bitmap;
    std::unique_ptr<IUniverse> _u;
    int _magnifier = 1;
    wxPoint _delta;
    wxPoint _ldown;

    enum {
        PAUSED,
        STOPPED,
        RUNNING,
    } _state = PAUSED;
};
