#ifndef RIDE_PROJECTEXPLORER_H
#define RIDE_PROJECTEXPLORER_H

#include "ride/wx.h"
#include <wx/treectrl.h>

class wxFileName;
class MainWindow;

class ProjectExplorer : public wxTreeCtrl {
public:
  explicit ProjectExplorer(MainWindow* main);

  void UpdateColors();

  void SetFolder(const wxString& folder);
  void UpdateFolderStructure();

private:
  void SubUpdateFolderStructure(const wxFileName& root, wxTreeItemId parent, const wxString filespec, const int flags, int index);

  void OnDoubleClick(wxMouseEvent& event);

  wxDECLARE_EVENT_TABLE();

private:
  wxImageList images_;
  MainWindow* main_;
  wxString folder_;
};

#endif  // RIDE_PROJECTEXPLORER_H
