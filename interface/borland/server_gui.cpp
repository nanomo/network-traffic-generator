//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "server_gui.h"
#include "about.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TServerGuiForm *ServerGuiForm;
//---------------------------------------------------------------------------
__fastcall TServerGuiForm::TServerGuiForm(TComponent* Owner)
    : TForm(Owner)
{
 ServerComboBox->ItemIndex = 0;
 ReplyComboBox->ItemIndex = 0;
 sr=NULL;
 sp=NULL;
 ss=NULL;
 PortUpDown->Position=10;
}
//---------------------------------------------------------------------------
void __fastcall TServerGuiForm::PortUpDownClick(TObject *Sender,
      TUDBtnType Button)
{
 int val;
 switch (Button)
  {
  case btNext:
    val=PortEdit->Text.ToIntDef( 0 );
    val+=1;
    if (val<1) val=1;
    if (val>65534) val=65534;
    PortEdit->Text=val;
    break;
  case btPrev:
    val=PortEdit->Text.ToIntDef( 0 );
    val-=1;
    if (val<1) val=1;
    if (val>65534) val=65534;
    PortEdit->Text=val;
    break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TServerGuiForm::AddButtonClick(TObject *Sender)
{
 if (KnownProtocolsListBox->ItemIndex!=-1 && PortEdit->Text.ToIntDef( 0 )!=0)
   {
    String S;
    S=KnownProtocolsListBox->Items->Strings[KnownProtocolsListBox->ItemIndex];
    S+=":";
    S+=PortEdit->Text;
    for (int cnt=0;cnt<ProtocolsListBox->Items->Count;cnt++)
      { // do not allow duplicates
       if (ProtocolsListBox->Items->Strings[cnt]==S) return;
      }
    ProtocolsListBox->Items->Add( S );
   }
}
//---------------------------------------------------------------------------
void __fastcall TServerGuiForm::ClearButtonClick(TObject *Sender)
{
  ProtocolsListBox->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TServerGuiForm::RemoveButtonClick(TObject *Sender)
{
 if (ProtocolsListBox->ItemIndex!=-1)
   {
    ProtocolsListBox->Items->Delete( ProtocolsListBox->ItemIndex );
   }
}
//---------------------------------------------------------------------------
void __fastcall TServerGuiForm::StartButtonClick(TObject *Sender)
{ // This does all the real work.
 // D 1. Check that there are protocols
 // D 2. Check that there is a reply and it can be loaded
 // D 3. Check that there is a server
 // D 4. Load the protocols
 // D 5. Load the server
 // D 6. Enable Stop
 // D 7. Disable Start
 // 8. Update Count
 if (!ProtocolsListBox->Items->Count)
    {
     Application->MessageBox( "Please select protocol and port combination to use", Caption.c_str(), MB_OK );
     return;
    }
 if (ReplyComboBox->ItemIndex==-1)
    {
     Application->MessageBox( "Please select reply type to use", Caption.c_str(), MB_OK );
     return;
    }
 if (ServerComboBox->ItemIndex==-1)
    {
     Application->MessageBox( "Please select server type to use", Caption.c_str(), MB_OK );
     return;
    }
// ServerComboBox->Enabled=False;
// ReplyComboBox->Enabled=False;
// AddButton->Enabled=False;
// RemoveButton->Enabled=False;
// ClearButton->Enabled=False;
 StartButton->Enabled=False;
 StopButton->Enabled=True;
 SettingsPanel->Enabled=False;
 sr=allocate_reply();
 if (!sr) return;
 InitNetwork();
 sp=allocate_protocol();
 if (!sp)
   {
    cleanup( NULL, sr, NULL );
    sr=NULL;
    return;
   }
 ss=allocate_servers( sr, sp );
 if (!ss)
   {
    cleanup( NULL, sr, sp );
    sr=NULL;
    sp=NULL;
    return;
   }
 ActivityTimer->Enabled=True;
 ss->serv->StartServer( ss->handle );
}
//---------------------------------------------------------------------------
struct server_reply *TServerGuiForm::allocate_reply()
{
  struct server_reply *tmp;
  char *tmpc;
  char err_msg[1024];
  char **argv;
  tmp=new struct server_reply();
  if (!tmp)
    {
      Application->MessageBox( "Out of memory", Caption.c_str(), MB_OK );
      return( NULL );
    }
  argv=new char *[2];
  argv[0]=new char( 0 );
  argv[1]=new char[1024];
  strcpy( argv[1], ReplyComboBox->Items->Strings[ReplyComboBox->ItemIndex].c_str() );
  tmpc=set_library_path( 2, argv, 1, -1, "reply", err_msg, 1024 );
  delete argv[0];
  delete []argv[1];
  delete []argv;
  if (!tmpc)
    {
      Application->MessageBox( err_msg, Caption.c_str(), MB_OK );
      return( NULL );
    }
  tmp->rep=new reply( tmpc );
  delete []tmpc;
  if (!tmp->rep)
    {
      Application->MessageBox( "Out of memory", Caption.c_str(), MB_OK );
      return( NULL );
    }
  if (!tmp->rep->isOk())
    {
      Application->MessageBox( "Unable to load reply library", Caption.c_str(), MB_OK );
      delete tmp;
      return( NULL );
    }
  return( tmp );
}

struct cpc_map
{
  bool operator()( const char *s1, const char *s2 )
  {
    return strcmp(s1, s2) < 0;
  }
};

struct client_protocol *TServerGuiForm::allocate_protocol()
{
  std::map <char *,struct client_protocol *,cpc_map> known_prot;
  struct client_protocol *tmp;
  char *tmpc;
  char err_msg[1024];
  char **argv;
  char *port;
  int nport;

  tmp=new struct client_protocol[ProtocolsListBox->Items->Count];
  if (!tmp)
    {
      Application->MessageBox( "Unable to allocate server protocol", Caption.c_str(), MB_OK );
      return( NULL );
    }
  tmpc=NULL;
  for (int cnt=0;cnt<ProtocolsListBox->Items->Count;cnt++)
    {
    if (tmpc) delete []tmpc;
    tmpc=NULL;
    argv=new char *[2];
    argv[0]=new char(0);
    argv[1]=new char[1024];
    strcpy( argv[1], ProtocolsListBox->Items->Strings[cnt].c_str() );
    *(strrchr( argv[1], ':' ))=0;
    port=strchr( ProtocolsListBox->Items->Strings[cnt].c_str(), ':' )+1;
    nport=atoi( port );
    tmpc=set_library_path( 2, argv, 1, -1, "protocol", err_msg, 1024 );
    delete argv[0];
    delete []argv[1];
    delete []argv;
    if (!tmpc)
       {
        Application->MessageBox( err_msg, Caption.c_str(), MB_OK );
        return( NULL );
       }
    std::map <char *,struct client_protocol *,cpc_map>::iterator cur = known_prot.find( tmpc );

      tmp[cnt].proto=NULL;
      tmp[cnt].proto_handle=NULL;
      tmp[cnt].connection=NULL;
      if (cur==known_prot.end())
    	{ // first time this protocol come's up...
    	  tmp[cnt].proto=new protocol( tmpc );
    	  if (!tmp[cnt].proto)
    	    {
              Application->MessageBox( "Unable to allocate memory for protocol", Caption.c_str(), MB_OK );
    	    }
    	  else
    	    {
    	      if (!tmp[cnt].proto->isOk())
    		{
              Application->MessageBox( "Unable to initialize protocol", Caption.c_str(), MB_OK );
    		  delete tmp[cnt].proto;
    		  tmp[cnt].proto=NULL;
    		}
	      else
    		{
    		  tmp[cnt].proto_handle=tmp[cnt].proto->InitProtocol( 1 );
    		  if (!tmp[cnt].proto_handle)
    		    {
                  Application->MessageBox( "Unable to start server for protocol", Caption.c_str(), MB_OK );
    		    }
	    	  else
    		    {
    		      tmp[cnt].connection=tmp[cnt].proto->ListenForConnections( tmp[cnt].proto_handle,
    									nport,
										10, NULL );
	    	      if (!tmp[cnt].connection)
        			{
                      Application->MessageBox( "Unable to listen for protocol", Caption.c_str(), MB_OK );
        			}
        		      else
        			{
        			  known_prot[tmpc]=&(tmp[cnt]);
        			}
		    }
		}
	    }
	}
      else
	{ // we have seen this protocol before - so just a new connection
	  tmp[cnt].proto=(known_prot[tmpc])->proto;
	  tmp[cnt].proto_handle=(known_prot[tmpc])->proto_handle;
	  tmp[cnt].connection=tmp[cnt].proto->ListenForConnections( tmp[cnt].proto_handle,
								    nport,
								    10, NULL );
	  if (!tmp[cnt].connection)
	    {
          Application->MessageBox( "Unable to listen for protocol", Caption.c_str(), MB_OK );
	    }
	}
    }
  if (tmpc) delete []tmpc;
  tmpc=NULL;
  return( tmp );
}

struct server_server *TServerGuiForm::allocate_servers( struct server_reply *sr, struct client_protocol *sp )
{
  char *tmpc;
  char err_msg[1024];
  char **argv;
  struct server_server *tmp;
  tmp=new struct server_server();
  if (!tmp)
    {
      Application->MessageBox( "Unable to allocate server", Caption.c_str(), MB_OK );
      return( NULL );
    }
  argv=new char *[2];
  argv[0]=new char( 0 );
  argv[1]=new char[1024];
  strcpy( argv[1], ServerComboBox->Items->Strings[ServerComboBox->ItemIndex].c_str() );
  tmpc=set_library_path( 2, argv, 1, -1, "server", err_msg, 1024 );
  delete argv[0];
  delete []argv[1];
  delete []argv;
  if (!tmpc)
       {
        Application->MessageBox( err_msg, Caption.c_str(), MB_OK );
        return( NULL );
       }
  tmp->serv=new server( tmpc );
  delete []tmpc;
  if (!tmp->serv)
    {
      Application->MessageBox( "Unable to allocate server library", Caption.c_str(), MB_OK );
      delete tmp;
      return( NULL );
    }
  if (!tmp->serv->isOk())
    {
      Application->MessageBox( "Unable to initialize server library", Caption.c_str(), MB_OK );
      delete tmp->serv;
      delete tmp;
      return( NULL );
    }
  tmp->handle=tmp->serv->InitServer( &sp, ProtocolsListBox->Items->Count, &sr, 1 );
  if (!tmp->handle)
    {
      delete tmp->serv;
      delete tmp;
      Application->MessageBox( "Unable to initialize server", Caption.c_str(), MB_OK );
      return( NULL );
    }
  return( tmp );
}

void TServerGuiForm::cleanup( struct server_server *ss,
	      struct server_reply *sr, struct client_protocol *sp )
{
  if (ss)
    {
      if (ss->handle && ss->serv) ss->serv->DestroyServer( ss->handle );
      if (ss->serv) delete ss->serv;
      delete ss;
      ss=NULL;
    }
  if (sr)
    {
      /*      if (sr->handle && sr->rep)
	{
	  sr->rep->DestroyReply( sr->handle );
	  } */
      if (sr->rep)
	{
	  delete sr->rep;
	}
      delete sr;
      sr=NULL;
    }
  if (sp)
    {
      for (int cnt=0;cnt<ProtocolsListBox->Items->Count;cnt++)
	{
	  struct client_protocol *tmp;

	  tmp=&(sp[cnt]);
	  if (tmp->proto && tmp->proto_handle && tmp->connection)
	    {
	      for (int cnt2=cnt+1;cnt2<ProtocolsListBox->Items->Count;cnt2++)
		{
		  if (tmp->proto_handle==sp[cnt2].proto_handle && sp[cnt2].proto_handle && sp[cnt2].connection)
		    {
		      sp[cnt2].proto->DestroyConnection( sp[cnt2].proto_handle, sp[cnt2].connection, 0 );
		      sp[cnt2].proto_handle=NULL;
		      sp[cnt2].connection=NULL;
		    }
		}
	      tmp->proto->DestroyProtocol( tmp->proto_handle, 0 );
	      delete tmp->proto;
	      tmp->proto=NULL;
	      tmp->proto_handle=NULL;
	    }
	}
      delete []sp;
      sp=NULL;
    }
  ShutdownNetwork();
}


void __fastcall TServerGuiForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 cleanup( ss, sr, sp );
}
//---------------------------------------------------------------------------



void __fastcall TServerGuiForm::ActivityTimerTimer(TObject *Sender)
{
 if (ss && ss->serv && ss->handle)
   {
    LONGLONG rd, we;
    ss->serv->ServerTransferedData( ss->handle, &rd, &we );
    BytesReadLabel->Caption=rd;
    BytesWrittenLabel->Caption=we;
   }
}
//---------------------------------------------------------------------------

void __fastcall TServerGuiForm::AboutBitBtnClick(TObject *Sender)
{
 AboutBox->ProductName->Caption="Traffic Server 0.1.3";
 AboutBox->Visible=true;
 ServerGuiForm->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TServerGuiForm::StopButtonClick(TObject *Sender)
{
 ActivityTimer->Enabled=False;
 if (ss && ss->serv && ss->handle)
    ss->serv->StopServer( ss->handle );
 cleanup( ss, sr, sp );
 ss=NULL;
 sr=NULL;
 sp=NULL;
 SettingsPanel->Enabled=True;
// ServerComboBox->Enabled=True;
// ReplyComboBox->Enabled=True;
// AddButton->Enabled=True;
// RemoveButton->Enabled=True;
// ClearButton->Enabled=True;
 StartButton->Enabled=True;
 StopButton->Enabled=False;
}
//---------------------------------------------------------------------------

void __fastcall TServerGuiForm::QuitButtonClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

