// Copyright (2015) Gustav

#ifndef RIDE_AUIX_H_
#define RIDE_AUIX_H_

#include <ride/wx.h>

#include <wx/aui/tabart.h>

#include <wx/generic/statusbr.h>

class AuiGenericTabArt : public wxAuiTabArt {
 public:
  AuiGenericTabArt();
  virtual ~AuiGenericTabArt();

  wxAuiTabArt* Clone();
  void SetFlags(unsigned int flags);
  void SetSizingInfo(const wxSize& tabCtrlSize, size_t tabCount);

  void SetNormalFont(const wxFont& font);
  void SetSelectedFont(const wxFont& font);
  void SetMeasuringFont(const wxFont& font);
  void SetColour(const wxColour& colour);
  void SetActiveColour(const wxColour& colour);

  void DrawBorder(wxDC& dc, wxWindow* wnd, const wxRect& rect);

  void DrawBackground(wxDC& dc, wxWindow* wnd, const wxRect& rect);

  void DrawTab(wxDC& dc, wxWindow* wnd, const wxAuiNotebookPage& pane,
               const wxRect& inRect, int closeButtonState, wxRect* outTabRect,
               wxRect* outButtonRect, int* xExtent);

  void DrawButton(wxDC& dc, wxWindow* wnd, const wxRect& inRect, int bitmapId,
                  int buttonState, int orientation, wxRect* outRect);

  int GetIndentSize();

  int GetBorderWidth(wxWindow* wnd);

  int GetAdditionalBorderSpace(wxWindow* wnd);

  wxSize GetTabSize(wxDC& dc, wxWindow* wnd, const wxString& caption,
                    const wxBitmap& bitmap, bool active, int closeButtonState,
                    int* xExtent);

  int ShowDropDown(wxWindow* wnd, const wxAuiNotebookPageArray& items,
                   int activeIdx);

  int GetBestTabCtrlSize(wxWindow* wnd, const wxAuiNotebookPageArray& pages,
                         const wxSize& requiredBmpSize);

  AuiGenericTabArt& set_backgroundColor(const wxColor& c);
  AuiGenericTabArt& set_activeTabBackground(const wxColor& c);
  AuiGenericTabArt& set_inactiveTabBackground(const wxColor& c);
  AuiGenericTabArt& set_activeBorderColor(const wxColor& c);
  AuiGenericTabArt& set_inactiveBorderColor(const wxColor& c);
  AuiGenericTabArt& set_activeTabText(const wxColor& c);
  AuiGenericTabArt& set_inactiveTabText(const wxColor& c);

 protected:
  wxFont m_normalFont;
  wxFont m_selectedFont;
  wxFont m_measuringFont;

  // base
  wxColor backgroundColor_;
  wxColor activeTabBackground_;
  wxColor inactiveTabBackground_;
  wxColor activeBorderColor_;
  wxColor inactiveBorderColor_;
  wxColor activeTabText_;
  wxColor inactiveTabText_;

  wxBitmap m_activeCloseBmp;
  wxBitmap m_disabledCloseBmp;
  wxBitmap m_activeLeftBmp;
  wxBitmap m_disabledLeftBmp;
  wxBitmap m_activeRightBmp;
  wxBitmap m_disabledRightBmp;
  wxBitmap m_activeWindowListBmp;
  wxBitmap m_disabledWindowListBmp;

  int m_fixedTabWidth;
  int m_tabCtrlHeight;
  unsigned int m_flags;
};

class StatusBarGeneric : public wxStatusBarGeneric {
 public:
  StatusBarGeneric(wxWindow* parent, wxWindowID winid = wxID_ANY,
                   long style = wxSTB_DEFAULT_STYLE,  // NOLINT wx default
                   const wxString& name = wxStatusBarNameStr);

  void set_shadow(const wxColor& shadow) { shadow_ = shadow; }
  void set_highlight(const wxColor& highlight) { highlight_ = highlight; }

 private:
  wxColor shadow_;
  wxColor highlight_;
  int wxFIELD_TEXT_MARGIN;

 protected:
  void InitColours();

  void DrawField(wxDC& dc, int i, int textHeight);  // NOLINT

  void DrawFieldText(wxDC& dc, const wxRect& rect, int i, int textHeight);
};

#endif  // RIDE_AUIX_H_
