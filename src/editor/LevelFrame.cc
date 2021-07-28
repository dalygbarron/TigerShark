#include "App.hh"

using namespace App;

LevelFrame::LevelFrame(): wxFrame(NULL, wxID_ANY, "gday") {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(Event::Hello, "&Hello...\tCtrl-H", "says help string");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    this->SetMenuBar(menuBar);
    this->CreateStatusBar();
    this->SetStatusText("Tiger Shark Engine: EXTREME SEX Edition");
    this->Bind(wxEVT_MENU, [=](wxCommandEvent&) {
        wxLogMessage("gday idiots");
    }, Event::Hello);
    this->Bind(wxEVT_MENU, [=](wxCommandEvent&) {
        wxMessageBox("hello", "about helo", wxOK | wxICON_INFORMATION);
    }, wxID_ABOUT);
    this->Bind(wxEVT_MENU, [=](wxCommandEvent&) {
        this->Close(true);
    }, wxID_EXIT);
}
