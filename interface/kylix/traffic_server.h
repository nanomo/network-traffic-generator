//---------------------------------------------------------------------------

#ifndef traffic_serverH
#define traffic_serverH
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
#include <map>
#include "../../config.h"
#include "../../os/os.h"
#include "../../server/loadserver.h"
#include "../../reply/loadreply.h"
#include "../../protocols/loadprotocol.h"
//---------------------------------------------------------------------------
struct server_server
{
  server *serv;
  void *handle;
};
//---------------------------------------------------------------------------

class TTrafficServerForm : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TPanel *SettingsPanel;
        TLabel *Label1;
        TComboBox *ServerComboBox;
        TLabel *Label2;
        TComboBox *ReplyComboBox;
        TListBox *KnownProtocolsListBox;
        TListBox *ProtocolsListBox;
        TBitBtn *AddBitBtn;
        TBitBtn *RemoveBitBtn;
        TBitBtn *ClearBitBtn;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TSpinEdit *PortEdit;
        TPanel *Panel2;
        TBitBtn *StartButton;
        TBitBtn *StopButton;
        TBitBtn *AboutButton;
        TBitBtn *QuitBitBtn;
        TPanel *Panel3;
        TLabel *BytesReadLabel1;
        TLabel *BytesWrittenLabel1;
        TLabel *BytesReadLabel;
        TLabel *BytesWrittenLabel;
        TTimer *ActivityTimer;
        void __fastcall QuitBitBtnClick(TObject *Sender);
        void __fastcall AddBitBtnClick(TObject *Sender);
        void __fastcall RemoveBitBtnClick(TObject *Sender);
        void __fastcall ClearBitBtnClick(TObject *Sender);
        void __fastcall StartButtonClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall StopButtonClick(TObject *Sender);
        void __fastcall ActivityTimerTimer(TObject *Sender);
        void __fastcall AboutButtonClick(TObject *Sender);
private:	// User declarations
    struct server_server *ss;
    struct server_reply *sr;
    struct client_protocol *sp;

    struct server_reply *allocate_reply();
    struct client_protocol *allocate_protocol();
    struct server_server *allocate_servers( struct server_reply
*sr, struct client_protocol *sp );
    void cleanup( struct server_server *ss,
    struct server_reply *sr, struct client_protocol *sp );
public:		// User declarations
        __fastcall TTrafficServerForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTrafficServerForm *TrafficServerForm;
//---------------------------------------------------------------------------
#endif
