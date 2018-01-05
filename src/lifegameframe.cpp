#include "lifegameframe.h"

#include <wx/dcbuffer.h>
#include <wx/rawbmp.h>

#include <iostream>

const wxString TITLE = wxT("Conway's Game of Life");

LifeGameFrame::LifeGameFrame()
   : wxFrame(NULL, -1, TITLE, wxDefaultPosition, wxSize(800, 600)) {
    Bind(wxEVT_PAINT, &LifeGameFrame::OnPaint, this);
    Bind(wxEVT_CREATE, &LifeGameFrame::OnCreated, this);
    Bind(wxEVT_CLOSE_WINDOW, &LifeGameFrame::OnClose, this);
}

void LifeGameFrame::OnCreated(wxWindowCreateEvent& e) {
    auto size = GetClientSize();
    auto width = size.GetWidth();
    auto height = size.GetHeight();

    _u = bigBang<CpuLoopUniverse>(width, height);
    _bitmap = std::make_unique<wxBitmap>(width, height, 32);
    _timer.callback = std::bind(&LifeGameFrame::Update, this);
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
    for (int y = 0; y < pixels.GetHeight(); ++y) {
        for (int x = 0; x < pixels.GetWidth(); ++x, ++dst, ++src) {
            dst.Alpha() = 0xFF;     // TODO: assign alpha only once?
            dst.Red() = *src;
            dst.Green() = *src;
            dst.Blue() = *src;
        }
    }

    wxBufferedPaintDC dc(this);
    dc.DrawBitmap(*_bitmap, 0, 0);
}

void LifeGameFrame::Update() {
    _u->next();
    Draw();
}
