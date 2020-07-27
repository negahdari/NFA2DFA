/******************************************************************************
*********************************************************
********  Programmer   :   Ramtin Negahdari    **********
********          In   :   DEC 2000            **********
*********************************************************
*********************************************************
Note:
    Just a method of Graph Class Named GreedySolve commented.
    (I Don't have much time to comment all lines.try it yourself)

Note:
    This program is not fully functioning
    Alse deleting a Node or an edge is supported(Methods are already written)
*/
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TMainForm *MainForm;


#define Page MainForm->Image1->Canvas
#define WForm GraphMatrix
void FillGrid();

/*****************************************************************************/
/* Node Class **************************************************************/
/*****************************************************************************/
class Node{
    public:
        int       X,Y;
        int       ID;
        bool      Final;
        char      Name[3];
        Node      * Next;
        TColor    cl;
        int       Connection;
        Node(int x,int y,int id){
            X=x;
            Y=y;
            Final=false;
            cl=clBlack;
            ID=id;
            itoa(ID,Name,10);
            Next=NULL;
            Connection=0;
        }
        void Visible(TCanvas *_Page,TColor icl,bool Id=false,bool SCase=false);
        int NodeID(int id);
};

void Node::Visible(TCanvas *_Page,TColor icl,bool Id,bool SCase){
    cl=icl;
    _Page->Pen->Color=icl;
    if(Final)_Page->Ellipse(X-13,Y-13,X+13,Y+13);
    _Page->Ellipse(X-10,Y-10,X+10,Y+10);
    if(SCase)_Page->Rectangle(X-10,Y-10,X+10,Y+10);
    _Page->Font->Color=icl;
    if(Id)
            _Page->TextOut(X-6,Y-7,ID);
    else
            _Page->TextOut(X-6,Y-7,Name);

}

int Node::NodeID(int id){
        do{
          if(id==ID)id++;
          if(Next!=NULL)id=Next->NodeID(id);
        }while(id==ID);
        return id;
}

class NodeList{
    public:
      NodeList * Next;
      NodeList * Child;
      Node * NodeP;
      NodeList * Last(){
        if(Next==NULL)return this;
        else return Next->Last();
      }
      int count(int n){
        if (Next==NULL)return n;
        return Next->count(n+1);
      }
      NodeList(Node * n){
        NodeP=n;
        Next=NULL;
        Child=NULL;
      }
      ~NodeList(){
        if(Child!=NULL)delete Child;
        if(Next!=NULL)delete Next;
      }
      NodeList*  AddChild(Node * CNode){
         NodeList * NL=new NodeList(CNode);
         if(Child==NULL)Child=NL;
         else (Child->Last())->Next=NL;
         return NL;
      }

      bool IsChildSelf(Node * node=NULL){
           if(node==NULL)node=NodeP;
           else if(node==NodeP)return true;
           bool result=false;
           for(NodeList * nl=Child;nl!=NULL;nl=nl->Next)
             if(nl->IsChildSelf(node))result=true;
           return result;
      }

};


/*****************************************************************************/
/* Edge Class ****************************************************************/
/*****************************************************************************/
class Edge{
    public:
        class Node * Startv;
        class Node * Terminatev;
        char      Name[11];
        char      Des[21];
        Edge      * Next;
        TColor    cl;
        float     Mplex;
        Edge(class Node *StV,
             class Node *TrV,
             char * n,char * d)
        {
            Startv=StV;
            Terminatev=TrV;
            cl=clDkGray;
            Next=NULL;
            StrLCopy(Name,n,10);
            StrLCopy(Des,d,20);
            Name[10]=0;
            Des[20]=0;
            Mplex=1.0;
        }
        void Visible(TCanvas *_Page,TColor icl=clLtGray);
};

void Edge::Visible(TCanvas *_Page,TColor icl){
    _Page->Pen->Color=icl;
    _Page->Font->Color=icl;
    if(Startv==Terminatev){
        _Page->Ellipse(Startv->X-2,Startv->Y-2,Startv->X+20,Startv->Y+20);
        _Page->TextOut(Startv->X+10,Startv->Y+10,Name);

    }else{

        int H,W,XL,XH,YL,YH;
        float arc;
        XL=(Startv->X)<(Terminatev->X)?(Startv->X):(Terminatev->X);
        XH=(Startv->X)>(Terminatev->X)?(Startv->X):(Terminatev->X);
        YL=(Startv->Y)<(Terminatev->Y)?(Startv->Y):(Terminatev->Y);
        YH=(Startv->Y)>(Terminatev->Y)?(Startv->Y):(Terminatev->Y);

        W=XH-XL;
        H=YH-YL;

        Windows::TPoint p[4];
        p[0]=Point(Startv->X,Startv->Y);
        p[1]=Point(XL+(W*Mplex)/2,YL+(H*Mplex)/2);
        p[2]=p[1];
        p[3]=Point(Terminatev->X,Terminatev->Y);

        _Page->PolyBezier (p,3);
        _Page->TextOut(p[1].x-5 ,p[1].y-5,Name);

        if((Terminatev->Y-p[1].y) || (Terminatev->X-p[1].x))
        arc=atan2((float)(Terminatev->Y-p[1].y),(float)(Terminatev->X-p[1].x));

        p[1]=Point(Terminatev->X - 10*cos(arc),Terminatev->Y-10*sin(arc));
        p[3]=Point(Terminatev->X - 14*cos(arc),Terminatev->Y-14*sin(arc));
        p[0]=Point(Terminatev->X - 18*cos(arc-(M_PI_4/4)),Terminatev->Y-18*sin(arc-(M_PI_4/4)));
        p[2]=Point(Terminatev->X - 18*cos(arc+(M_PI_4/4)),Terminatev->Y-18*sin(arc+(M_PI_4/4)));
        _Page->Brush->Color=clGray;
        _Page->Polygon(p,3);
        _Page->Brush->Color=clLtGray;
    }
}

