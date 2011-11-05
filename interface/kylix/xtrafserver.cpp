//---------------------------------------------------------------------------

#include <clx.h>
#pragma hdrstop
USEFORM("traffic_server.cpp", TrafficServerForm);
USEFORM("about.cpp", AboutBox);
//---------------------------------------------------------------------------
int main(void)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Traffic Server";
                 Application->CreateForm(__classid(TTrafficServerForm), &TrafficServerForm);
                 Application->CreateForm(__classid(TAboutBox), &AboutBox);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch(...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch(Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
