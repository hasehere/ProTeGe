/***************************************************************
 * Name:      ProTeGeMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Hase ()
 * Created:   2015-03-02
 * Copyright: Hase ()
 * License:
 **************************************************************/

#include "ProTeGeMain.h"
#include <wx/msgdlg.h>
#include <wx/image.h>
#include "NoiseLib.cpp"

//(*InternalHeaders(ProTeGeFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)


//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(ProTeGeFrame)
const long ProTeGeFrame::ID_BUTTON1 = wxNewId();
const long ProTeGeFrame::ID_STATICBITMAP1 = wxNewId();
const long ProTeGeFrame::ID_STATICTEXT1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ProTeGeFrame,wxFrame)
    //(*EventTable(ProTeGeFrame)
    //*)
END_EVENT_TABLE()

ProTeGeFrame::ProTeGeFrame(wxWindow* parent,wxWindowID id)
{
    srand(time(0));

    //(*Initialize(ProTeGeFrame)
    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(570,568));
    Button1 = new wxButton(this, ID_BUTTON1, _("Test"), wxPoint(8,8), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    StaticBitmap1 = new wxStaticBitmap(this, ID_STATICBITMAP1, wxNullBitmap, wxPoint(16,48), wxSize(328,136), wxSIMPLE_BORDER, _T("ID_STATICBITMAP1"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Sin pattern"), wxPoint(120,16), wxDefaultSize, 0, _T("ID_STATICTEXT1"));

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ProTeGeFrame::OnButton1Click);
    //*)
}

ProTeGeFrame::~ProTeGeFrame()
{
    //(*Destroy(ProTeGeFrame)
    //*)
}

void ProTeGeFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void ProTeGeFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

int counter = 0;
greyscale * test;

void ProTeGeFrame::OnButton1Click(wxCommandEvent& event)
{
    switch(counter)
    {
    case 0:
        {
            test = new greyscale(2048);
            test->normalize_pre_marble(256,0);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("table"));
            break;
        }
    case 1:
        {
            delete test;
            test = table(16);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("dist"));
            break;
        }
    case 2:
        {
            delete test;
            test = dist(16);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("s-curve"));
            break;
        }
    case 3:
        {
            delete test;
            test = dist(16);
            test->normalize_pre_fog();
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("one iteration"));
            break;
        }
    case 4:
        {
            delete test;
            int distribution[] = {0,0,0,1,0,0,0,0,0};
            vector_field  * f = new vector_field(1024);
            test = perlin_noise(16,f);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("sum of big"));
            break;
        }
    case 5:
        {
            delete test;
            int distribution[] = {1,1,1,1,1,1,0,0,0};
            test = fractal_perlin_noise(distribution);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("fractal sum"));
            break;
        }
    case 6:
        {
            delete test;
            test = fractal_perlin_noise(2);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("deform slighty"));
            break;
        }
    case 7:
        {
            delete test;
            test = fractal_perlin_noise(2);
            test->normalize_pre_marble(256,4);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("stronger deform"));
            break;
        }
    case 8:
        {
            delete test;
            test = fractal_perlin_noise(2);
            test->normalize_pre_marble(256,12);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("wood"));
            break;
        }
    case 9:
        {
            delete test;
            test = fractal_perlin_noise(2);
            test->normalize_pre_wood(45,8);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("paint"));
            break;
        }
    case 10:
        {
            test->set_image(StaticBitmap1, 650,220,190,40,60,40,12);
            StaticText1->SetLabel(wxT("sin one line"));
            break;
        }
    case 11:
        {
            delete test;
            test = new greyscale(2048);
            test->normalize_pre_marble(1024,0);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("small deform"));
            break;
        }
    case 12:
        {
            delete test;
            test = fractal_perlin_noise(2);
            test->normalize_pre_marble(650,2);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("deform 4x"));
            break;
        }
    case 13:
        {
            delete test;
            test = fractal_perlin_noise(2);
            test->normalize_pre_marble(1024,8);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("colorize fire"));
            break;
        }
    case 14:
        {
            test->set_image(StaticBitmap1, 650,255,235,0,190,0,0);
            StaticText1->SetLabel(wxT("sum same weight"));
            break;
        }
    case 15:
        {
            delete test;
            test = fractal_perlin_noise(1);
            test->normalize();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("max contrast"));
            break;
        }
    case 16:
        {
            test->maximize_contrast();
            test->set_image(StaticBitmap1, 650);
            StaticText1->SetLabel(wxT("grass color"));
            break;
        }
    case 17:
        {
            test->set_image(StaticBitmap1, 650,20,200,40,40,100,20);
            StaticText1->SetLabel(wxT("render"));
            break;
        }
    case 18:
        {
            wxImage * demo = new wxImage();
            demo->LoadFile(wxT("/home/hase/demo.png"));
            wxBitmap * temp_wxbitmap = new wxBitmap(*demo);
            StaticBitmap1->SetBitmap(*demo);
            delete temp_wxbitmap;
            delete demo;
            break;
        }
    }

    counter ++;
}
