//---------------------------------------------------------------------------

#ifndef traffic_clientH
#define traffic_clientH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <QControls.hpp>
#include <QStdCtrls.hpp>
#include <QForms.hpp>
#include <QExtCtrls.hpp>
#include <QButtons.hpp>
#include <QComCtrls.hpp>
#include <QTypes.hpp>
//---------------------------------------------------------------------------
#include "../../os/os.h"
#include "../../client/loadclient.h"
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
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TComboBox *PayloadComboBox;
        TEdit *PayloadMinEdit;
        TEdit *PayloadMaxEdit;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *PayloadTimeEdit;
        TEdit *PayloadIncrementEdit;
        TBitBtn *PayloadHelpBitBtn;
        TGroupBox *GroupBox2;
        TLabel *Label6;
        TComboBox *ClientComboBox;
        TLabel *Label7;
        TEdit *ClientMinEdit;
        TLabel *Label8;
        TEdit *ClientMaxEdit;
        TLabel *Label9;
        TEdit *ClientTimeEdit;
        TLabel *Label10;
        TEdit *ClientIncrementEdit;
        TBitBtn *ClientHelpBitBtn;
        TGroupBox *GroupBox3;
        TLabel *Label11;
        TLabel *Label12;
        TComboBox *ProtocolComboBox;
        TLabel *Label13;
        TSpinEdit *ProtocolPortEdit;
        TEdit *ProtocolAddressEdit;
        TBitBtn *ProtocolHelpBitBtn;
        TGroupBox *GroupBox4;
        TLabel *Label14;
        TSpinEdit *NumConnectionsEdit;
        TBitBtn *BitBtn1;
        TPanel *Panel2;
        TPanel *Panel3;
        TBitBtn *StartBitBtn;
        TBitBtn *StopBitBtn;
        TBitBtn *AboutButton;
        TBitBtn *QuitButton;
        TLabel *Label15;
        TLabel *DataWrittenLabel1;
        TLabel *DataReadLabel;
        TLabel *DataWrittenLabel;
        TTimer *ActivityTimer;
        void __fastcall QuitButtonClick(TObject *Sender);
        void __fastcall AboutButtonClick(TObject *Sender);
        void __fastcall PayloadComboBoxChange(TObject *Sender);
        void __fastcall ProtocolComboBoxChange(TObject *Sender);
        void __fastcall ClientMaxEditChange(TObject *Sender);
        void __fastcall NumConnectionsEditChanged(TObject *Sender,
          int NewValue);
        void __fastcall ClientComboBoxChange(TObject *Sender);
        void __fastcall StartBitBtnClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ActivityTimerTimer(TObject *Sender);
        void __fastcall PayloadHelpBitBtnClick(TObject *Sender);
        void __fastcall PayloadComboBoxClick(TObject *Sender);
        void __fastcall ClientComboBoxClick(TObject *Sender);
        void __fastcall ClientHelpBitBtnClick(TObject *Sender);
        void __fastcall StopBitBtnClick(TObject *Sender);
        void __fastcall ProtocolHelpBitBtnClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
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
