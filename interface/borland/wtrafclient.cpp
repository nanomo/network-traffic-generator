//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("wtrafclient.res");
USEFORM("client_gui.cpp", TrafficClientForm);
USELIB("loadprotocol.lib");
USELIB("loadclient.lib");
USELIB("loadpayload.lib");
USELIB("libos.lib");
USEFORM("about.cpp", AboutBox);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "Traffic Client";
         Application->CreateForm(__classid(TTrafficClientForm), &TrafficClientForm);
         Application->CreateForm(__classid(TAboutBox), &AboutBox);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
