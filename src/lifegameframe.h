#pragma once

#include "universe.h"

#include <wx/wx.h>
#include <memory>
#include <functional>

class Timer : public wxTimer {
public:
    using callback_t = std::function<void()>;

    Timer() {}
    Timer(const callback_t& cb) : callback(cb) {}
    void Notify() final { callback(); }

    callback_t callback;
};

class LifeGameFrame : public wxFrame {
public:
    LifeGameFrame();

    void OnPaint(wxPaintEvent& e);
    void OnCreated(wxWindowCreateEvent& e);
    void OnClose(wxCloseEvent& e);

private:
    void Draw();
    void Update();

private:
    std::unique_ptr<wxBitmap> _bitmap;
    std::unique_ptr<IUniverse> _u;
    Timer _timer;
};
