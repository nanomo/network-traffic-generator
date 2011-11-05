//----------------------------------------------------------------------------
#ifndef aboutH
#define aboutH
//----------------------------------------------------------------------------
#include <System.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <QGraphics.hpp>
#include <QForms.hpp>
#include <QControls.hpp>
#include <QStdCtrls.hpp>
#include <QButtons.hpp>
#include <QExtCtrls.hpp>
#include <QComCtrls.hpp>
//----------------------------------------------------------------------------
class TAboutBox : public TForm
{
__published:
	TPanel *AboutPanel;
	TLabel *ProductName;
	TLabel *Copyright;
	TButton *OKButton;
        TTextViewer *TextViewer;
private:
public:
	virtual __fastcall TAboutBox(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TAboutBox *AboutBox;
//----------------------------------------------------------------------------
#endif
