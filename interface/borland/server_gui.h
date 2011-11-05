//---------------------------------------------------------------------------

#ifndef server_guiH
#define server_guiH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
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
class TServerGuiForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *SettingsPanel;
    TComboBox *ServerComboBox;
    TLabel *Label1;
    TComboBox *ReplyComboBox;
    TLabel *Label2;
    TListBox *KnownProtocolsListBox;
    TListBox *ProtocolsListBox;
    TButton *AddButton;
    TButton *RemoveButton;
    TButton *ClearButton;
    TLabel *Label3;
    TLabel *Label4;
    TUpDown *PortUpDown;
    TEdit *PortEdit;
    TLabel *Label5;
    TPanel *Panel2;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *BytesReadLabel;
    TLabel *BytesWrittenLabel;
    TTimer *ActivityTimer;
    TBitBtn *AboutBitBtn;
    TBitBtn *StartButton;
    TBitBtn *StopButton;
    TBitBtn *QuitButton;
    void __fastcall PortUpDownClick(TObject *Sender, TUDBtnType Button);
    void __fastcall AddButtonClick(TObject *Sender);
    void __fastcall ClearButtonClick(TObject *Sender);
    void __fastcall RemoveButtonClick(TObject *Sender);
    void __fastcall StartButtonClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ActivityTimerTimer(TObject *Sender);
    void __fastcall AboutBitBtnClick(TObject *Sender);
    void __fastcall StopButtonClick(TObject *Sender);
    void __fastcall QuitButtonClick(TObject *Sender);
private:	// User declarations
    struct server_server *ss;
    struct server_reply *sr;
    struct client_protocol *sp;

    struct server_reply *TServerGuiForm::allocate_reply();
    struct client_protocol *TServerGuiForm::allocate_protocol();
    struct server_server *TServerGuiForm::allocate_servers( struct server_reply *sr, struct client_protocol *sp );
    void TServerGuiForm::cleanup( struct server_server *ss,
	      struct server_reply *sr, struct client_protocol *sp );
public:		// User declarations
    __fastcall TServerGuiForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TServerGuiForm *ServerGuiForm;
//---------------------------------------------------------------------------
#endif
