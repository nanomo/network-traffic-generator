//---------------------------------------------------------------------------

#include <clx.h>
#pragma hdrstop
#include <wchar.h>
#include "traffic_server.h"
#include "about.h"
//#include <dlfcn.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.xfm"
TTrafficServerForm *TrafficServerForm;
//---------------------------------------------------------------------------
__fastcall TTrafficServerForm::TTrafficServerForm(TComponent* Owner)
        : TForm(Owner)
{
 PortEdit->Value=11111;
 sr=NULL;
 sp=NULL;
 ss=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TTrafficServerForm::QuitBitBtnClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TTrafficServerForm::AddBitBtnClick(TObject *Sender)
{
 if (KnownProtocolsListBox->ItemIndex!=-1 && PortEdit->Value!=0)
   {
    String S;
    S=KnownProtocolsListBox->Items->Strings[KnownProtocolsListBox->ItemIndex];
    S+=String(":");
    S+=PortEdit->Value;//PortEdit->CleanText;
    for (int cnt=0;cnt<ProtocolsListBox->Items->Count;cnt++)
      { // do not allow duplicates
       if (ProtocolsListBox->Items->Strings[cnt]==S) return;
      }
    ProtocolsListBox->Items->Add( S );
   }

}
//---------------------------------------------------------------------------
void __fastcall TTrafficServerForm::RemoveBitBtnClick(TObject *Sender)
{
 if (ProtocolsListBox->ItemIndex!=-1)
   {
    ProtocolsListBox->Items->Delete( ProtocolsListBox->ItemIndex );
   }
}
//---------------------------------------------------------------------------
void __fastcall TTrafficServerForm::ClearBitBtnClick(TObject *Sender)
{
  ProtocolsListBox->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TTrafficServerForm::StartButtonClick(TObject *Sender)
{
 if (!ProtocolsListBox->Items->Count)
    {
     Application->MessageBox( "Please select protocol and port combination to use", Caption, TMessageButtons() << smbOK, smsWarning );
     return;
    }
 if (ReplyComboBox->ItemIndex==-1)
    {
     Application->MessageBox( "Please select reply type to use", Caption , TMessageButtons() << smbOK, smsWarning  );
     return;
    }
 if (ServerComboBox->ItemIndex==-1)
    {
     Application->MessageBox( "Please select server type to use", Caption, TMessageButtons() << smbOK, smsWarning );
     return;
    }
 StartButton->Enabled=False;
 StopButton->Enabled=True;
 SettingsPanel->Enabled=False;
 sr=allocate_reply();
 if (!sr)
   {
    StartButton->Enabled=True;
    StopButton->Enabled=False;
    return;
   }
 InitNetwork();
 sp=allocate_protocol();
 if (!sp)
   {
    StartButton->Enabled=True;
    StopButton->Enabled=False;
    cleanup( NULL, sr, NULL );
    sr=NULL;
    return;
   }
 ss=allocate_servers( sr, sp );
 if (!ss)
   {
    StartButton->Enabled=True;
    StopButton->Enabled=False;
    cleanup( NULL, sr, sp );
    sr=NULL;
    sp=NULL;
    return;
   }
 ActivityTimer->Enabled=True;
 ss->serv->StartServer( ss->handle );
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
struct server_reply *TTrafficServerForm::allocate_reply()
{
  struct server_reply *tmp;
  char *tmpc;
  char err_msg[1024];
  char **argv;
  tmp=new struct server_reply();
  if (!tmp)
    {
      Application->MessageBox( "Out of memory", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }
  argv=new char *[2];
  argv[0]=new char( 0 );
  argv[1]=new char[1024];
  strcpy( argv[1], ReplyComboBox->Items->Strings[ReplyComboBox->ItemIndex].c_str()
 );
  tmpc=set_library_path( 2, argv, 1, -1, "reply", err_msg, 1024 );
  delete argv[0];
  delete []argv[1];
  delete []argv;
  if (!tmpc)
    {
      Application->MessageBox( err_msg, Caption, TMessageButtons() << smbOK, smsWarning  );
      return( NULL );
    }
  tmp->rep=new reply( tmpc );
  delete []tmpc;
  if (!tmp->rep)
    {
      Application->MessageBox( "Out of memory", Caption, TMessageButtons() << smbOK, smsWarning  );
      return( NULL );
    }
  if (!tmp->rep->isOk())
    {
      Application->MessageBox( "Unable to load reply library", Caption, TMessageButtons() << smbOK, smsWarning );
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

struct client_protocol *TTrafficServerForm::allocate_protocol()
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
      Application->MessageBox( "Unable to allocate server protocol", Caption, TMessageButtons() << smbOK, smsWarning );
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
    nport=atol( port );
    tmpc=set_library_path( 2, argv, 1, -1, "protocol", err_msg, 1024 );
    delete argv[0];
    delete []argv[1];
    delete []argv;
    if (!tmpc)
       {
        Application->MessageBox( err_msg, Caption, TMessageButtons() << smbOK, smsWarning );
        return( NULL );
       }
    std::map <char *,struct client_protocol *,cpc_map>::iterator cur = known_prot.
find( tmpc );

      tmp[cnt].proto=NULL;
      tmp[cnt].proto_handle=NULL;
      tmp[cnt].connection=NULL;
      if (cur==known_prot.end())
        { // first time this protocol come's up...
          tmp[cnt].proto=new protocol( tmpc );
          if (!tmp[cnt].proto)
            {
              Application->MessageBox( "Unable to allocate memory for protocol", Caption, TMessageButtons() << smbOK, smsWarning );
            }
          else
            {
              if (!tmp[cnt].proto->isOk())
                {
                  Application->MessageBox( "Unable to initialize protocol", Caption, TMessageButtons() << smbOK, smsWarning );
                  delete tmp[cnt].proto;
                  tmp[cnt].proto=NULL;
                }
              else
                {
                  tmp[cnt].proto_handle=tmp[cnt].proto->InitProtocol( 1 );
                  if (!tmp[cnt].proto_handle)
                    {
                     Application->MessageBox( "Unable to start server for protocol", Caption, TMessageButtons() << smbOK, smsWarning  );
                    }
                  else
                    {
                      tmp[cnt].connection=tmp[cnt].proto->ListenForConnections( tmp[cnt].proto_handle,
                                                                                nport,
                                                                                10, NULL );
                      if (!tmp[cnt].connection)
                                {
                                  Application->MessageBox( "Unable to listen for protocol", Caption, TMessageButtons() << smbOK, smsWarning );
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
             Application->MessageBox( "Unable to listen for protocol", Caption, TMessageButtons() << smbOK, smsWarning );
            }
        }
    }
  if (tmpc) delete []tmpc;
  tmpc=NULL;
  return( tmp );
}

struct server_server *TTrafficServerForm::allocate_servers( struct server_reply *sr, struct client_protocol *sp )
{
  char *tmpc;
  char err_msg[1024];
  char **argv;
  struct server_server *tmp;
  tmp=new struct server_server();
  if (!tmp)
    {
      Application->MessageBox( "Unable to allocate server", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }
  argv=new char *[2];
  argv[0]=new char( 0 );
  argv[1]=new char[1024];
  strcpy( argv[1], ServerComboBox->Items->Strings[ServerComboBox->ItemIndex].c_str
() );
  tmpc=set_library_path( 2, argv, 1, -1, "server", err_msg, 1024 );
  delete argv[0];
  delete []argv[1];
  delete []argv;
  if (!tmpc)
       {
        Application->MessageBox( err_msg, Caption, TMessageButtons() << smbOK, smsWarning );
        return( NULL );
       }
  tmp->serv=new server( tmpc );
  delete []tmpc;
  if (!tmp->serv)
    {
      Application->MessageBox( "Unable to allocate server library", Caption, TMessageButtons() << smbOK, smsWarning  );
      delete tmp;
      return( NULL );
    }
  if (!tmp->serv->isOk())
    {
      Application->MessageBox( "Unable to initialize server library", Caption, TMessageButtons() << smbOK, smsWarning );
      delete tmp->serv;
      delete tmp;
      return( NULL );
    }
  tmp->handle=tmp->serv->InitServer( &sp, ProtocolsListBox->Items->Count, &sr, 1 );
  if (!tmp->handle)
    {
      delete tmp->serv;
      delete tmp;
      Application->MessageBox( "Unable to initialize server", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }
  return( tmp );
}

void TTrafficServerForm::cleanup( struct server_server *ss,
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
                      sp[cnt2].proto->DestroyConnection( sp[cnt2].proto_handle, sp
[cnt2].connection, 0 );
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
void __fastcall TTrafficServerForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 cleanup( ss, sr, sp );
}
//---------------------------------------------------------------------------
void __fastcall TTrafficServerForm::StopButtonClick(TObject *Sender)
{
 ActivityTimer->Enabled=False;
 if (ss && ss->serv && ss->handle)
    ss->serv->StopServer( ss->handle );
 cleanup( ss, sr, sp );
 ss=NULL;
 sr=NULL;
 sp=NULL;
 SettingsPanel->Enabled=True;
 StartButton->Enabled=True;
 StopButton->Enabled=False;
}
//---------------------------------------------------------------------------
void __fastcall TTrafficServerForm::ActivityTimerTimer(TObject *Sender)
{
 if (ss && ss->serv && ss->handle)
   {
    LONGLONG rd, we;
    ss->serv->ServerTransferedData( ss->handle, &rd, &we );
    AnsiString S( rd );
    AnsiString T( we );
    BytesReadLabel->Caption=S;
    BytesWrittenLabel->Caption=T;
   }
}
//---------------------------------------------------------------------------
void __fastcall TTrafficServerForm::AboutButtonClick(TObject *Sender)
{
 AboutBox->ProductName->Caption="Traffic Server 0.1.3";
 AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

