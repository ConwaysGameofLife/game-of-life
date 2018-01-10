#include "lifegamepanel.h"

#include "config.h"

#include <wx/dcbuffer.h>
#include <wx/rawbmp.h>

#include <cmath>

const wxString TITLE = wxT("Conway's Game of Life");
const std::string DEFAULT_ALG = "CPU AVX";

LifeGamePanel::LifeGamePanel(wxFrame* parent, int width, int height)
   : wxPanel(parent) {
    Bind(wxEVT_PAINT, &LifeGamePanel::OnPaint, this);
    Bind(wxEVT_SIZE, &LifeGamePanel::OnSizeChanged, this);
    Bind(wxEVT_MOUSEWHEEL, &LifeGamePanel::OnMouseScroll, this);
    Bind(wxEVT_MOTION, &LifeGamePanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_DOWN, &LifeGamePanel::OnMouseLDown, this);
    Bind(wxEVT_IDLE, &LifeGamePanel::OnIdle, this);
    Bind(wxEVT_KEY_UP, &LifeGamePanel::OnKeyUp, this);

    BigBang::init();
    Regenerate(width, height);
}

void LifeGamePanel::Regenerate(int width, int height) {
    _u = BigBang::create("CPU AVX", width, height);
    Refresh();
}

void LifeGamePanel::OnSizeChanged(wxSizeEvent& e) {
    auto size = e.GetSize();
    _bitmap = std::make_unique<wxBitmap>(size.GetWidth(), size.GetHeight(), 32);
    wxAlphaPixelData pixels(*_bitmap);
    assert(pixels);
    auto iter = pixels.GetPixels();
    for (int i = 0; i < size.GetWidth() * size.GetHeight(); ++i, ++iter) {
        iter.Alpha() = 0xFF;
    }
}

void LifeGamePanel::OnPaint(wxPaintEvent& e) {
    Draw();
    if (_state == PAUSED) {
        _state = RUNNING;
    }
}

void LifeGamePanel::OnIdle(wxIdleEvent& e) {
    if (_state == RUNNING) {
        Update();
        e.RequestMore();
    }
}

void LifeGamePanel::Draw() {
    assert(_bitmap);
    assert(_u);
    wxAlphaPixelData pixels(*_bitmap);
    assert(pixels);

    auto w = pixels.GetWidth();
    auto h = pixels.GetHeight();

    auto src = _u->render() + _u->width() * (_delta.y / _magnifier);
    auto dst = pixels.GetPixels();
    for (int y = 0; y < ceil(h / _magnifier); ++y) {
        auto psr = src + (_delta.x / _magnifier);
        auto dsr = dst;
        for (int x = 0; x < ceil(w / _magnifier); ++x, ++dsr, ++psr) {
            dsr.Red() = *psr;
            dsr.Green() = *psr;
            dsr.Blue() = *psr;
        }
        src += _u->width();
        dst.OffsetY(pixels, 1);
    }

    wxClientDC dc(this);
    if (_magnifier == 1) {
        dc.DrawBitmap(*_bitmap, 0, 0);
    } else {
        wxMemoryDC tmpDc;
        tmpDc.SelectObject(*_bitmap);
        auto dcSize = dc.GetSize();
        dc.StretchBlit(0, 0, dcSize.GetWidth(), dcSize.GetHeight(),
                       &tmpDc,
                       0, 0, ceil(w / _magnifier), ceil(h / _magnifier));
    }
}

void LifeGamePanel::Update() {
    _u->next();
    Draw();
}

void LifeGamePanel::OnMouseScroll(wxMouseEvent& e) {
    if (e.GetWheelRotation() > 0) {
        ZoomIn();
    } else {
        ZoomOut();
    }
}

void LifeGamePanel::OnMouseMove(wxMouseEvent& e) {
    if (e.Dragging()) {
        _delta += _ldown - wxGetMousePosition();
        if (_delta.x < 0) {
            _delta.x = 0;
        }
        if (_delta.x > _u->width() - _bitmap->GetSize().GetWidth()) {
            _delta.x = _u->width() - _bitmap->GetSize().GetWidth();
        }
        if (_delta.y < 0) {
            _delta.y = 0;
        }
        if (_delta.y > _u->height() - _bitmap->GetSize().GetHeight()) {
            _delta.y = _u->height() - _bitmap->GetSize().GetHeight();
        }
        _ldown = wxGetMousePosition();
    }
}

void LifeGamePanel::OnMouseLDown(wxMouseEvent& e) {
    _ldown = wxGetMousePosition();
}

void LifeGamePanel::ZoomIn() {
    int MAX = 10;
    _magnifier += 1;
    if (_magnifier > MAX) {
        _magnifier = MAX;
    }
}

void LifeGamePanel::ZoomOut() {
    _magnifier -= 1;
    if (_magnifier < 1) {
        _magnifier = 1;
    }
}

void LifeGamePanel::OnKeyUp(wxKeyEvent& e) {
    switch (e.GetKeyCode()) {
    case WXK_RETURN:
    case WXK_NUMPAD_ENTER:
        Regenerate(_u->width(), _u->height());
        break;
    case WXK_SPACE:
        Toggle();
        break;
    }
}

void LifeGamePanel::Toggle() {
    if (_state == STOPPED) {
        _state = RUNNING;
    } else {
        _state = STOPPED;
        ConfigDialog config;
        config.SetAlgorithms(DEFAULT_ALG, BigBang::names());
        if (config.ShowModal() != wxID_OK) {
        }
        _state = RUNNING;
    }
}
