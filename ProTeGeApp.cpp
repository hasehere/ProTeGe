/***************************************************************
 * Name:      ProTeGeApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Hase ()
 * Created:   2015-03-02
 * Copyright: Hase ()
 * License:
 **************************************************************/

#include "ProTeGeApp.h"

//(*AppHeaders
#include "ProTeGeMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(ProTeGeApp);

bool ProTeGeApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	ProTeGeFrame* Frame = new ProTeGeFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