/*****************************************************************************/
/* Graph Class ***************************************************************/
/*****************************************************************************/
class Graph{
    public:
        bool      Directed;
        Node  *   PNode;
        Node  *   SNode;
        Edge  *   PEdge;
        Graph(bool directed=false){
            PNode=NULL;
            PEdge=NULL;
            SNode=NULL;
            Directed=directed;
        }
        void            empty();
        int             NodeNoInBus(const class Node *v);
        class Node      *NodePointerInBus(int no);
        int             EmptyNodeID();
        Node *          NodeAdd(int X,int Y);
        void            NodeAdd(class Node * N);
        bool            NodeRemove(int ID);
        bool            NodeRemove(Node * N);
        int             NodeCount();
        class Node      *LastNode();
        class Node      *NodePointedToCurrent(const class Node *v);
        class Node      *NodePointer(int ID);
        class Node      *NodeEnter(int X,int Y);

        class Edge      *LastEdge();
        void            EdgeAdd(class Edge *E);
        bool            EdgeAdd(class Node *Start,class Node *End,char * n,char * d);
        bool            EdgeRemove(Edge * E);
        void            EdgeAllRemove(char * n);
        class Edge      *EdgeFind(const class Node *Start,const class Node *End,char * n);
        class Edge      *EdgeFind(const class Node *Start,const class Node *End);
        class Edge      *EdgeFind(const class Node *Start,class Edge * StartE,char * n);
        class Edge      *EdgeFind(const class Node *Start,class Edge * StartE);
        int             EdgeCount();
        class Edge      *EdgePointedToCurrent(const class Edge *e);
        bool            IsNullWay();
        void            RemoveDuplicatedEdge();

        void            Show(TImage * _Page,bool ID=false);

        int             IsolateNodeCount();
        void            KillIsolateNode();
        bool            MarkGraphPice(int ID,class Node *v);
        int             SubGraphCount();

        NodeList        *NullEdge(Node * N);

        bool            IsNodeWay(Node * S,Node * D);
        NodeList        *NullEdgeTree(Node * N,Node * StopNode=NULL,NodeList * nl=NULL);
        NodeList        *RemoveNullWay(NodeList * Tree,Node * StopNode=NULL);
        void            MoveNode2Node(Node * Dest,Node * Source);
        void            CopyNode2Node(Node * Dest,Node * Source);
        NodeList        *FindNonDeterminestive();
        void            NodeTraverseRemoval();

        void            NullCycleRemoval();
        void            NullTransitionRemoval();
        void            NonDeterminestiveRemovalOnePhase(NodeList *NL);
        void            NonDeterminestiveRemoval();
};

void Graph::empty(){
    if(PNode!=NULL){
        if(PEdge!=NULL)
            for(class Edge *e=PEdge,*le;
                e!=NULL;
                le=e->Next,delete e,e=le);

        for(class Node *v=PNode,*lv;
            v!=NULL;
            lv=v->Next,delete v,v=lv);
    }
    SNode=PNode=NULL;
    PEdge=NULL;
}

int  Graph::NodeCount(){
    int Counter=0;

    for(class Node *v=PNode;
        v!=NULL;
        v=v->Next,Counter++);
    return Counter;
}

int Graph::NodeNoInBus(const class Node *v){
    int i=1;
    for(class Node *vp=PNode;vp!=NULL && vp!=v; vp=vp->Next,i++);
    return i;
}

class Node *Graph::NodePointerInBus(int no){
    class Node* v=PNode;
    for(int i=0;i<no;i++)
        if(v!=NULL)v=v->Next;
    return v;
}

int Graph::EmptyNodeID(){
    if(PNode==NULL)return 1;    
    return PNode->NodeID(1);    
}

class Node *Graph::NodePointer(int ID){
    if(ID==-1)return NULL;
    for(class Node *v= PNode;v!=NULL;v=v->Next)
        if(v->ID==ID)return v;
    return NULL;
}

class Node *Graph::NodePointedToCurrent(const class Node *v){
    if(v==NULL)return NULL;
    for(class Node *vp= PNode;vp!=NULL;vp=vp->Next)
        if(vp->Next==v)return vp;
    return NULL;
}

class Node *Graph::LastNode(){
    for(class Node *n=PNode;n!=NULL;n=n->Next)
        if(n->Next==NULL)return n;
    return NULL;
}

Node * Graph::NodeAdd(int X,int Y){
    int EmptyID=EmptyNodeID();
    class Node *v=new class Node(X,Y,EmptyID);

    if(PNode==NULL) PNode=v;
    else (LastNode())->Next=v;

    v->Visible(Page,clBlack);
    return v;
}

void Graph::NodeAdd(class Node * N){
    N->Next=NULL;
    if(PNode==NULL) PNode=N;
    else (LastNode())->Next=N;
    N->Visible(Page,clBlack);
}


