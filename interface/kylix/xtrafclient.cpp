//---------------------------------------------------------------------------

#include <clx.h>
#pragma hdrstop
USEFORM("traffic_client.cpp", TrafficClientForm);
USEFORM("about.cpp", AboutBox);
//---------------------------------------------------------------------------
int main(void)
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
