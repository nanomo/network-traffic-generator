//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "about.h"
#if defined(_ISCLIENT)
#include "client_gui.h"
#endif
#if defined(_ISSERVER)
#include "server_gui.h"
#endif
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//--------------------------------------------------------------------- 
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TAboutBox::OKButtonClick(TObject *Sender)
{
  AboutBox->Visible=false;
#if defined(_ISCLIENT)
  TrafficClientForm->Enabled=true;
#else
#if defined(_ISSERVER)
  ServerGuiForm->Enabled=true;
#else
#error This must be defined
#endif
#endif
}
//---------------------------------------------------------------------------