bool Graph::NodeRemove(Node * N){
    if(N==NULL)return false;    //Error
    if(PNode==N)
       PNode=N->Next;
    else
       NodePointedToCurrent(N)->Next=N->Next;

    for(Edge * E=PEdge;E!=NULL;E=E->Next)
        if(E->Startv==N || E->Terminatev==N) EdgeRemove(E);

    if(SNode==N){
        SNode=NULL;
        N->Visible(Page,clLtGray,false,true);
    }
    N->Visible(Page,clLtGray);
    delete N;
    return true;
}

bool Graph::NodeRemove(int ID){
    return NodeRemove(NodePointer(ID));
}

class Node *Graph::NodeEnter(int X,int Y){
    for(class Node *v=PNode;v!=NULL;v=v->Next){
        if(abs(X-v->X)<10 && abs(Y-v->Y)<10){
            return v;
        }
    }
    return NULL;
}


class Edge *Graph::LastEdge(){
    for(class Edge *e=PEdge;e!=NULL;e=e->Next)
        if(e->Next==NULL)return e;
    return NULL;
}

void Graph::RemoveDuplicatedEdge(){
    if(PEdge==NULL)return;
    for(Edge * E=PEdge;E!=NULL;E=E->Next){
        class Edge * et=EdgeFind(E->Startv,E->Terminatev,E->Name);
        if(et!=NULL && et!=E)
        EdgeRemove(et);
    }
}


bool Graph::EdgeAdd(class Node *Start,class Node *End,char * n,char * d){
    class Edge *LastEdgep=LastEdge();
    class Edge *e=new class Edge(Start,End,n,d);

    if(EdgeFind(Start,End,n)!=NULL){
        Application->MessageBox("Duplicated Edge","Error",MB_OK);
        return false;
    }

    if(LastEdgep==NULL)PEdge=e;
    else LastEdgep->Next=e;

    e->Visible(Page,clDkGray);
    return true;
}


void Graph::EdgeAdd(class Edge * E){
    class Edge *LastEdgep=LastEdge();
    E->Next=NULL;
    if(LastEdgep==NULL)PEdge=E;
    else LastEdgep->Next=E;

    E->Visible(Page,clDkGray);
}

bool Graph::EdgeRemove(Edge * E){
    if(E==NULL)return false;    //Error
    if(PEdge==E)
       PEdge=E->Next;
    else
       EdgePointedToCurrent(E)->Next=E->Next;

    E->Visible(Page,clLtGray);
    delete E;
    return true;
}

void    Graph::EdgeAllRemove(char * n){
    if(n==NULL)return;    //Error
    for(class Edge *e=PEdge;e!=NULL;e=e->Next)
        if(!StrComp(e->Name,n))
           EdgeRemove(e);


}

class Edge *Graph::EdgeFind(const class Node *Start,const class Node *End,char * n){
    for(class Edge *e=PEdge;e!=NULL;e=e->Next)
        if((Start==e->Startv  && End==e->Terminatev)&&
           !StrComp(e->Name,n))
           return e;

    return NULL;
}

class Edge *Graph::EdgeFind(const class Node *Start,const class Node *End){
    for(class Edge *e=PEdge;e!=NULL;e=e->Next)
        if((Start==e->Startv  && End==e->Terminatev))
           return e;

    return NULL;
}

class Edge *Graph::EdgeFind(const class Node *Start,class Edge * StartE,char * n){
    for(class Edge *e=StartE;e!=NULL;e=e->Next)
        if(Start==e->Startv  && !StrComp(e->Name,n))
           return e;

    return NULL;
}

class Edge *Graph::EdgeFind(const class Node *Start,class Edge * StartE){
    for(class Edge *e=StartE;e!=NULL;e=e->Next)
        if(Start==e->Startv)
           return e;

    return NULL;
}

int  Graph::EdgeCount(){
    int Counter=0;

    for(class Edge *e=PEdge;
        e!=NULL;
        e=e->Next,Counter++);
    return Counter;
}

class Edge *Graph::EdgePointedToCurrent(const class Edge *e){
    if(e==NULL)return NULL;
    for(Edge *ep= PEdge;ep!=NULL;ep=ep->Next)
        if(ep->Next==e)return ep;
    return NULL;
}

bool  Graph::IsNullWay(){
    for(Edge *e=PEdge;e!=NULL;e=e->Next)if(!StrComp(e->Name,"E"))return true;
    return false;
}

void Graph::Show(TImage * _Page,bool ID){
    class Node *n;
    class Edge *e;
    _Page->Canvas->FillRect(Rect(0,
                         0,
                         _Page->Width,
                         _Page->Height));
    for(e=PEdge;e!=NULL;e=e->Next)
        e->Visible(Page,e->cl);
    for(n=PNode;n!=NULL;n=n->Next){
      n->Visible(Page,n->cl,ID);
      if(SNode==n)n->Visible(Page,n->cl,ID,true);
    }
}


int Graph::IsolateNodeCount(){
    int Count=0;
    for(class Node *v=PNode;v!=NULL;v=v->Next)
        v->Connection=0;

    for(class Edge *e=PEdge;e!=NULL;e=e->Next)
        if(e->Startv!=e->Terminatev){
            (e->Startv)->Connection=1;
            (e->Terminatev)->Connection=1;
        }

    for(class Node *v=PNode;v!=NULL;v=v->Next)
        if(!v->Connection)Count++;

    return Count;
}

