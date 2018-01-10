#pragma once

#include <string>
#include <vector>

#include <wx/wx.h>

class ConfigDialog : public wxDialog {
public:
    ConfigDialog();

    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }
    int GetSeed() const { return _seed; }
    int GetGPS() const { return _gps; }
    bool IsMouseControlled() const { return _mouseControlled; }
    bool IsColored() const { return _colored; }
    int GetSwarmTotal() const { return _swarmTotal; }
    std::string GetAlgorithm() const { return _algorithm; }

    void SetAlgorithms(const std::string& current,
                       const std::vector<std::string>& algorithms);

private:
    static const wxString TITLE;

    wxComboBox *_algCBox;

    int _width;
    int _height;
    int _seed;
    int _gps;
    bool _mouseControlled;
    bool _colored;
    int _swarmTotal;
    std::string _algorithm;
};
