//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "client_gui.h"
#include "about.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTrafficClientForm *TrafficClientForm;
//---------------------------------------------------------------------------
__fastcall TTrafficClientForm::TTrafficClientForm(TComponent* Owner)
    : TForm(Owner)
{
 cc=NULL;
 cp=NULL;
 cd=NULL;
 PayloadComboBox->ItemIndex=0;
 ClientComboBox->ItemIndex=0;
 ProtocolComboBox->ItemIndex=0;
 PayloadComboBoxChange(Owner);
 ClientComboBoxChange(Owner);
}
//---------------------------------------------------------------------------
void __fastcall TTrafficClientForm::QuitButtonClick(TObject *Sender)
{
  Close();    
}
//---------------------------------------------------------------------------
void __fastcall TTrafficClientForm::PortUpDownClick(TObject *Sender,
      TUDBtnType Button)
{
 int val;
 val=ProtocolPortEdit->Text.ToIntDef( 0 );
 switch (Button)
    {
     case btNext:
        val+=1;
        break;
     case btPrev:
        val-=1;
        break;
    }
 if (val<1) val=1;
 if (val>65534) val=65534;
 ProtocolPortEdit->Text=val;
}
//---------------------------------------------------------------------------
void __fastcall TTrafficClientForm::StartButtonClick(TObject *Sender)
{
  SettingsPanel->Enabled=False;
  StopBitBtn->Enabled=True;
  StartBitBtn->Enabled=False;
  if (ProtocolComboBox->ItemIndex==-1)
    {
     Application->MessageBox( "Please select protocol to use", Caption.c_str(), MB_OK );
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
     Application->MessageBox( "Please select client type to use", Caption.c_str(), MB_OK );
     return;
    }
  if (PayloadComboBox->ItemIndex==-1)
    {
     SettingsPanel->Enabled=True;
     StopBitBtn->Enabled=False;
     StartBitBtn->Enabled=True;
     Application->MessageBox( "Please select payload type to use", Caption.c_str(), MB_OK );
     return;
    }
  if (NumConnectionsEdit->Text.ToIntDef( 0 )<1)
    {
     SettingsPanel->Enabled=True;
     StopBitBtn->Enabled=False;
     StartBitBtn->Enabled=True;
     Application->MessageBox( "More than one connection should be specified", Caption.c_str(), MB_OK );
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
  cc=allocate_clients( cd, NumConnectionsEdit->Text.ToIntDef( 0 ), cp, NumConnectionsEdit->Text.ToIntDef( 0 ) );
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
  // TODO
  // D 1. Test whether protocol, client and payload set
  // D 2. Load payload
  // 2.1. InitNetwork
  // D 3. Load protocol
  // D 4. Load client
  // D 5. Start Client
  // 6. Start timer
}
//---------------------------------------------------------------------------
struct client_payload *TTrafficClientForm::allocate_payload()
{
  struct client_payload *tmp;
  payload *mypayload;
  char *tmpc;
  char err_msg[1024];
  char **argv;

  tmp=new struct client_payload[NumConnectionsEdit->Text.ToIntDef( 0 )];
  if (!tmp)
    {
      Application->MessageBox( "Out of memory loading payloads", Caption.c_str(), MB_OK );
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
      Application->MessageBox( err_msg, Caption.c_str(), MB_OK );
      return( NULL );
    }

  mypayload=new payload( tmpc );
  delete []tmpc;
  if (!mypayload)
    {
      Application->MessageBox( "Out of memory loading payload library", Caption.c_str(), MB_OK );
      return( NULL );
    }
  if (!mypayload->isOk())
    {
      delete mypayload;
      Application->MessageBox( "Unable to load payload library", Caption.c_str(), MB_OK );
      return( NULL );
    }
  for (int cnt=0;cnt<NumConnectionsEdit->Text.ToIntDef( 0 );cnt++)
    {
      void *myhandle;

      myhandle=mypayload->InitializePayload( PayloadMinEdit->Text.ToIntDef( 0 ),
                                             PayloadMaxEdit->Text.ToIntDef( 0 ),
                                             PayloadIncrementEdit->Text.ToIntDef( 0 ),
                                             PayloadTimeEdit->Text.ToIntDef( 0 ), NULL );
      if (!myhandle)
	{
      Application->MessageBox( "Unable to initialize payload ", Caption.c_str(), MB_OK );
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

  tmp=new struct client_protocol[NumConnectionsEdit->Text.ToIntDef( 0 )];
  if (!tmp)
    {
      Application->MessageBox( "Out of memory loading protocol", Caption.c_str(), MB_OK );
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
      Application->MessageBox( err_msg, Caption.c_str(), MB_OK );
      return( NULL );
    }
  myproto=new protocol( tmpc );
  delete []tmpc;
  if(!myproto)
    {
      Application->MessageBox( "Out of memory loading protocol library", Caption.c_str(), MB_OK );
      return( NULL );
    }
  if (!myproto->isOk())
    {
      delete myproto;
      Application->MessageBox( "Unable to load protocol library", Caption.c_str(), MB_OK );
      return( NULL );
    }
  my_proto_handle=myproto->InitProtocol( 0 ); // client
  if (!my_proto_handle)
    {
      delete myproto;
      Application->MessageBox( "Unable to initialize protocol", Caption.c_str(), MB_OK );
      return( NULL );
    }
  for (int cnt=0;cnt<NumConnectionsEdit->Text.ToIntDef( 0 );cnt++)
    {
      void *myconnection;

      myconnection=myproto->CreateConnection( my_proto_handle,
                                              ProtocolAddressEdit->Text.c_str(),
					                          ProtocolPortEdit->Text.ToIntDef( 0 ),
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
      Application->MessageBox( "Unable to create connection", Caption.c_str(), MB_OK );
      for (int cnt2=0;cnt2<cnt;cnt++)
        {
	  myproto->DestroyConnection( my_proto_handle, tmp[cnt2].connection, 0 );
        }
	  myproto->DestroyProtocol( my_proto_handle, 0 );
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
      Application->MessageBox( "Out of memory loading client", Caption.c_str(), MB_OK );
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
      Application->MessageBox( err_msg, Caption.c_str(), MB_OK );
      return( NULL );
    }
  myclient=new client( tmpc );
  delete []tmpc;
  if (!myclient)
    {
      Application->MessageBox( "Out of memory loading client instance", Caption.c_str(), MB_OK );
      return( NULL );
    }
  if (!myclient->isOk())
    {
      Application->MessageBox( "Unable to initialize client instance", Caption.c_str(), MB_OK );
      delete myclient;
      return( NULL );
    }
  tmp->clnt=myclient;
  tmp->handle=myclient->InitClient( proto, num_proto, pld, num_payload,
				    ClientMinEdit->Text.ToIntDef( 0 ),
                    ClientMaxEdit->Text.ToIntDef( 0 ),
                    ClientIncrementEdit->Text.ToIntDef( 0 ),
                    ClientTimeEdit->Text.ToIntDef( 0 ) );
  if (!tmp->handle)
    {
      Application->MessageBox( "Unable to initialize client", Caption.c_str(), MB_OK );
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
      for (int cnt=0;cnt<NumConnectionsEdit->Text.ToIntDef( 0 );cnt++)
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
      for (int cnt=0;cnt<ClientMaxEdit->Text.ToIntDef( 0 );cnt++)
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



void __fastcall TTrafficClientForm::NumConnectionsEditChange(
      TObject *Sender)
{
 if (NumConnectionsEdit->Text.ToIntDef( 0 )<1)
    {
      Application->MessageBox( "This must be a number larger than 0", Caption.c_str(), MB_OK );
      return;
    }
 ClientMaxEdit->Text=NumConnectionsEdit->Text;
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::ActivityTimerTimer(TObject *Sender)
{
 if (cc && cc->clnt && cc->handle)
   {
    LONGLONG rd, wt;
    cc->clnt->GetClientDataTransfered( cc->handle, &rd, &wt );
    DataReadLabel->Caption=rd;
    DataWrittenLabel->Caption=wt;
   }
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


void __fastcall TTrafficClientForm::PayloadHelpBitBtnClick(TObject *Sender)
{
 if (PayloadComboBox->Items->Strings[PayloadComboBox->ItemIndex]=="fixed")
   {
    Application->MessageBox( "This payload will return a fixed size packet to be sent to the server.\n\n"
                             "Payloads normally accept minimum, maximum, increment and time as parameters.\n\n"
                             "The fixed payload ignores all of these parameters except maximum to "
                             "determine the size of the payload the send. This maximum should be at least 8."
                             , (Caption + " Fixed Payload Help").c_str(), MB_OK );
   }
 if (PayloadComboBox->Items->Strings[PayloadComboBox->ItemIndex]=="increasing")
   {
    Application->MessageBox( "This payload will return an increasing sized packet to be sent to the server.\n\n"
                             "The packet will increase by a specified increment starting from a specified "
                             "minimum to a specified maximum. Once it reaches that maximum it will keep on "
                             "sending that sized payload. Each size packet, except the maximum sized "
                             "packet will be sent a specified number of times.\n\n"
                             "Payloads normally accept minimum, maximum, increment and time as parameters.\n\n"
                             "The payload starts with the minimum sized packet. This must be at least 8 "
                             "bytes in size and must be less than the maximum supplied. It will send this "
                             "packet size [time] times before it will add the increment to it. If adding "
                             "the increment to it makes it bigger than the maximum specified size, the "
                             "maximum sized packet will be sent."
                            , (Caption + " Increasing Payload Help").c_str(), MB_OK );
   }
 if (PayloadComboBox->Items->Strings[PayloadComboBox->ItemIndex]=="random")
   {
    Application->MessageBox( "This payload will return a random size packet to be sent to the server.\n\n"
                             "Payloads normally accept minimum, maximum, increment and time as parameters.\n\n"
                             "The random payload will select a random size between the minimum and maximum "
                             "specified and will return this sized payload [time] times. The increment "
                             "parameter is ignored."
                             , (Caption + " Random Payload Help").c_str(), MB_OK );
   }
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::ClientComboBoxChange(TObject *Sender)
{
 ClientMinEdit->Enabled=False;
 ClientIncrementEdit->Enabled=False;
 ClientTimeEdit->Enabled=False;
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::ClientHelpBitBtnClick(TObject *Sender)
{
  Application->MessageBox( "This client uses the payload and protocols passed to it to create a fixed "
                           "number of connections and send a steady stream of payloads down it. It will "
                           "read the reply and discard it.\n\n"
                           "Clients take minimum, maximum, increment and time parameters.\n\n"
                           "The fixed client ignores all the parameters except the maximum parameter. "
                           "This is the number of instances it will create to send data to the server "
                           , (Caption + " Fixed Client Help").c_str(), MB_OK );
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::ClientMaxEditChange(TObject *Sender)
{
 if (ClientMaxEdit->Text.ToIntDef( 0 )<1)
    {
      Application->MessageBox( "This must be a number larger than 0", Caption.c_str(), MB_OK );
      return;
    }
 NumConnectionsEdit->Text=ClientMaxEdit->Text;
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
                           , (Caption + " Protocol Help").c_str(), MB_OK );
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::ConnectionsHelpBitBtnClick(
      TObject *Sender)
{
  Application->MessageBox(  "This is the number of simultatious connections that should\n"
                            "be made to the server using the specified protocol and payload.\n"
                            "This value is synchronized with the Client Maximum value"
                           , (Caption + " Connections Help").c_str(), MB_OK );
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::QuitBitBtnClick(TObject *Sender)
{
  Close();    
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

void __fastcall TTrafficClientForm::StartBitBtnClick(TObject *Sender)
{
  SettingsPanel->Enabled=False;
  StopBitBtn->Enabled=True;
  StartBitBtn->Enabled=False;
  if (ProtocolComboBox->ItemIndex==-1)
    {
     Application->MessageBox( "Please select protocol to use", Caption.c_str(), MB_OK );
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
     Application->MessageBox( "Please select client type to use", Caption.c_str(), MB_OK );
     return;
    }
  if (PayloadComboBox->ItemIndex==-1)
    {
     SettingsPanel->Enabled=True;
     StopBitBtn->Enabled=False;
     StartBitBtn->Enabled=True;
     Application->MessageBox( "Please select payload type to use", Caption.c_str(), MB_OK );
     return;
    }
  if (NumConnectionsEdit->Text.ToIntDef( 0 )<1)
    {
     SettingsPanel->Enabled=True;
     StopBitBtn->Enabled=False;
     StartBitBtn->Enabled=True;
     Application->MessageBox( "More than one connection should be specified", Caption.c_str(), MB_OK );
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
  cc=allocate_clients( cd, NumConnectionsEdit->Text.ToIntDef( 0 ), cp, NumConnectionsEdit->Text.ToIntDef( 0 ) );
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
  // TODO
  // D 1. Test whether protocol, client and payload set
  // D 2. Load payload
  // 2.1. InitNetwork
  // D 3. Load protocol
  // D 4. Load client
  // D 5. Start Client
  // 6. Start timer
}
//---------------------------------------------------------------------------

void __fastcall TTrafficClientForm::AboutBitBtnClick(TObject *Sender)
{
 AboutBox->ProductName->Caption="Traffic Client 0.1.3";
 AboutBox->Visible=true;
 TrafficClientForm->Enabled=false;
}
//---------------------------------------------------------------------------

