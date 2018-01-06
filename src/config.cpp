#include "config.h"

const wxString ConfigDialog::TITLE = wxT("Configuration");

ConfigDialog::ConfigDialog()
    : wxDialog(NULL, wxID_ANY, TITLE, wxDefaultPosition)
{
    auto panel = new wxPanel(this, wxID_ANY);
    auto vbox = new wxBoxSizer(wxVERTICAL);

    auto hboxSize = new wxBoxSizer(wxHORIZONTAL);
    hboxSize->Add(new wxStaticText(panel, wxID_ANY, wxT("Width")), 0, wxALIGN_CENTER);
    hboxSize->Add(new wxTextCtrl(panel, wxID_ANY), 0, wxALIGN_CENTER | wxLEFT, 5);
    hboxSize->Add(new wxStaticText(panel, wxID_ANY, wxT("Height")), 0, wxALIGN_CENTER | wxLEFT, 10);
    hboxSize->Add(new wxTextCtrl(panel, wxID_ANY), 0, wxALIGN_CENTER | wxLEFT, 5);
    vbox->Add(hboxSize, 0, wxALL, 10);

    auto hboxSeed = new wxBoxSizer(wxHORIZONTAL);
    hboxSeed->Add(new wxStaticText(panel, wxID_ANY, wxT("Seed")), 0, wxALIGN_CENTER);
    hboxSeed->Add(new wxTextCtrl(panel, wxID_ANY), 0, wxALIGN_CENTER | wxLEFT, 5);
    vbox->Add(hboxSeed, 0, wxALL, 10);

    auto hboxSpeed = new wxBoxSizer(wxHORIZONTAL);
    hboxSpeed->Add(new wxStaticText(panel, wxID_ANY, wxT("Generation per second")), 0, wxALIGN_CENTER);
    hboxSpeed->Add(new wxTextCtrl(panel, wxID_ANY), 0, wxALIGN_CENTER | wxLEFT, 5);
    vbox->Add(hboxSpeed, 0, wxALL, 10);

    auto vboxSwitch = new wxBoxSizer(wxVERTICAL);
    vboxSwitch->Add(new wxCheckBox(panel, wxID_ANY, wxT("Mouse control")), 0, wxALIGN_LEFT);
    vboxSwitch->Add(new wxCheckBox(panel, wxID_ANY, wxT("Color cell by age")), 0, wxALIGN_LEFT);
    vbox->Add(vboxSwitch, 0, wxALL, 10);

    auto hboxAlg = new wxBoxSizer(wxHORIZONTAL);
    hboxAlg->Add(new wxStaticText(panel, wxID_ANY, wxT("Algorithm")), 0, wxALIGN_CENTER);
    hboxAlg->Add(new wxComboBox(panel, wxID_ANY), 0, wxALIGN_CENTER | wxLEFT, 5);
    vbox->Add(hboxAlg, 0, wxALL, 10);

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
