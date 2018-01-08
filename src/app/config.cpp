#include "config.h"

#include <wx/gbsizer.h>

const wxString ConfigDialog::TITLE = wxT("Configuration");

ConfigDialog::ConfigDialog()
    : wxDialog(NULL, wxID_ANY, TITLE, wxDefaultPosition)
{
    auto panel = new wxPanel(this, wxID_ANY);
    auto vbox = new wxBoxSizer(wxVERTICAL);

    auto grid = new wxGridBagSizer(9, 25);

    int r = 0;
    grid->Add(new wxStaticText(panel, wxID_ANY, wxT("Width")), {r, 0}, wxDefaultSpan, 0, wxALIGN_LEFT);
    grid->Add(new wxTextCtrl(panel, wxID_ANY), {r, 1}, wxDefaultSpan, wxALIGN_LEFT | wxLEFT, 5);
    grid->Add(new wxStaticText(panel, wxID_ANY, wxT("Height")), {r, 2}, wxDefaultSpan, wxALIGN_LEFT | wxLEFT, 10);
    grid->Add(new wxTextCtrl(panel, wxID_ANY), {r, 3}, wxDefaultSpan, wxALIGN_LEFT | wxLEFT, 5);

    ++r;
    grid->Add(new wxStaticText(panel, wxID_ANY, wxT("Seed")), {r, 0}, wxDefaultSpan, wxALIGN_LEFT);
    grid->Add(new wxTextCtrl(panel, wxID_ANY), {r, 1}, {1, 3}, wxEXPAND | wxALIGN_LEFT | wxLEFT, 5);

    ++r;
    grid->Add(new wxStaticText(panel, wxID_ANY, wxT("Generation per second")), {r, 0}, wxDefaultSpan, wxALIGN_LEFT);
    grid->Add(new wxTextCtrl(panel, wxID_ANY), {r, 1}, {1, 3}, wxEXPAND | wxALIGN_LEFT | wxLEFT, 5);

    ++r;
    grid->Add(new wxCheckBox(panel, wxID_ANY, wxT("Mouse control")), {r, 0}, {1, 2}, wxALIGN_LEFT);
    grid->Add(new wxCheckBox(panel, wxID_ANY, wxT("Color cell by age")), {r, 2}, {1, 2}, wxALIGN_LEFT);

    ++r;
    grid->Add(new wxStaticText(panel, wxID_ANY, wxT("Number of swarms")), {r, 0}, wxDefaultSpan, wxALIGN_LEFT);
    grid->Add(new wxTextCtrl(panel, wxID_ANY), {r, 1}, {1, 3}, wxEXPAND | wxALIGN_LEFT | wxLEFT, 5);

    ++r;
    grid->Add(new wxStaticText(panel, wxID_ANY, wxT("Algorithm")), {r, 0}, wxDefaultSpan, wxALIGN_LEFT);
    grid->Add(new wxComboBox(panel, wxID_ANY, wxT("CPU AVX256 Single-Threaded")), {r, 1}, {1, 3}, wxEXPAND | wxALIGN_LEFT | wxLEFT, 5);

    vbox->Add(grid, 0, wxALL, 10);

    auto hboxConfirmBtn = new wxBoxSizer(wxHORIZONTAL);
    auto okButton = new wxButton(panel, wxID_OK, wxT("Ok"), wxDefaultPosition, wxSize(70, 30));
    okButton->SetDefault();
    hboxConfirmBtn->Add(okButton, 1);
    auto closeButton = new wxButton(panel, wxID_CANCEL, wxT("Close"), wxDefaultPosition, wxSize(70, 30));
    closeButton->SetDefault();
    hboxConfirmBtn->Add(closeButton, 1, wxLEFT, 5);

    vbox->Add(hboxConfirmBtn, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    panel->SetSizer(vbox);
    panel->Fit();
    Fit();
}
