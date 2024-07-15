#include "context-webview.h"

#include "EmptyFileTest.h"

//MyFrame(wxWindow* parent = nullptr) : wxFrame(parent, wxID_ANY, "Test")


MyFrame::MyFrame(wxWindow* parent) : wxFrame(parent, wxID_ANY, "Test")
{



    SetMinClientSize(FromDIP(wxSize(800, 600)));

    m_webView = wxWebView::New(this, wxID_ANY, "https://www.wxWidgets.org");

    m_webView->EnableContextMenu(false);
    m_webView->Bind(wxEVT_WEBVIEW_LOADED, &MyFrame::OnWebViewLoaded, this);
    m_webView->AddScriptMessageHandler("wx_msg");
    m_webView->Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, &MyFrame::OnWebViewMessageReceived, this);

    wxMenuBar* const mbar = new wxMenuBar;
    wxMenu* const menuFrames = new wxMenu;

    SetMenuBar(mbar);
    Show();
};


void MyFrame::OnWebViewLoaded(wxWebViewEvent&)
{

    const wxString script =
        R"(
            document.addEventListener('contextmenu',
                (evt) => { var message = 'context-menu:' + evt.clientX + '|' + evt.clientY;
                            window.wx_msg.postMessage(message)});
        )";

    m_webView->RunScript(script);
};

void MyFrame::OnWebViewMessageReceived(wxWebViewEvent& evt)
{
    const wxString fullMessage = evt.GetString();
    wxString messageData;

    if ( fullMessage.StartsWith("context-menu:", &messageData) )
    {
        wxArrayString coords = wxSplit(messageData, '|');

        if ( coords.size() == 2 )
        {
            unsigned long x, y;

            if ( coords[0].ToCULong(&x) && coords[1].ToCULong(&y) )
            {
                wxMenu menu;
                wxPoint menuLocation(x, y);

                menu.Append(wxID_ABOUT);
                menuLocation = m_webView->ClientToScreen(menuLocation);
                menuLocation = ScreenToClient(menuLocation);
                wxLogDebug("x: %d, y: %d", menuLocation.x, menuLocation.y);
                GetPopupMenuSelectionFromUser(menu/* , menuLocation*/);
                return;
            }
        }
    }
    wxLogError("Invalid webview message");
};



wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU (wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()


void MyFrame::OnAbout (wxCommandEvent &WXUNUSED(event))
{
    wxMessageDialog dialog(this, "It's a test");
    dialog.ShowModal();

};




class MyApp : public wxApp
{
    bool OnInit() override
    {
        (new MyFrame())->Show();
        return true;
    }
}; wxIMPLEMENT_APP(MyApp);
