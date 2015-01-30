#ifndef RIDE_MAINWINDOW_H
#define RIDE_MAINWINDOW_H

#include <wx/wx.h>
#include <wx/aui/aui.h>

class MainWindow: public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
  ~MainWindow();

private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

  void OnNotebookPageClose(wxAuiNotebookEvent& evt);
  void OnNotebookPageClosed(wxAuiNotebookEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
  void setupEdit();
  void createNotebook();

  wxAuiManager aui;
  wxAuiNotebook* notebook;
};

#endif  // RIDE_MAINWINDOW_H