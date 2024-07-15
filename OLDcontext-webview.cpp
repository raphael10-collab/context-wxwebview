#include "context-webview.h"

#include "EmptyFileTest.h"

//MyFrame(wxWindow* parent = nullptr) : wxFrame(parent, wxID_ANY, "Test")

enum
{
    Show_Transparent = 1,
    Show_TransparentBg = 2
};


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
    menuFrames->Append(Show_Transparent, "Show &transparent window\tCtrl-T");

    SetMenuBar(mbar);
    Show();
};


SeeThroughFrame::~SeeThroughFrame() {};

wxBEGIN_EVENT_TABLE(SeeThroughFrame, wxFrame)
    EVT_PAINT(SeeThroughFrame::OnPaint)
wxEND_EVENT_TABLE()





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

void SeeThroughFrame::OnAlpha(wxCommandEvent& event)
{
    SetTransparent(event.GetInt());
}


// Paints a grid of varying hue and alpha
void SeeThroughFrame::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
    wxPaintDC dc(this);
    std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));

    // Draw 3 bands: one opaque, one semi-transparent and one transparent.
    const wxSize size = GetClientSize();
    const double h = size.y / 3.0;

    wxGraphicsPath path = gc->CreatePath();
    path.AddRectangle(0, 0, size.x, h);
    gc->SetBrush(wxBrush(wxColour(0, 0, 255, wxALPHA_OPAQUE)));
    gc->FillPath(path);

    gc->SetBrush(wxBrush(wxColour(0, 0, 255, wxALPHA_OPAQUE / 2)));
    gc->Translate(0, h);
    gc->FillPath(path);

    // This blue won't actually be seen and instead the white background will
    // be visible, because this brush is fully transparent.
    gc->SetBrush(wxBrush(wxColour(0, 0, 255, wxALPHA_TRANSPARENT)));
    gc->Translate(0, h);
    gc->FillPath(path);
}




class MyApp : public wxApp
{
    bool OnInit() override
    {
        (new MyFrame())->Show();
        return true;
    }
}; wxIMPLEMENT_APP(MyApp);
