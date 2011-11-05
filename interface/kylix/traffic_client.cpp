//---------------------------------------------------------------------------

#include <clx.h>
#pragma hdrstop

#include "traffic_client.h"
#include "about.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.xfm"
TTrafficClientForm *TrafficClientForm;
//---------------------------------------------------------------------------
__fastcall TTrafficClientForm::TTrafficClientForm(TComponent* Owner)
        : TForm(Owner)
{
 cc=NULL;
 cp=NULL;
 cd=NULL;
 ProtocolPortEdit->Value=11111;
 PayloadComboBoxChange(Owner);
 ClientComboBoxChange(Owner);
}
//---------------------------------------------------------------------------
void __fastcall TTrafficClientForm::QuitButtonClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::AboutButtonClick(TObject *Sender)
{
 AboutBox->ProductName->Caption="Traffic Client 0.1.3";
 AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::PayloadComboBoxChange(TObject *Sender)
{
 PayloadMinEdit->Enabled=False;
 PayloadMaxEdit->Enabled=False;
 PayloadTimeEdit->Enabled=False;
 PayloadIncrementEdit->Enabled=False;
 if (PayloadComboBox->Items->Strings[PayloadComboBox->ItemIndex]=="fixed")
   {
    PayloadMaxEdit->Enabled=True;
   }
 if (PayloadComboBox->Items->Strings[PayloadComboBox->ItemIndex]=="increasing")
   {
    PayloadMinEdit->Enabled=True;
    PayloadMaxEdit->Enabled=True;
    PayloadTimeEdit->Enabled=True;
    PayloadIncrementEdit->Enabled=True;
   }
 if (PayloadComboBox->Items->Strings[PayloadComboBox->ItemIndex]=="random")
   {
    PayloadMinEdit->Enabled=True;
    PayloadMaxEdit->Enabled=True;
    PayloadTimeEdit->Enabled=True;
   }
}
//---------------------------------------------------------------------------


void __fastcall TTrafficClientForm::ProtocolComboBoxChange(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::ClientMaxEditChange(TObject *Sender)
{
 if (String(ClientMaxEdit->Text).ToIntDef( 0 )<1)
    {
      Application->MessageBox( "This must be a number larger than 0", Caption, TMessageButtons() << smbOK, smsWarning );
      return;
    }
 NumConnectionsEdit->Value=String(ClientMaxEdit->Text).ToIntDef(0);
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::NumConnectionsEditChanged(
      TObject *Sender, int NewValue)
{
 ClientMaxEdit->Text=NumConnectionsEdit->CleanText;
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::ClientComboBoxChange(TObject *Sender)
{
 ClientMinEdit->Enabled=False;
 ClientIncrementEdit->Enabled=False;
 ClientTimeEdit->Enabled=False;
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::StartBitBtnClick(TObject *Sender)
{
  SettingsPanel->Enabled=False;
  StopBitBtn->Enabled=True;
  StartBitBtn->Enabled=False;
  if (ProtocolComboBox->ItemIndex==-1)
    {
     Application->MessageBox( "Please select protocol to use", Caption, TMessageButtons() << smbOK, smsWarning );
     SettingsPanel->Enabled=True;
     StopBitBtn->Enabled=False;
     StartBitBtn->Enabled=True;
     return;
    }
  if (ClientComboBox->ItemIndex==-1)
    {
     SettingsPanel->Enabled=True;
     StopBitBtn->Enabled=False;
     StartBitBtn->Enabled=True;
     Application->MessageBox( "Please select client type to use", Caption, TMessageButtons() << smbOK, smsWarning );
     return;
    }
  if (PayloadComboBox->ItemIndex==-1)
    {
     SettingsPanel->Enabled=True;
     StopBitBtn->Enabled=False;
     StartBitBtn->Enabled=True;
     Application->MessageBox( "Please select payload type to use", Caption, TMessageButtons() << smbOK, smsWarning );
     return;
    }
  if (String(NumConnectionsEdit->Text).ToIntDef( 0 )<1)
    {
     SettingsPanel->Enabled=True;
     StopBitBtn->Enabled=False;
     StartBitBtn->Enabled=True;
     Application->MessageBox( "More than one connection should be specified", Caption, TMessageButtons() << smbOK, smsWarning );
     return;
    }
  cp=allocate_payload();
  if (!cp)
    {
     SettingsPanel->Enabled=True;
     StopBitBtn->Enabled=False;
     StartBitBtn->Enabled=True;
     return;
    }
  InitNetwork();
  cd=allocate_protocol();
  if (!cd)
    {
     cleanup( cp, cd, cc );
     cp=NULL;
     cd=NULL;
     cc=NULL;
     SettingsPanel->Enabled=True;
     StopBitBtn->Enabled=False;
     StartBitBtn->Enabled=True;
     return;
    }
  cc=allocate_clients( cd, String(NumConnectionsEdit->Text).ToIntDef( 0 ), cp, String(NumConnectionsEdit->Text).ToIntDef( 0 ) );
  if (!cc)
    {
     cleanup( cp, cd, cc );
     cp=NULL;
     cd=NULL;
     cc=NULL;
     SettingsPanel->Enabled=True;
     StopBitBtn->Enabled=False;
     StartBitBtn->Enabled=True;
     return;
    }
  cc->clnt->StartClient( cc->handle );
  ActivityTimer->Enabled=true;
}
//---------------------------------------------------------------------------
struct client_payload *TTrafficClientForm::allocate_payload()
{
  struct client_payload *tmp;
  payload *mypayload;
  char *tmpc;
  char err_msg[1024];
  char **argv;

  tmp=new struct client_payload[String(NumConnectionsEdit->Text).ToIntDef( 0 )];
  if (!tmp)
    {
      Application->MessageBox( "Out of memory loading payloads", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }

  argv=new char *[2];
  argv[0]=new char( 0 );
  argv[1]=new char[1024];
  strcpy( argv[1], PayloadComboBox->Items->Strings[PayloadComboBox->ItemIndex].c_str() );
  tmpc=set_library_path( 2, argv, 1, -1, "payload", err_msg, 1024 );
  delete argv[0];
  delete []argv[1];
  delete []argv;
  if (!tmpc)
    {
      Application->MessageBox( err_msg, Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }

  mypayload=new payload( tmpc );
  delete []tmpc;
  if (!mypayload)
    {
      Application->MessageBox( "Out of memory loading payload library", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }
  if (!mypayload->isOk())
    {
      delete mypayload;
      Application->MessageBox( "Unable to load payload library", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }
  for (int cnt=0;cnt<String(NumConnectionsEdit->Text).ToIntDef( 0 );cnt++)
    {
      void *myhandle;

      myhandle=mypayload->InitializePayload( String(PayloadMinEdit->Text).ToIntDef( 0 ),
                                             String(PayloadMaxEdit->Text).ToIntDef( 0 ),
                                             String(PayloadIncrementEdit->Text).ToIntDef( 0 ),
                                             String(PayloadTimeEdit->Text).ToIntDef( 0 ), NULL );
      if (!myhandle)
        {
      Application->MessageBox( "Unable to initialize payload ", Caption, TMessageButtons() << smbOK, smsWarning );
          for (int cnt2=0;cnt2<cnt;cnt2++)
            {
              mypayload->DestroyPayload( tmp[cnt].handle );
            }
          delete tmp[0].pld;
          return( NULL );
        }
      else
        {
          tmp[cnt].pld=mypayload;
          tmp[cnt].handle=myhandle;
        }
    }
  return( tmp );
}
//---------------------------------------------------------------------------
struct client_protocol *TTrafficClientForm::allocate_protocol()
{
  struct client_protocol *tmp;
  protocol *myproto;
  void *my_proto_handle;
  char *tmpc;
  char err_msg[1024];
  char **argv;

  tmp=new struct client_protocol[String(NumConnectionsEdit->Text).ToIntDef( 0 )];
  if (!tmp)
    {
      Application->MessageBox( "Out of memory loading protocol", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }

  argv=new char *[2];
  argv[0]=new char( 0 );
  argv[1]=new char[1024];
  strcpy( argv[1], ProtocolComboBox->Items->Strings[ProtocolComboBox->ItemIndex].c_str() );
  tmpc=set_library_path( 2, argv, 1, -1, "protocol", err_msg, 1024 );
  delete argv[0];
  delete []argv[1];
 delete []argv;
  if (!tmpc)
    {
      Application->MessageBox( err_msg, Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }
  myproto=new protocol( tmpc );
  delete []tmpc;
  if(!myproto)
    {
      Application->MessageBox( "Out of memory loading protocol library", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }
  if (!myproto->isOk())
    {
      delete myproto;
      Application->MessageBox( "Unable to load protocol library", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }
  my_proto_handle=myproto->InitProtocol( 0 ); // client
  if (!my_proto_handle)
    {
      delete myproto;
      Application->MessageBox( "Unable to initialize protocol", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }
  for (int cnt=0;cnt<String(NumConnectionsEdit->Text).ToIntDef( 0 );cnt++)
    {
      void *myconnection;

      myconnection=myproto->CreateConnection( my_proto_handle,
                                              String(ProtocolAddressEdit->Text).c_str(),
                                              String(ProtocolPortEdit->Text).ToIntDef( 0 ),
                                              "0.0.0.0",
                                              0,
                                              NULL );
      if (myconnection)
        {
          tmp[cnt].proto=myproto;
          tmp[cnt].proto_handle=my_proto_handle;
          tmp[cnt].connection=myconnection;
        }
      else
        {
      Application->MessageBox( "Unable to create connection", Caption, TMessageButtons() << smbOK, smsWarning );
/*      for (int cnt2=0;cnt2<cnt;cnt++)
        {
          myproto->DestroyConnection( my_proto_handle, tmp[cnt2].connection, 0 );
        }*/
//          myproto->DestroyProtocol( my_proto_handle, 0 );
          delete myproto;
      delete []tmp;
      return( NULL );
        }
    }
  return( tmp );
}
//---------------------------------------------------------------------------
struct client_client *TTrafficClientForm::allocate_clients( struct client_protocol *proto, int num_proto, struct client_payload *pld, int num_payload )
{
  struct client_client *tmp;
  client *myclient;
  char *tmpc;
  char err_msg[1024];
  char **argv;

  tmp=new client_client();
  if (!tmp)
    {
      Application->MessageBox( "Out of memory loading client", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }

  argv=new char *[2];
  argv[0]=new char( 0 );
  argv[1]=new char[1024];
  strcpy( argv[1], ClientComboBox->Items->Strings[ClientComboBox->ItemIndex].c_str() );
  tmpc=set_library_path( 2, argv, 1, -1, "client", err_msg, 1024 );
  delete argv[0];
  delete []argv[1];
  delete []argv;
  if (!tmpc)
    {
      Application->MessageBox( err_msg, Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
    }
  myclient=new client( tmpc );
  delete []tmpc;
  if (!myclient)
    {
      Application->MessageBox( "Out of memory loading client instance", Caption, TMessageButtons() << smbOK, smsWarning );
      return( NULL );
   }
  if (!myclient->isOk())
    {
      Application->MessageBox( "Unable to initialize client instance", Caption, TMessageButtons() << smbOK, smsWarning );
      delete myclient;
      return( NULL );
    }
  tmp->clnt=myclient;
  tmp->handle=myclient->InitClient( proto, num_proto, pld, num_payload,
                                    String(ClientMinEdit->Text).ToIntDef( 0 ),
                                    String(ClientMaxEdit->Text).ToIntDef( 0 ),
                                    String(ClientIncrementEdit->Text).ToIntDef( 0 ),
                                    String(ClientTimeEdit->Text).ToIntDef( 0 ) );
  if (!tmp->handle)
    {
      Application->MessageBox( "Unable to initialize client", Caption, TMessageButtons() << smbOK, smsWarning );
      delete myclient;
      return( NULL );
    }
  return( tmp );
}
//---------------------------------------------------------------------------
void TTrafficClientForm::cleanup( struct client_payload *payload, struct client_protocol *protocol, struct client_client *client )
{
  if (payload)
    {
      for (int cnt=0;cnt<String(NumConnectionsEdit->Text).ToIntDef( 0 );cnt++)
        {
          if (payload[cnt].pld && payload[cnt].handle)
            {
              payload[cnt].pld->DestroyPayload( payload[cnt].handle );
              payload[cnt].handle=NULL;
            }
        }
      if (payload[0].pld)
        {
          delete payload[0].pld;
          payload[0].pld=NULL;
        }
      delete []payload;
      payload=NULL;
    }
  if (protocol)
    {
      for (int cnt=0;cnt<String(ClientMaxEdit->Text).ToIntDef( 0 );cnt++)
        {
          if (protocol[cnt].connection && protocol[cnt].proto_handle && protocol[cnt].proto)
            {
              protocol[cnt].proto->DestroyConnection( protocol[cnt].proto_handle, protocol[cnt].connection ,0 );
              protocol[cnt].connection=NULL;
            }
        }
      if (protocol[0].proto_handle && protocol[0].proto)
        {
          protocol[0].proto->DestroyProtocol( protocol[0].proto_handle, 0 );
          protocol[0].proto_handle=NULL;
        }
      if (protocol[0].proto)
        {
          delete protocol[0].proto;
        }
      delete []protocol;
      protocol=NULL;
    }
  if (client)
    {
      if (client->clnt && client->handle)
        {
          client->clnt->DestroyClient( client->handle );
          client->handle=NULL;
        }
      if (client->clnt)
        {
          delete client->clnt;
          client->clnt=NULL;
        }
    }
  ShutdownNetwork();
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 cleanup( cp, cd, cc );
 cp=NULL;
 cd=NULL;
 cc=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::ActivityTimerTimer(TObject *Sender)
{
 if (cc && cc->clnt && cc->handle)
   {
    LONGLONG rd, wt;
    cc->clnt->GetClientDataTransfered( cc->handle, &rd, &wt );
    AnsiString S( rd );
    AnsiString T( wt );
    DataReadLabel->Caption=S;
    DataWrittenLabel->Caption=T;
   }
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::PayloadHelpBitBtnClick(TObject *Sender)
{
 if (PayloadComboBox->Items->Strings[PayloadComboBox->ItemIndex]=="fixed")
   {
    Application->MessageBox( "This payload will return a fixed size packet to be sent to the server.\n\n"
                             "Payloads normally accept minimum, maximum, increment and time as parameters.\n\n"
                             "The fixed payload ignores all of these parameters except maximum to\n"
                             "determine the size of the payload the send. This maximum should be at least 8."
                             , (Caption + " Fixed Payload Help"),  TMessageButtons() << smbOK, smsWarning  );
   }
 if (PayloadComboBox->Items->Strings[PayloadComboBox->ItemIndex]=="increasing")
   {
    Application->MessageBox( "This payload will return an increasing sized packet to be sent to the server.\n\n"
                             "The packet will increase by a specified increment starting from a specified\n"
                             "minimum to a specified maximum. Once it reaches that maximum it will keep on\n"
                             "sending that sized payload. Each size packet, except the maximum sized\n"
                             "packet will be sent a specified number of times.\n\n"
                             "Payloads normally accept minimum, maximum, increment and time as parameters.\n\n"
                             "The payload starts with the minimum sized packet. This must be at least 8\n"
                             "bytes in size and must be less than the maximum supplied. It will send this\n"
                             "packet size [time] times before it will add the increment to it. If adding\n"
                             "the increment to it makes it bigger than the maximum specified size, the\n"
                             "maximum sized packet will be sent."
                            , (Caption + " Increasing Payload Help"),  TMessageButtons() << smbOK, smsWarning  );
   }
 if (PayloadComboBox->Items->Strings[PayloadComboBox->ItemIndex]=="random")
   {
    Application->MessageBox( "This payload will return a random size packet to be sent to the server.\n\n"
                             "Payloads normally accept minimum, maximum, increment and time as parameters.\n\n"
                             "The random payload will select a random size between the minimum and maximum\n"
                             "specified and will return this sized payload [time] times. The increment\n"
                             "parameter is ignored."
                             , (Caption + " Random Payload Help"),  TMessageButtons() << smbOK, smsWarning  );
   }
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::PayloadComboBoxClick(TObject *Sender)
{
 PayloadComboBoxChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::ClientComboBoxClick(TObject *Sender)
{
  ClientComboBoxChange( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::ClientHelpBitBtnClick(TObject *Sender)
{
 Application->MessageBox(  "This is the number of simultatious connections that should\n"
                            "be made to the server using the specified protocol and payload.\n"
                            "This value is synchronized with the Client Maximum value"
                           , (Caption + " Connections Help"), TMessageButtons() << smbOK, smsWarning  );
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::StopBitBtnClick(TObject *Sender)
{
  SettingsPanel->Enabled=True;
  StopBitBtn->Enabled=False;
  StartBitBtn->Enabled=True;
  if (cc && cc->clnt && cc->handle)
    cc->clnt->StopClient( cc->handle );
  cleanup( cp, cd, cc );
  cp=NULL;
  cd=NULL;
  cc=NULL;
  ActivityTimer->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::ProtocolHelpBitBtnClick(
      TObject *Sender)
{
  Application->MessageBox(  "Currently the following protocols are supported:\n"
                            "IPV4 TCP with port ranges from 1 - 65534\n"
                            "IPV4 UDP with port ranges from 1 - 65534\n\n"
                            "The address can be specified either as a name\n"
                            "or as an IP address"
                           , (Caption + " Protocol Help"), TMessageButtons() << smbOK, smsWarning );
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::BitBtn1Click(TObject *Sender)
{
  Application->MessageBox(  "This is the number of simultatious connections that should\n"
                            "be made to the server using the specified protocol and payload.\n"
                            "This value is synchronized with the Client Maximum value"
                           , (Caption + " Connections Help"), TMessageButtons() << smbOK, smsWarning );
}
//---------------------------------------------------------------------------