void  Graph::KillIsolateNode(){
    IsolateNodeCount();
    for(class Node *v=PNode;v!=NULL;v=v->Next)
        if(!v->Connection) NodeRemove(v->ID);
}

bool Graph::MarkGraphPice(int ID,class Node *v){
    if(v->Connection)return false;
    v->Connection=ID;
    bool SW=false;
    for(class Edge *e=PEdge;e!=NULL;e=e->Next)
        if( ((e->Startv)==v || (e->Terminatev)==v) && (e->Terminatev!=e->Startv ) ){
            if((e->Startv)==v)MarkGraphPice(ID,e->Terminatev);
            else MarkGraphPice(ID,e->Startv);
            SW=true;
        }
    if(SW){
        v->Connection=ID;
        return true;
    }else{
        v->Connection=0;
        return false;
    }
}

int Graph::SubGraphCount(){
    int Count=IsolateNodeCount();
    class Node *v=PNode;
    for(;v!=NULL;v=v->Next)
        v->Connection=0;

    for(v=PNode;v!=NULL;v=v->Next)
        if(MarkGraphPice(v->ID,v))Count++;


    return Count;
}


NodeList *Graph::NullEdge(Node * N){
        if(N==NULL)return NULL;
        NodeList * Result=new NodeList(N);
        for(Edge * e=PEdge;e!=NULL;e=e->Next){
                if( (e->Startv==N)  && !StrComp(e->Name,"E")){
                    Result->AddChild(e->Terminatev);
                }
        }
        return Result;
}


NodeList *Graph::NullEdgeTree(Node * N,Node * StopNode,NodeList * nl){
        if(StopNode==NULL)StopNode=N;
        if(N==NULL)return NULL;
        NodeList * Result;
        if(nl==NULL)Result=new NodeList(N);
        else Result = nl;
        for(Edge * e=PEdge;e!=NULL;e=e->Next){
                if( (e->Startv==N)  && !StrComp(e->Name,"E")){
                    NodeList * NextNode= Result->AddChild(e->Terminatev);
                    if(NextNode->NodeP!=StopNode) NullEdgeTree(NextNode->NodeP,StopNode,NextNode);
                }
        }
        return Result;
}

NodeList *Graph::RemoveNullWay(NodeList * Tree,Node * StopNode){
        if(Tree==NULL)return Tree;
        if(StopNode==NULL){
                if(!(Tree->IsChildSelf())){
                        delete Tree;
                        return NULL;
                }
                StopNode=Tree->NodeP;
        }
        NodeList * lnl=NULL;
        for(NodeList * nl=Tree->Child;nl!=NULL;nl=nl->Next){
                if(nl->IsChildSelf(StopNode)){
                     RemoveNullWay(nl,StopNode);
                }
                else{
                     if(lnl==NULL)Tree->Child=nl->Next;
                     else lnl->Next=nl->Next;
                     delete nl;
                     nl=Tree->Child;
                     lnl=NULL;
                }
                lnl=nl;
        }


        return Tree;
}

void Graph::MoveNode2Node(Node * Dest,Node * Source){
        if(Dest==NULL || Source==NULL)return;
        if(SNode==Source)SNode=Dest;
        if(Source->Final)Dest->Final=Source->Final;
        for(Edge *e=PEdge;e!=NULL;e=e->Next){
                if(e->Startv==Source)e->Startv=Dest;
                if(e->Terminatev==Source)e->Terminatev=Dest;
        }
        for(Edge *e=PEdge;e!=NULL;e=e->Next)
                if(e->Startv==e->Terminatev && !StrComp(e->Name,"E"))EdgeRemove(e);
        RemoveDuplicatedEdge();
}

void Graph::CopyNode2Node(Node * Dest,Node * Source){
        if(Dest==NULL || Source==NULL)return;
//        if(SNode==Source)SNode=Dest;
        if(Source->Final)Dest->Final=Source->Final;
        for(Edge *e=PEdge;e!=NULL;e=e->Next)
                if(e->Startv==Source)
                        EdgeAdd(Dest,e->Terminatev,e->Name,e->Des);
        for(Edge *e=PEdge;e!=NULL;e=e->Next)
                if(e->Startv==e->Terminatev && !StrComp(e->Name,"E"))EdgeRemove(e);
        RemoveDuplicatedEdge();
}

void Graph::NullCycleRemoval(){
      if(PNode==NULL)return;
          Node * N=PNode;
          NodeList * NL;
          while(N!=NULL){
             NL=NullEdgeTree(N);
             NL=RemoveNullWay(NL);
             if(NL!=NULL){
                    for(NodeList * nl=NL->Child;nl!=NULL;nl=nl->Next)
                       MoveNode2Node(NL->NodeP,nl->NodeP);
                    delete NL;
             }else
                    N=N->Next;
          }
         KillIsolateNode();
}

