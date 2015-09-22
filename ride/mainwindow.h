// Copyright (2015) Gustav

#ifndef RIDE_MAINWINDOW_H_
#define RIDE_MAINWINDOW_H_

#include <wx/aui/aui.h>

#include "ride/language.h"
#include "ride/settings.h"
#include "ride/project.h"
#include "ride/outputdirector.h"

class FileEdit;
class SettingsDlg;
class CompilerMessage;
class FindResultsControl;
class ProjectExplorer;

struct FoundEdit {
  FoundEdit(size_t i, FileEdit* e) : index(i), edit(e) {}

  // has a edit been found?
  operator bool() const { return edit != NULL; }

  static FoundEdit NOT_FOUND;

  size_t index;
  FileEdit* edit;
};

void CreateNewFile(const wxString& project_root, MainWindow* main,
                   ProjectExplorer* project_explorer);

enum StatusBarWidgets {
  STATUSBAR_GENERAL,
  STATUSBAR_BUILD_CONF,
  STATUSBAR_RUN_CONF,
  STATUSBAR_LINE,
  STATUSBAR_COL,
  STATUSBAR_CH,
  STATUSBAR_INS,
  STATUSBAR_MAXCOUNT
};

class MainWindow : public wxFrame {
 public:
  MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
  ~MainWindow();

  const ride::Settings& settings() const;
  void set_settings(const ride::Settings& settings);

  void SetStatusBarText(const wxString& text, StatusBarWidgets widget);

  void OpenCompilerMessage(const CompilerMessage& message);
  void FileHasBeenRenamed(const wxString& old_path, const wxString& new_path);
  void ProjectSettingsHasChanged();

  void SaveAllChangedProjectFiles();
  void ReloadFilesIfNeeded();

  FileEdit* OpenFile(const wxString& file, int start_line = -1,
                     int start_index = -1, int end_line = -1,
                     int end_index = -1);
  FileEdit* GetFile(const wxString& file);

  const wxString& root_folder() const;

  FoundEdit GetEditFromFileName(const wxString& file);

  OutputDirector& build_output();
  OutputDirector& compiler_output();

  Project* project();

 private:
  void OnActivated(wxActivateEvent& event);

  void OnFileOpen(wxCommandEvent& event);
  void OnFileSave(wxCommandEvent& event);
  void OnFileSaveAs(wxCommandEvent& event);

  void OnEditUndo(wxCommandEvent& event);
  void OnEditRedo(wxCommandEvent& event);
  void OnEditCut(wxCommandEvent& event);
  void OnEditCopy(wxCommandEvent& event);
  void OnEditPaste(wxCommandEvent& event);
  void OnEditDuplicate(wxCommandEvent& event);
  void OnEditDelete(wxCommandEvent& event);
  void OnEditFind(wxCommandEvent& event);
  void OnEditReplace(wxCommandEvent& event);
  void OnEditMatchBrace(wxCommandEvent& event);
  void OnEditSelectBrace(wxCommandEvent& event);
  void OnEditGotoLine(wxCommandEvent& event);
  void OnEditIndent(wxCommandEvent& event);
  void OnEditUnIndent(wxCommandEvent& event);
  void OnEditSelectAll(wxCommandEvent& event);
  void OnEditSelectLine(wxCommandEvent& event);
  void OnEditToUpper(wxCommandEvent& event);
  void OnEditToLower(wxCommandEvent& event);
  void OnEditMoveLinesUp(wxCommandEvent& event);
  void OnEditMoveLinesDown(wxCommandEvent& event);
  void OnEditOpenInOnlineDocumentation(wxCommandEvent& event);
  void OnEditShowProperties(wxCommandEvent& event);
  void OnEditShowAutocomplete(wxCommandEvent& event);

  void OnProjectNew(wxCommandEvent& event);
  void OnProjectOpen(wxCommandEvent& event);
  void OnProjectSettings(wxCommandEvent& event);
  void OnProjectBuild(wxCommandEvent& event);
  void OnProjectSelectActiveBuild(wxCommandEvent& event);
  void OnProjectClean(wxCommandEvent& event);
  void OnProjectRebuild(wxCommandEvent& event);
  void OnProjectDoc(wxCommandEvent& event);
  void OnProjectRun(wxCommandEvent& event);
  void OnProjectSelectActiveRun(wxCommandEvent& event);
  void OnProjectTest(wxCommandEvent& event);
  void OnProjectBench(wxCommandEvent& event);
  void OnProjectUpdate(wxCommandEvent& event);
  void OnProjectFileNew(wxCommandEvent& event);
  void OnProjectQuickOpen(wxCommandEvent& event);
  void OnProjectFindInFiles(wxCommandEvent& event);
  void OnProjectReplaceInFiles(wxCommandEvent& event);

  void OnViewRestoreWindows(wxCommandEvent& event);
  void OnViewSaveLayout(wxCommandEvent& event);
  void OnViewLoadLayout(wxCommandEvent& event);
  void OnViewShowStart(wxCommandEvent& event);
  void OnViewShowFindResult(wxCommandEvent& event);
  void OnViewShowBuild(wxCommandEvent& event);
  void OnViewShowCompile(wxCommandEvent& event);
  void OnViewShowProject(wxCommandEvent& event);

  void OnFileExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnFileShowSettings(wxCommandEvent& event);

  void OnClose(wxCloseEvent& event);

  void OnNotebookPageClose(wxAuiNotebookEvent& event);
  void OnNotebookPageChanged(wxAuiNotebookEvent& event);

  void OnMenuOpen(wxMenuEvent& event);

  void BindEvents();

 private:
  void ShowFindWindow();
  void ShowBuildWindow();
  void ShowCompileWindow();

  void SaveSession();
  void RestoreSession();
  bool OpenProject(const wxString project_file);

  void UpdateTitle();
  void UpdateAllEdits();
  void CreateNotebook();
  FileEdit* GetSelectedEditorNull();
  FileEdit* AddAllCompilerMessages(FileEdit* file_edit);

  void UpdateMenuItemView();

 private:
  bool closing_;
  wxAuiManager aui_;
  wxAuiNotebook* notebook_;
  OutputDirector build_output_;
  OutputDirector compiler_output_;
  FindResultsControl* findres_window_;
  ProjectExplorer* project_explorer_;

  ride::Settings settings_;
  std::unique_ptr<Project> project_;
  wxString app_name_;

  wxString windows_locations_;

  wxMenuItem* menuItemViewFind_;
  wxMenuItem* menuItemViewProject_;
  wxMenuItem* menuItemViewBuild_;
  wxMenuItem* menuItemViewCompile_;

  wxWindow* last_focus_;
  wxStatusBar* statusbar_;

  Languages languages_;
};

#endif  // RIDE_MAINWINDOW_H_
