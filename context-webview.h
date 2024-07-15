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


    wxDECLARE_EVENT_TABLE();
};

