// Copyright (2015) Gustav

#ifndef RIDE_MENUEVENT_H_
#define RIDE_MENUEVENT_H_

enum {
  ID_FIRST = wxID_HIGHEST,

  ID_EDIT_MATCH_BRACE,
  ID_EDIT_SELECT_BRACE,
  ID_EDIT_GOTO_LINE,
  ID_EDIT_SELECT_LINE,
  ID_EDIT_TOUPPER,
  ID_EDIT_TOLOWER,
  ID_EDIT_MOVELINESUP,
  ID_EDIT_MOVELINESDOWN,
  ID_EDIT_SHOW_PROPERTIES,
  ID_EDIT_OPEN_IN_ONLINE_DOCUMENTATION,
  ID_EDIT_SHOW_AUTOCOMPLETE,

  ID_PROJECT_NEW,
  ID_PROJECT_OPEN,
  ID_PROJECT_SETTINGS,
  ID_PROJECT_BUILD,
  ID_PROJECT_SELECT_ACTIVE_BUILD,
  ID_PROJECT_CLEAN,
  ID_PROJECT_REBUILD,
  ID_PROJECT_DOC,
  ID_PROJECT_RUN,
  ID_PROJECT_SELECT_ACTIVE_RUN,
  ID_PROJECT_TEST,
  ID_PROJECT_BENCH,
  ID_PROJECT_UPDATE,

  ID_FIND_IN_FILES,
  ID_REPLACE_IN_FILES,

  ID_VIEW_RESTORE_WINDOWS,
  ID_VIEW_SAVE_LAYOUT,
  ID_VIEW_LOAD_LAYOUT,
  ID_VIEW_SHOW_FINDRESULT,
  ID_VIEW_SHOW_START,
  ID_VIEW_SHOW_PROJECT,
  ID_VIEW_SHOW_BUILD,
  ID_VIEW_SHOW_COMPILE,

  ID_TAB_NEXT,
  ID_TAB_PREV,

  ID_QUICK_OPEN,

  ID_GAMES_BOMBS,
  ID_GAMES_FORTY,

  //////////////////////////////////////////////////////////////////////////
  // Specific tab events

  ID_SPECIAL_TAB_NO_EVENT,

  // bombs
  bombsID_NEW_GAME,
  bombsID_LEVEL,
  bombsID_EASY,
  bombsID_MEDIUM,
  bombsID_HARD,
  bombsID_EASYCORNER,

  // forty thieves
  RIGHT_BUTTON_UNDO,
  HELPING_HAND,
  LARGE_CARDS,
  FORTY_NEW,
  FORTY_ABOUT,
  FORTY_UNDO,
  FORTY_REDO,
  FORTY_HELP_CONTENTS,

  ID_SPECIAL_TAB_LAST_EVENT
};

#endif  // RIDE_MENUEVENT_H_
