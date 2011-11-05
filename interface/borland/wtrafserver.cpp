//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("wtrafserver.res");
USEFORM("server_gui.cpp", ServerGuiForm);
USELIB("loadreply.lib");
USELIB("loadprotocol.lib");
USELIB("loadserver.lib");
USELIB("libos.lib");
USEFORM("about.cpp", AboutBox);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "Traffic Server";
         Application->CreateForm(__classid(TServerGuiForm), &ServerGuiForm);
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
