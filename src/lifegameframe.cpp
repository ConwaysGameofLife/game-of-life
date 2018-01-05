#include "lifegameframe.h"

#include <wx/dcbuffer.h>
#include <wx/rawbmp.h>

#include <iostream>
#include <cmath>

const wxString TITLE = wxT("Conway's Game of Life");

LifeGameFrame::LifeGameFrame(int width, int height)
   : wxFrame(NULL, -1, TITLE, wxDefaultPosition, wxSize(width, height)) {
    Bind(wxEVT_PAINT, &LifeGameFrame::OnPaint, this);
    Bind(wxEVT_CREATE, &LifeGameFrame::OnCreated, this);
    Bind(wxEVT_CLOSE_WINDOW, &LifeGameFrame::OnClose, this);
    _timer.SetOwner(this);
    Bind(wxEVT_TIMER, &LifeGameFrame::Update, this);
    Bind(wxEVT_MOUSEWHEEL, &LifeGameFrame::OnMouseScroll, this);
    Bind(wxEVT_MOTION, &LifeGameFrame::OnMouseMove, this);
    Bind(wxEVT_LEFT_DOWN, &LifeGameFrame::OnMouseLDown, this);
}

void LifeGameFrame::OnCreated(wxWindowCreateEvent& e) {
    auto size = GetClientSize();
    auto width = size.GetWidth();
    auto height = size.GetHeight();

    _u = bigBang<CpuAvxUniverse>(width, height);
    _bitmap = std::make_unique<wxBitmap>(width, height, 32);
}

void LifeGameFrame::OnClose(wxCloseEvent& e) {
    _timer.Stop();
    e.Skip();
}

void LifeGameFrame::OnPaint(wxPaintEvent& e) {
    Draw();
    if (!_timer.IsRunning()) {
        _timer.Start(50);
    }
}

void LifeGameFrame::Draw() {
    wxAlphaPixelData pixels(*_bitmap);
    assert(pixels);

    auto w = pixels.GetWidth();
    auto h = pixels.GetHeight();

    auto src = _u->render();
    wxAlphaPixelData::Iterator dst(pixels);
    for (int y = 0; y < w; ++y) {
        for (int x = 0; x < h; ++x, ++dst, ++src) {
            dst.Alpha() = 0xFF;     // TODO: assign alpha only once?
            dst.Red() = *src;
            dst.Green() = *src;
            dst.Blue() = *src;
        }
    }

    wxBufferedPaintDC dc(this);
    dc.SetUserScale(_magnifier, _magnifier);
    dc.DrawBitmap(*_bitmap, _deltaX, _deltaY);
}

void LifeGameFrame::Update(wxTimerEvent& e) {
    _u->next();
    Draw();
}

void LifeGameFrame::OnMouseScroll(wxMouseEvent& e) {
    if (e.GetWheelRotation() > 0) {
        ZoomIn();
    } else {
        ZoomOut();
    }
}

void LifeGameFrame::OnMouseMove(wxMouseEvent& e) {
    if (e.Dragging()) {
        _deltaX = wxGetMousePosition().x - _ldown.x;
        _deltaY = wxGetMousePosition().y - _ldown.y;
        // TODO: set a limit
    }
}

void LifeGameFrame::OnMouseLDown(wxMouseEvent& e) {
    _ldown = wxGetMousePosition();
}

void LifeGameFrame::ZoomIn() {
    int MAX = 5;
    _magnifier += 1;
    if (_magnifier > MAX) {
        _magnifier = MAX;
    }
}

void LifeGameFrame::ZoomOut() {
    _magnifier -= 1;
    if (_magnifier < 1) {
        _magnifier = 1;
    }
}