void Graph::NullTransitionRemoval(){
      if(PNode==NULL)return;
      Node * N=PNode;
      NodeList * NL,*nl;
      while(N!=NULL){
         NL=NullEdgeTree(N);
         NL=RemoveNullWay(NL);
         if(NL!=NULL){
                Application->MessageBox("Null Cycle Detection.\nRemove It first.","Error",MB_OK);
                return;
         }
         delete NL;
         N=N->Next;
      }

      while(IsNullWay()){
            N=PNode;
            while(N!=NULL){
               NL=NullEdge(N);
               if(NL!=NULL){
                      for(nl=NL->Child ;nl!=NULL;nl=nl->Next){
                         CopyNode2Node(NL->NodeP,nl->NodeP);
                         EdgeRemove(EdgeFind(NL->NodeP,nl->NodeP,"E"));
                      }
                      delete NL;
               }
               N=N->Next;
            }
            KillIsolateNode();
      }
}

NodeList * Graph::FindNonDeterminestive(){
      NodeList * NL=NULL,*nl=NULL;

      for(Rule * R=Rules->frule;R!=NULL;R=R->next){
      for(Node * N=PNode;N!=NULL;N=N->Next){
        if(NL==NULL){
                NL=new NodeList(N);
                nl=NL;
        }else{
                nl->Next=new NodeList(N);
                nl=nl->Next;
        }
        int count=0;

        Edge * E=PEdge;
        while((E=EdgeFind(N,E,R->name))!=NULL){
             count++;
             nl->AddChild(E->Terminatev);
             E=E->Next;
        }

        if(count==0 || count==1){
                if(nl==NL){
                        delete nl;
                        NL=NULL;
                }else{
                        for(NodeList *lnl=NL;lnl->Next!=NULL;lnl=lnl->Next)
                           if(lnl->Next==nl){
                              lnl->Next=nl->Next;
                              delete nl;
                              nl=lnl;
                              break;
                           }
                }
        }
      }
      }
      return NL;

}

bool Graph::IsNodeWay(Node * S,Node * D){
        if(S==NULL || D==NULL)return false;
        if(S==D)return true;
        for(Edge * E=PEdge;E!=NULL;E=E->Next)
              if(E->Startv==S && E->Terminatev!=S)
                    if(IsNodeWay(E->Terminatev,D))return true;
        return false;            
}

void Graph::NonDeterminestiveRemovalOnePhase(NodeList *NL){
      for(NodeList *nl=NL;nl!=NULL;nl=nl->Next)
       for(Rule * R=Rules->frule;R!=NULL;R=R->next){
         if(nl->Child==NULL)break;
         Edge * E=EdgeFind(nl->NodeP,nl->Child->NodeP,R->name);
         if (E!=NULL){
            Node * ND=NodeAdd(nl->NodeP->X+80,nl->NodeP->Y);
            ND->Name[0]=nl->Child->NodeP->Name[0];
            ND->Name[1]=nl->Child->Next->NodeP->Name[0];
            ND->Name[2]=0;
            EdgeAdd(new Edge(nl->NodeP,ND,R->name,R->des));

            for(NodeList *cnl=nl->Child;cnl!=NULL;cnl=cnl->Next){
               EdgeRemove(EdgeFind(nl->NodeP,cnl->NodeP,R->name));
               for(Edge * LE=PEdge;LE!=NULL;LE=LE->Next){
                   if(LE->Startv==cnl->NodeP && LE->Terminatev==cnl->NodeP)
                       EdgeAdd(new Edge(ND,ND,LE->Name,LE->Des));
                   if(LE->Startv==cnl->NodeP && LE->Terminatev!=cnl->NodeP)
                       EdgeAdd(new Edge(ND,LE->Terminatev,LE->Name,LE->Des));
               }
               if(cnl->NodeP->Final)ND->Final=true;
               if(SNode==cnl->NodeP)SNode=ND;
            }
         }
       }
      RemoveDuplicatedEdge();
      KillIsolateNode();
}

void Graph::NonDeterminestiveRemoval(){
    if(PNode==NULL)return;
    NodeList * NL=NULL;
    do{
      NL=FindNonDeterminestive();
      NonDeterminestiveRemovalOnePhase(NL);
      delete NL;
    }while(NL!=NULL);
}


void Graph::NodeTraverseRemoval(){
        if(SNode==NULL){
            Application->MessageBox("No Start Node Find.","About",MB_OK);
            return;
        }
        for(Node * N=PNode;N!=NULL;N=N->Next)
                if(!IsNodeWay(SNode,N))
                        NodeRemove(N);

      KillIsolateNode();
}

TMouseButton    _CurrentButton;
Node        * _CurrentPointedNode;
Node        * _CurrentMovingNode;
Node        * _CurrentSelectedNode;
Graph       *GraphPointer;
bool        NoMoveSw;
int         CurX,CurY;
bool        GlobalClick,GlobalCancelClick;

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
        NoMoveSw=false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    _CurrentPointedNode=NULL;
    _CurrentSelectedNode=NULL;
    _CurrentMovingNode=NULL;
    Page->Brush->Color=clLtGray;
    Page->Font->Name=GroupBox1->Font->Name;
    Page->Font->Size=7;
    New1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Image1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    static class Node *v;
    StatusBar1->Panels->Items[0]->Text="Y:" + AnsiString(Y) + "    X:" + X;
    CurX=X;CurY=Y;
    v=GraphPointer->NodeEnter(X,Y);
    _CurrentPointedNode=v;
    if(v==NULL){
          Image1->Cursor=crUpArrow;
          if(StatusBar1->Panels->Items[0]->Text!="")
             StatusBar1->Panels->Items[1]->Text="";
    }else{
          StatusBar1->Panels->Items[1]->Text="Current Node :"+ AnsiString(v->Name) +
                                             " [Hold Left and move Node]"+
                                             " [Right Click : Node Functions]";
          Image1->Cursor=crHandPoint;
    }

    if(_CurrentSelectedNode!=NULL){
        GraphPointer->Show(Image1);
        Page->Pen->Color=clDkGray;
        Page->MoveTo(X,Y);
        Page->LineTo(_CurrentSelectedNode->X,_CurrentSelectedNode->Y);
     }

    if(_CurrentMovingNode!=NULL && _CurrentButton==mbLeft){
        _CurrentMovingNode->X=X;
        _CurrentMovingNode->Y=Y;
        GraphPointer->Show(Image1);
     }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ShowInfo(){
    MainForm->NodeText->      Caption=GraphPointer->NodeCount();
    MainForm->EdgeText->      Caption=GraphPointer->EdgeCount();
    MainForm->IsolateText->   Caption=GraphPointer->IsolateNodeCount();
    MainForm->SubText->       Caption=GraphPointer->SubGraphCount();
}

