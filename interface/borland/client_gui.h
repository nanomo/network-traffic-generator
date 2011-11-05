//---------------------------------------------------------------------------

#ifndef client_guiH
#define client_guiH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "../../os/os.h"
#include "../../client/loadclient.h"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
struct client_client
{
  client *clnt;
  void *handle;
};
//---------------------------------------------------------------------------

class TTrafficClientForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *SettingsPanel;
    TUpDown *ConnectionsUpDown;
    TLabel *Label5;
    TEdit *NumConnectionsEdit;
    TPanel *Panel3;
    TPanel *Panel4;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *DataReadLabel;
    TLabel *DataWrittenLabel;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TComboBox *PayloadComboBox;
    TEdit *PayloadTimeEdit;
    TEdit *PayloadIncrementEdit;
    TEdit *PayloadMinEdit;
    TEdit *PayloadMaxEdit;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TGroupBox *GroupBox2;
    TLabel *Label2;
    TComboBox *ClientComboBox;
    TLabel *Label12;
    TEdit *ClientMinEdit;
    TLabel *Label13;
    TEdit *ClientMaxEdit;
    TLabel *Label14;
    TLabel *Label15;
    TEdit *ClientIncrementEdit;
    TEdit *ClientTimeEdit;
    TGroupBox *GroupBox3;
    TLabel *Label3;
    TComboBox *ProtocolComboBox;
    TLabel *Label4;
    TEdit *ProtocolPortEdit;
    TUpDown *PortUpDown;
    TLabel *Label16;
    TEdit *ProtocolAddressEdit;
    TTimer *ActivityTimer;
    TBitBtn *PayloadHelpBitBtn;
    TBitBtn *ClientHelpBitBtn;
    TBitBtn *ProtocolHelpBitBtn;
    TBitBtn *ConnectionsHelpBitBtn;
    TBitBtn *StopBitBtn;
    TBitBtn *QuitBitBtn;
    TBitBtn *StartBitBtn;
    TBitBtn *AboutBitBtn;
    void __fastcall QuitButtonClick(TObject *Sender);
    void __fastcall PortUpDownClick(TObject *Sender, TUDBtnType Button);
    void __fastcall StartButtonClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall NumConnectionsEditChange(TObject *Sender);
    void __fastcall ActivityTimerTimer(TObject *Sender);
    void __fastcall PayloadComboBoxChange(TObject *Sender);
    void __fastcall PayloadHelpBitBtnClick(TObject *Sender);
    void __fastcall ClientComboBoxChange(TObject *Sender);
    void __fastcall ClientHelpBitBtnClick(TObject *Sender);
    void __fastcall ClientMaxEditChange(TObject *Sender);
    void __fastcall ProtocolHelpBitBtnClick(TObject *Sender);
    void __fastcall ConnectionsHelpBitBtnClick(TObject *Sender);
    void __fastcall QuitBitBtnClick(TObject *Sender);
    void __fastcall StopBitBtnClick(TObject *Sender);
    void __fastcall StartBitBtnClick(TObject *Sender);
    void __fastcall AboutBitBtnClick(TObject *Sender);
private:	// User declarations
    struct client_client *cc;
    struct client_protocol *cd;
    struct client_payload *cp;

    void cleanup( struct client_payload *payload, struct client_protocol *protocol, struct client_client *client );
    struct client_client *allocate_clients( struct client_protocol *proto, int num_proto, struct client_payload *pld, int num_payload );
    struct client_protocol *allocate_protocol();
    struct client_payload *allocate_payload();

public:		// User declarations
    __fastcall TTrafficClientForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTrafficClientForm *TrafficClientForm;
//---------------------------------------------------------------------------
#endif
