/***************************************************************
 * Name:      ProTeGeMain.h
 * Purpose:   Defines Application Frame
 * Author:    Hase ()
 * Created:   2015-03-02
 * Copyright: Hase ()
 * License:
 **************************************************************/

#ifndef PROTEGEMAIN_H
#define PROTEGEMAIN_H

//(*Headers(ProTeGeFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>
//*)

class ProTeGeFrame: public wxFrame
{
    public:

        ProTeGeFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~ProTeGeFrame();

    private:

        //(*Handlers(ProTeGeFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton2Click(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnClose1(wxCloseEvent& event);
        //*)

        //(*Identifiers(ProTeGeFrame)
        static const long ID_BUTTON1;
        static const long ID_STATICBITMAP1;
        static const long ID_STATICTEXT1;
        //*)

        //(*Declarations(ProTeGeFrame)
        wxButton* Button1;
        wxStaticText* StaticText1;
        wxStaticBitmap* StaticBitmap1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // PROTEGEMAIN_H
