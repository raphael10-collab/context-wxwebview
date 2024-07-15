#include <wx/wx.h>
#include <wx/webview.h>
#include <wx/graphics.h>
#include <memory>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/layout.h>
#include <wx/app.h>
#include <wx/stattext.h>
#include <wx/dcclient.h>

class MyFrame : public wxFrame
{
public:
    MyFrame(wxWindow* parent = nullptr);

private:
    wxWebView* m_webView{nullptr};

    void OnWebViewLoaded(wxWebViewEvent&);

    void OnWebViewMessageReceived(wxWebViewEvent& evt);

    void OnAbout (wxCommandEvent&  evt);

    void OnShowTransparent(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};


// Define a new frame type: this is going to the frame showing the
// effect of wxWindow::SetTransparent and of
// wxWindow::SetBackgroundStyle(wxBG_STYLE_TRANSPARENT)
class SeeThroughFrame : public wxFrame
{
public:
    virtual ~SeeThroughFrame();
    void Create(wxWindow* parent);

private:
    // event handlers (these functions should _not_ be virtual)
    void OnPaint(wxPaintEvent& evt);

    void OnAlpha(wxCommandEvent& event);

    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};
