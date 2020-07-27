//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRules *Rules;

//---------------------------------------------------------------------------
__fastcall TRules::TRules(TComponent* Owner)
        : TForm(Owner)
{
        StringGrid1->Cells[0][0]="Rule";
        StringGrid1->Cells[1][0]="Description";
        frule=new Rule("E","Null Arc");
        RefreshGrid();
}
//---------------------------------------------------------------------------

void TRules::RefreshGrid(){
        int i=frule->count(1);
        Rule * r=frule;
        StringGrid1->RowCount=i+1;
        for(int j=1;j<i+1;j++){
                StringGrid1->Cells[0][j]=r->name;
                StringGrid1->Cells[1][j]=r->des;
                r=r->next;
        }

}


void __fastcall TRules::StringGrid1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{

        if(Key==27)ModalResult=mrCancel;
        
		if(Key==46 && StringGrid1->Row>1){
			if(Application->MessageBox("Are You Sure ?\nAll edges with current ruel will be removed.)","Confirm",MB_YESNO)==IDYES);{
				Rule * r=frule,*rb;
                for(int j=0;j<StringGrid1->Row-2;j++)r=r->next;
                rb=r->next;
                r->next=rb->next;
                MainForm->UpdateEdge(rb->name);
                delete rb;
            }
            RefreshGrid();
        }

        if((StringGrid1->Row==StringGrid1->RowCount-1)&& (StringGrid1->Cells[0][StringGrid1->RowCount-1]!="") && (Key==40)){
                RuleEdit->Text="";
                DesEdit->Text="";
                GroupBox1->Visible=true;
                RuleEdit->SetFocus();
                StringGrid1->Enabled=false;
                Button1->Enabled=false;
        }

}
//---------------------------------------------------------------------------

void __fastcall TRules::Button1Click(TObject *Sender)
{
        CurRule=frule;
        for(int j=0;j<StringGrid1->Row-1;j++)CurRule=CurRule->next;
        ModalResult=mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TRules::AddButtonClick(TObject *Sender)
{
        if(RuleEdit->Text==""){
             Application->MessageBox("Rule Name Not Correctly Set","Error",MB_OK);
             return;
        }
        if(frule->find(RuleEdit->Text.c_str())!=NULL ){
                            Application->MessageBox("Duplicated Rules Not Allowded in Set.","Error",MB_OK);
                            return;
        }

        frule->add(RuleEdit->Text.c_str(),DesEdit->Text.c_str());
        RefreshGrid();
        RuleEditKeyPress(NULL,27);
}
//---------------------------------------------------------------------------

void __fastcall TRules::RuleEditKeyPress(TObject *Sender, char &Key)
{
        if(Key==27){
                GroupBox1->Visible=false;
                StringGrid1->Enabled=true;
                Button1->Enabled=true;
                StringGrid1->SetFocus();
        }
}
//---------------------------------------------------------------------------

void __fastcall TRules::Button1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        if(Key==27)ModalResult=mrCancel;
}
//---------------------------------------------------------------------------

