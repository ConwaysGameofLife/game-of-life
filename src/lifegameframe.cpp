#include "lifegameframe.h"

#include <wx/dcbuffer.h>
#include <wx/rawbmp.h>

#include <iostream>
#include <cmath>

const wxString TITLE = wxT("Conway's Game of Life");

LifeGameFrame::LifeGameFrame(int width, int height)
   : wxFrame(NULL, -1, TITLE, wxDefaultPosition, wxSize(width, height)) {
    Bind(wxEVT_PAINT, &LifeGameFrame::OnPaint, this);
    Bind(wxEVT_SIZE, &LifeGameFrame::OnSizeChanged, this);
    Bind(wxEVT_MOUSEWHEEL, &LifeGameFrame::OnMouseScroll, this);
    Bind(wxEVT_MOTION, &LifeGameFrame::OnMouseMove, this);
    Bind(wxEVT_LEFT_DOWN, &LifeGameFrame::OnMouseLDown, this);
    Bind(wxEVT_IDLE, &LifeGameFrame::OnIdle, this);

    Regenerate(width, height);
}

void LifeGameFrame::Regenerate(int width, int height) {
    _u = bigBang<CpuAvxUniverse>(width, height);
}

void LifeGameFrame::OnSizeChanged(wxSizeEvent& e) {
    auto size = e.GetSize();
    _bitmap = std::make_unique<wxBitmap>(size.GetWidth(), size.GetHeight(), 32);
}

void LifeGameFrame::OnPaint(wxPaintEvent& e) {
    Draw();
    _startUpdate = true;
}

void LifeGameFrame::OnIdle(wxIdleEvent& e) {
    if (_startUpdate) {
        Update();
        e.RequestMore();
    }
}

void LifeGameFrame::Draw() {
    assert(_bitmap);
    assert(_u);
    wxAlphaPixelData pixels(*_bitmap);
    assert(pixels);

    auto w = pixels.GetWidth();
    auto h = pixels.GetHeight();

    auto src = _u->render() + _u->width() * _deltaY;
    wxAlphaPixelData::Iterator dst(pixels);
    for (int y = 0; y < h; ++y) {
        auto p = src + _deltaX;
        for (int x = 0; x < w; ++x, ++dst, ++p) {
            dst.Alpha() = 0xFF;     // TODO: assign alpha only once?
            dst.Red() = *p;
            dst.Green() = *p;
            dst.Blue() = *p;
        }
        src += _u->width();
    }

    wxBufferedPaintDC dc(this);
    dc.SetUserScale(_magnifier, _magnifier);
    dc.DrawBitmap(*_bitmap, 0, 0);
}

void LifeGameFrame::Update() {
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
        _deltaX += _ldown.x - wxGetMousePosition().x;
        if (_deltaX < 0) {
            _deltaX = 0;
        }
        if (_deltaX > _u->width() - _bitmap->GetSize().GetWidth()) {
            _deltaX = _u->width() - _bitmap->GetSize().GetWidth();
        }
        _deltaY += _ldown.y - wxGetMousePosition().y;
        if (_deltaY < 0) {
            _deltaY = 0;
        }
        if (_deltaY > _u->height() - _bitmap->GetSize().GetHeight()) {
            _deltaY = _u->height() - _bitmap->GetSize().GetHeight();
        }
        _ldown = wxGetMousePosition();
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

void LifeGameFrame::OnKeyUp(wxKeyEvent& e) {
    switch (e.GetKeyCode()) {
    case WXK_RETURN:
        Regenerate(_u->width(), _u->height());
        std::cerr << "WXK_RETURN" << std::endl;
        break;
    case WXK_SPACE:
        TogglePause();
        break;
    }
    std::cerr << "KEY UP" << std::endl;
}