void __fastcall TMainForm::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    GlobalClick=true;
    if(NoMoveSw){
            NoMoveSw=false;
            return;
    }
    _CurrentButton=Button;
    if(_CurrentButton==mbRight )GlobalCancelClick=true;
    if(_CurrentButton==mbLeft &&  _CurrentSelectedNode==NULL)
    _CurrentMovingNode=_CurrentPointedNode;

    if(_CurrentSelectedNode==NULL || _CurrentPointedNode==NULL)return;
    Rules->ShowModal();
    if(Rules->ModalResult==mrOk){
         GraphPointer->EdgeAdd(_CurrentSelectedNode,_CurrentPointedNode,
                                Rules->CurRule->name,Rules->CurRule->des);
    }

    _CurrentSelectedNode=NULL;
    GraphPointer->Show(Image1);

    ShowInfo();
}
//---------------------------------------------------------------------------

void newgraph(){
    if(GraphPointer!=NULL){
        GraphPointer->empty();
        delete GraphPointer;
    }
    MainForm->Image1->Canvas->FillRect(Rect(0,
                                  0,
                                  MainForm->Image1->Width,
                                  MainForm->Image1->Height));
    GraphPointer=new Graph();
    MainForm->ShowInfo();

    if(Rules!=NULL)
      if(Rules->frule!=NULL){
        if((Rules->frule)->next!=NULL){
          delete(Rules->frule)->next;
          (Rules->frule)->next=NULL;
        }
        Rules->RefreshGrid();
      }
}

