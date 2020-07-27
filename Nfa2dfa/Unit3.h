//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class Rule{
     public:
        Rule * next;
        char name[11];
        char des[21];
        Rule(char * n,char * d){
                StrLCopy(name,n,10);
                StrLCopy(des,d,20);
                name[10]=0;
                des[20]=0;
                next=NULL;
        }
        int count(int n){
                if(next==NULL)return n;
                return next->count(n+1);
        }
        void add(char * n,char * d){
                if(next!=NULL) next->add(n,d);
                else{
                        next=new Rule(n,d);
                }
        }
        Rule * find(char * n){
                if(!StrLComp(name,n,10))return this;
                if(next!=NULL)return next->find(n);
                return NULL;
        }
        ~Rule(){
              if(next!=NULL)delete next;  
        }
};


class TRules : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TStringGrid *StringGrid1;
        TGroupBox *GroupBox1;
        TEdit *RuleEdit;
        TEdit *DesEdit;
        TLabel *Label1;
        TLabel *Label2;
        TButton *AddButton;
        void __fastcall StringGrid1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall AddButtonClick(TObject *Sender);
        void __fastcall RuleEditKeyPress(TObject *Sender, char &Key);
        void __fastcall Button1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
        __fastcall TRules(TComponent* Owner);
        void RefreshGrid();
        class Rule *CurRule,*frule;
};
//---------------------------------------------------------------------------
extern PACKAGE TRules *Rules;
extern PACKAGE Rule *CurRule,*frule;
//---------------------------------------------------------------------------
#endif
