//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TImage *Image1;
        TStaticText *NodeText;
        TLabel *NodeLabel;
    TStaticText *EdgeText;
    TLabel *EdgeLabel;
    TLabel *SubLabel;
    TStaticText *SubText;
    TStatusBar *StatusBar1;
    TMainMenu *MainMenu1;
    TMenuItem *Graph1;
    TMenuItem *New1;
    TMenuItem *Refresh1;
    TMenuItem *N1;
    TMenuItem *Exit1;
    TMenuItem *Help1;
    TMenuItem *About1;
    TMenuItem *N2;
    TLabel *IsolateLabel;
    TStaticText *IsolateText;
        TMenuItem *Grammer1;
        TMenuItem *InternalSymbolsSet1;
        TMenuItem *Save1;
        TSaveDialog *SaveDialog1;
        TMenuItem *Open1;
        TOpenDialog *OpenDialog1;
        TMenuItem *Node1;
        TMenuItem *KillIsolated1;
        TMenuItem *ShowByID1;
        TMenuItem *N3;
        TMenuItem *ENullRemoval1;
        TMenuItem *ENull0EdgeRemoval1;
        TMenuItem *NonDeterminestiveRemoval1;
        TPopupMenu *PageMenu;
        TMenuItem *Insert1;
        TMenuItem *Rename1;
        TMenuItem *CreateEdge1;
        TMenuItem *Delete1;
        TMenuItem *N4;
        TMenuItem *StartNode1;
        TMenuItem *FinalState1;
        TMenuItem *NFADFA1;
        TMenuItem *NonDeterminestiveRemovalOnePhase1;
        TMenuItem *NullWayRemoval1;
        TMenuItem *N5;
        TMenuItem *Delete2;
        TMenuItem *EditEdge1;
        TMenuItem *CurveEdge1;
    
    
    void __fastcall FormCreate(TObject *Sender);
    
    void __fastcall ShowInfo();
    void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    
    void __fastcall New1Click(TObject *Sender);
    void __fastcall Refresh1Click(TObject *Sender);
    
    void __fastcall About1Click(TObject *Sender);
    void __fastcall Exit1Click(TObject *Sender);
        void __fastcall InternalSymbolsSet1Click(TObject *Sender);
        void __fastcall Image1DblClick(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall ShowByID1Click(TObject *Sender);
        void __fastcall KillIsolated1Click(TObject *Sender);
        void __fastcall ENull0EdgeRemoval1Click(TObject *Sender);
        void __fastcall Insert1Click(TObject *Sender);
        void __fastcall PageMenuPopup(TObject *Sender);
        void __fastcall Delete1Click(TObject *Sender);
        void __fastcall Rename1Click(TObject *Sender);
        void __fastcall CreateEdge1Click(TObject *Sender);
        void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall ENullRemoval1Click(TObject *Sender);
        void __fastcall NonDeterminestiveRemoval1Click(TObject *Sender);
        void __fastcall StartNode1Click(TObject *Sender);
        void __fastcall FinalState1Click(TObject *Sender);
        void __fastcall NFADFA1Click(TObject *Sender);
        void __fastcall NonDeterminestiveRemovalOnePhase1Click(
          TObject *Sender);
        void __fastcall NullWayRemoval1Click(TObject *Sender);
        void __fastcall EditEdge1Click(TObject *Sender);
        void __fastcall Delete2Click(TObject *Sender);
        void __fastcall CurveEdge1Click(TObject *Sender);
        void __fastcall UpdateEdge(char * c);
private:	// User declarations
public:		// User declarations
    __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