void __fastcall TMainForm::New1Click(TObject *Sender)
{
    if(GraphPointer!=NULL)
        if(Application->MessageBox("Delete current graph?","Confirm",MB_YESNO)==IDNO)return;
    newgraph();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Refresh1Click(TObject *Sender)
{
    GraphPointer->Show(Image1);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::About1Click(TObject *Sender)
{
    Application->MessageBox("NFA To DFA Convertor\n\nProgrammer : Ramtin Negahdari\nIn December 2000","About",MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Exit1Click(TObject *Sender)
{
    exit(0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::InternalSymbolsSet1Click(TObject *Sender)
{
        Rules->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Image1DblClick(TObject *Sender)
{
          if(_CurrentPointedNode!=NULL){
                StrLCopy(_CurrentPointedNode->Name,InputBox("", "Node Name (2char):",_CurrentPointedNode->Name).c_str(),2);
               _CurrentPointedNode->Visible(Image1->Canvas,clBlack);
          }
          NoMoveSw=true;
          Image1MouseMove(NULL,TShiftState() , 0,0);
}
//---------------------------------------------------------------------------

FILE * fp;

void __fastcall TMainForm::Save1Click(TObject *Sender)
{
    if(GraphPointer==NULL){
        Application->MessageBox("No Graph Avaliable.","Error",MB_OK);
        return;
    }
    if(SaveDialog1->Execute()){
        if((fp=fopen(SaveDialog1->FileName.c_str() ,"wb"))==NULL ){
                Application->MessageBox("Can't open output file.","Error",MB_OK);
                return;
        }
           fwrite(GraphPointer, sizeof(class Graph),1,fp);
           int GNC=GraphPointer->NodeCount();
           fwrite(&GNC, sizeof(GNC),1,fp);
           for(class Node * N= GraphPointer->PNode;N!=NULL;N=N->Next)
              fwrite(N, sizeof(class Node),1,fp);

           int ID;
           if(GraphPointer->SNode!=NULL)ID=GraphPointer->SNode->ID;
           else ID=-1;
           fwrite(&ID, sizeof(ID),1,fp);
           
           int RC=Rules->frule->count(1); ;
           fwrite(&RC, sizeof(RC),1,fp);
           for(Rule *R= Rules->frule;R!=NULL;R=R->next){
            fwrite(R->name, 11,1,fp);
            fwrite(R->des, 21,1,fp);
           }


           int GEC=GraphPointer->EdgeCount() ;
           int temp=-1;
           fwrite(&GEC, sizeof(GEC),1,fp);
           for(class Edge * E= GraphPointer->PEdge;E!=NULL;E=E->Next){
              fwrite(E, sizeof(class Edge),1,fp);
              if(E->Startv==NULL) fwrite(&temp,sizeof(temp),1,fp);
              else fwrite(&(E->Startv->ID),sizeof(E->Startv->ID),1,fp);

              if(E->Terminatev==NULL) fwrite(&temp,sizeof(temp),1,fp);
              else fwrite(&(E->Terminatev->ID),sizeof(E->Terminatev->ID),1,fp);
           }

        fclose(fp);

    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Open1Click(TObject *Sender)
{
    if(GraphPointer->NodeCount()!=0 )
      if(Application->MessageBox("Clear Current Graph ?","Confirm",MB_YESNO)==IDNO)return;
    newgraph();
    if(OpenDialog1->Execute()){
        if((fp=fopen(OpenDialog1->FileName.c_str() ,"rb"))==NULL ){
                Application->MessageBox("Can't open input file.","Error",MB_OK);
                return;
        }
           fread(GraphPointer, sizeof(class Graph),1,fp);
           GraphPointer->PNode=NULL;
           GraphPointer->PEdge=NULL;
           int GNC=0;
           fread(&GNC, sizeof(GNC),1,fp);
           for(int i=0;i<GNC;i++){
              class Node * N=new class Node(0,0,0);
              fread(N, sizeof(class Node),1,fp);
              GraphPointer->NodeAdd(N);
           }
           int ID;
           fread(&ID, sizeof(ID),1,fp);
           GraphPointer->SNode=GraphPointer->NodePointer(ID);


           char      Name[11];
           char      Des[21];
           int RC;
           fread(&RC, sizeof(RC),1,fp);
           fread(Name, 11,1,fp);
           fread(Des, 21,1,fp);
           for(int i=1;i<RC;i++){
            fread(Name, 11,1,fp);
            fread(Des, 21,1,fp);
            Rules->frule->add(Name,Des);
           }
           Rules->RefreshGrid();

           int GEC=0;
           int SN,TN;
           fread(&GEC, sizeof(GEC),1,fp);
           for(int i=0;i<GEC;i++){
              class Edge * E=new class Edge(0,0,"Temp","Temp");
              fread(E, sizeof(class Edge),1,fp);
              fread(&SN,sizeof(SN),1,fp);
              E->Startv=GraphPointer->NodePointer(SN);
              fread(&TN,sizeof(TN),1,fp);
              E->Terminatev=GraphPointer->NodePointer(TN);
              GraphPointer->EdgeAdd(E);
           }
           GraphPointer->Show(Image1);
           ShowInfo();

           fclose(fp);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ShowByID1Click(TObject *Sender)
{
    GraphPointer->Show(Image1,true);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::KillIsolated1Click(TObject *Sender)
{
    GraphPointer->KillIsolateNode();
    GraphPointer->Show(Image1);
    ShowInfo();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ENull0EdgeRemoval1Click(TObject *Sender)
{
     GraphPointer->NullCycleRemoval();
     GraphPointer->Show(Image1);
     ShowInfo();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Insert1Click(TObject *Sender)
{
    if(_CurrentPointedNode==NULL     &&
       _CurrentSelectedNode==NULL){
                Node * N=GraphPointer->NodeAdd(CurX,CurY);
                StrLCopy(N->Name,InputBox("", "Node Name (2char):",AnsiString(N->ID)).c_str() ,2);
                N->Visible(MainForm->Image1->Canvas,clBlack);
                ShowInfo();
    }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::PageMenuPopup(TObject *Sender)
{
    if(_CurrentSelectedNode!=NULL){
       for(int i=0;i<11;i++)
          PageMenu->Items->Items[i]->Visible=false;
       if(_CurrentPointedNode==NULL){
         _CurrentSelectedNode=NULL;
         GraphPointer->Show(Image1);
       }
       return;
    }
    if(_CurrentPointedNode==NULL){
       for(int i=1;i<11;i++)
          PageMenu->Items->Items[i]->Visible=false;
       PageMenu->Items->Items[0]->Visible=true;
    }else{
       for(int i=1;i<11;i++)
          PageMenu->Items->Items[i]->Visible=true;
       PageMenu->Items->Items[0]->Visible=false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Delete1Click(TObject *Sender)
{
    if(_CurrentPointedNode!=NULL &&
       _CurrentSelectedNode==NULL){
            if(Application->MessageBox("Delete Node ?","Confirm",MB_YESNO)==IDYES)
                 if(!GraphPointer->NodeRemove(_CurrentPointedNode))
                         Application->MessageBox("Can't Delete Node !","Error",MB_OK);
                  ShowInfo();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Rename1Click(TObject *Sender)
{
    if(_CurrentPointedNode!=NULL &&
       _CurrentSelectedNode==NULL){
                StrLCopy(_CurrentPointedNode->Name,InputBox("", "Node Name (2char):",AnsiString(_CurrentPointedNode->Name)).c_str() ,2);
                _CurrentPointedNode->Visible(MainForm->Image1->Canvas,clBlack);
                ShowInfo();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CreateEdge1Click(TObject *Sender)
{
            if(_CurrentPointedNode!=NULL)
                if(_CurrentSelectedNode==NULL){
                    _CurrentSelectedNode=_CurrentPointedNode;
                    ShowInfo();
                }    
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Image1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        _CurrentMovingNode=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ENullRemoval1Click(TObject *Sender)
{
     GraphPointer->NullTransitionRemoval();
     GraphPointer->Show(Image1);
     ShowInfo();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::NonDeterminestiveRemoval1Click(TObject *Sender)
{
     GraphPointer->NonDeterminestiveRemoval();
     GraphPointer->Show(Image1);
     ShowInfo();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StartNode1Click(TObject *Sender)
{
    if(_CurrentPointedNode!=NULL &&
       _CurrentSelectedNode==NULL){
            if(GraphPointer->SNode==_CurrentPointedNode){
                 if(Application->MessageBox("Remove Start State from Node?","Confirm",MB_YESNO)==IDNO)return;
                 GraphPointer->SNode=NULL;
            }else{
                 if(Application->MessageBox("Make This Node Start?","Confirm",MB_YESNO)==IDNO)return;
                 GraphPointer->SNode=_CurrentPointedNode;
            }


            GraphPointer->Show(Image1);
            ShowInfo();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FinalState1Click(TObject *Sender)
{
    if(_CurrentPointedNode!=NULL &&
       _CurrentSelectedNode==NULL){
            if(_CurrentPointedNode->Final){if(Application->MessageBox("Remove Final State from Node?","Confirm",MB_YESNO)==IDNO)return;
            }else if(Application->MessageBox("Make This Node Final State?","Confirm",MB_YESNO)==IDNO)return;
             _CurrentPointedNode->Final=!_CurrentPointedNode->Final;
            GraphPointer->Show(Image1);
            ShowInfo();
    }

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::NFADFA1Click(TObject *Sender)
{
     GraphPointer->NullCycleRemoval();
     GraphPointer->NullTransitionRemoval();
     GraphPointer->NonDeterminestiveRemoval();
     GraphPointer->NodeTraverseRemoval();
     GraphPointer->Show(Image1);
     ShowInfo();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::NonDeterminestiveRemovalOnePhase1Click(
      TObject *Sender)
{
    if(GraphPointer->PNode==NULL)return;
    NodeList * NL=NULL;
    NL=GraphPointer->FindNonDeterminestive();
    GraphPointer->NonDeterminestiveRemovalOnePhase(NL);
    delete NL;
    GraphPointer->Show(Image1);
    ShowInfo();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::NullWayRemoval1Click(TObject *Sender)
{
     GraphPointer->NodeTraverseRemoval();
     GraphPointer->Show(Image1);
     ShowInfo();
}
//---------------------------------------------------------------------------

Edge * SelectEdge(Node * N){
        if(N==NULL)return NULL;
        Edge * Result;
        if((Result=GraphPointer->EdgeFind(N,GraphPointer->PEdge))==NULL)return NULL;
        int LX=CurX,LY=CurY;
        GlobalCancelClick=GlobalClick=false;
        TColor    cl=Result->cl;
        Result->Visible(MainForm->Image1->Canvas,clBlue);
        while(!GlobalClick){
                  if(abs(LX-CurX)>10 || abs(LY-CurY)>10){
                        Result->Visible(MainForm->Image1->Canvas,cl);
                        Result=GraphPointer->EdgeFind(N,Result->Next);
                        if(Result==NULL)
                          Result=GraphPointer->EdgeFind(N,GraphPointer->PEdge);
                        LX=CurX;LY=CurY;
                        cl=Result->cl;
                        Result->Visible(MainForm->Image1->Canvas,clBlue);
                  }
                  Application->ProcessMessages();
       }
       Result->Visible(MainForm->Image1->Canvas,cl);
       if(GlobalCancelClick)return NULL;
       return Result;
}

void __fastcall TMainForm::EditEdge1Click(TObject *Sender)
{
        Edge * E;
        if((E=SelectEdge(_CurrentPointedNode))==NULL)return;
        Rules->ShowModal();
        if(Rules->ModalResult==mrOk){
                Edge *ET=GraphPointer->EdgeFind(E->Startv,E->Terminatev, Rules->CurRule->name);
                if(ET!=NULL && ET!=E){
                        Application->MessageBox("Duplicated Edge","Error",MB_OK);
                        return ;
                }

            StrLCopy(E->Name,Rules->CurRule->name,10);
            StrLCopy(E->Des,Rules->CurRule->des,20);
            GraphPointer->Show(Image1);
            ShowInfo();
        }

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Delete2Click(TObject *Sender)
{
        Edge * E;
        if((E=SelectEdge(_CurrentPointedNode))==NULL)return;
        if(Application->MessageBox("Delete Edge ?","Confirm",MB_YESNO)==IDYES)
           if(!GraphPointer->EdgeRemove(E))
               Application->MessageBox("Can't Delete Edge !","Error",MB_OK);

        GraphPointer->Show(Image1);
        ShowInfo();

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CurveEdge1Click(TObject *Sender)
{
        Edge * E;
        if((E=SelectEdge(_CurrentPointedNode))==NULL)return;
        int LY=CurY;
        GlobalCancelClick=GlobalClick=false;
        float M=E->Mplex;
        while(!GlobalClick){
                  if(abs(LY-CurY)>10){
                        if(LY-CurY>10)E->Mplex+=0.1;
                        else E->Mplex-=0.1;
                        LY=CurY;
                        GraphPointer->Show(Image1);
                  }
                  Application->ProcessMessages();
       }
       if(GlobalCancelClick)E->Mplex=M;
       GraphPointer->Show(Image1);

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UpdateEdge(char * c)
{
       GraphPointer->EdgeAllRemove(c);
       GraphPointer->Show(Image1);

}
