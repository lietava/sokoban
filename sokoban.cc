#include <cstdio>
#include <cstdint>
#include <utility>
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

uint Nrow = 4;
uint Ncol = 5;
const int NMAX=100;
//-----------------------------------------------------------
void splitstring(const std::string& str,
                      std::vector<std::string>& tokens,
                      const std::string& delimiters = " ")
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
//-------------------------------------------------------------------------
int char2int(char c){
 int i=-1;
 switch(c){
  case '0': i=0;break;
  case '1': i=1;break;
  case '2': i=2;break;
  case '3': i=3;break;
  case '4': i=4;break;
  case '5': i=5;break;
  case '6': i=6;break;
  case '7': i=7;break;
  case '8': i=8;break;
  case '9': i=9;break;
  case 'a': i=10;break;
  case 'b': i=11;break;
  case 'c': i=12;break;
  case 'd': i=13;break;
  case 'e': i=14;break;
  case 'f': i=15;break;
  default: 
  //cout << "character " << c << " not in table" << endl;
  printf("Char %c not in table \n",c);
 }
 return i;
}

//------------------------------------------------------------
// Converts decadic string>0  to int
int string2int(std::string s){
 int n=s.length();
 int num=0;
 int d=1;
 for(int i=0;i<n;i++){
  int digit=char2int(s[n-1-i]);
  if(digit == -1) return -1;
  num=num + d*digit;
  d=d*10;
 }
 return num;
}


class TreeNode 
{
private:
    uint i,j;            // Node coordinate = sokoban position
    bool type;          // 0/1 - box moved or not
    uint iters;          // number of iterations
    uint inpos;
    uint branch;           // 1 = 01, 2=10, 3=-1-, 4= 0-1
    uint pushes;
    TreeNode *parent;
    //std::vector<TreeNode*> children;
    //TreeNode *children[10000];
    uint **pboard;
public:
    TreeNode();
    ~TreeNode();
    TreeNode(uint ix,uint jx,bool type,TreeNode* parent);
    int CopyBoard();
    bool GetType(){return type;};
    uint GetX(){return i;};
    uint GetY(){return j;};
    int GetIters(){return iters;};
    uint GetInPos(){return inpos;};
    //int GetNChildren(){return children.size();};
    //TreeNode* GetChild(int ii){return children[ii];};
    uint GetBranch(){return branch;};
    void InPosition(uint ii){inpos+=ii;};
    void Iter(){iters++;};
    //void AddChild(TreeNode *child,int ii){children[ii]=child;}
    void SetXY(uint ii, uint jj){i=ii;j=jj;};
    void SetBranch(uint ii){branch=ii;};
    void SetInPos(uint ii){inpos=ii;};
    void IncreaseBranch(){branch++;};
    uint** GetBoard(){return pboard;};
    TreeNode* GetParent(){return parent;};
    void PrintBoard();
};
TreeNode::TreeNode():
i(0),j(0),type(1),iters(0),inpos(0),branch(0),parent(0),pboard(0)
{
  pboard = new uint*[Nrow];
  for(uint i=0;i<Nrow;i++)
  {
    pboard[i] = new uint[Ncol];
    for(uint j=0;j<Ncol;j++)pboard[i][j]=0;
  }
  //for(uint i=0;i<4;i++)children[i]=0;
}
TreeNode::~TreeNode() 
{
 //printf("Node %p deleted \n",this);   
 if(pboard)
 {
  for(uint i=0;i<Nrow;i++)
  {
    delete [] pboard[i];
  }
  delete [] pboard;
 }
}
TreeNode::TreeNode(uint ix,uint jx, bool type, TreeNode *parent):
i(ix),j(jx),type(type),iters(parent->GetIters()),inpos(parent->GetInPos()),branch(0),parent(parent),pboard(0)
{
 //printf("Parent: %p \n",parent);
 CopyBoard(); 
 //for(int i=0;i<4;i++)children[i]=0;
}
int TreeNode::CopyBoard()
{ 
  if(!parent)
  {
    printf("Copyboard should not be used for parent = 0 \n");
    return 1;
  }
  pboard = new uint*[Nrow];
  for(uint i=0;i<Nrow;i++)
  {
    pboard[i] = new uint[Ncol];
    for(uint j=0;j<Ncol;j++)pboard[i][j]=parent->pboard[i][j];
  }
  return 0;
}
void TreeNode::PrintBoard()
{
 for(uint i=0;i<Nrow;i++)
 {
   for(uint j=0;j<Ncol;j++)printf("%1x",pboard[i][j]);
   printf("\n");
 }
}
/******************************************************************************
 */
class Sokoban
{
 public:
        Sokoban(int NITER);
        ~Sokoban();
 	void InputBoard();
 	void InputBoard(std::string filename);
	int Explore(TreeNode *node);
	int DoBoard(std::string file);
	int FindSokoban(uint &i, uint &j);
	void PrintBoard();
	void PrintNode(TreeNode* node);
	void PrintTree(TreeNode* node);
	void PrintChildren(TreeNode* node);
	int ReadBoard(std::string filename);
        int GetNnodes(){return Nnodes;};
        int GetNIters(){return fiter;};
        void SetDebug(bool deb){debug=deb;};
 private:
	enum {fout=0, fin=1, ftar=2, fbox=4, fsoko=8, fpath=16 };
	// 0 not allowed; 1 board ;2 target field; 4 box; 8 sokoban; 
        int NITER;
        uint inPosTar,inPosBox;
        int fiter;
        int Nnodes;
        bool debug;
	TreeNode *first;
 	uint **board;
	std::vector< std::vector<uint> > rows;
	std::vector< std::vector<uint> > cols;
	bool isSoko(uint&, uint&j);
        bool isBox(uint i,uint j);
        bool isBoxEdge(uint i,uint j);
        bool isPos(uint i,uint j);
        bool isPosBox(uint i,uint j);
        bool isSquare(uint i,uint j,int dx,int dy);
        uint GetBranch(int dx,int dy);
        void GetChild(uint branch, int& dx, int &dy);
	TreeNode* go(TreeNode* node);
};
Sokoban::Sokoban(int NITER):
NITER(NITER),inPosTar(0),inPosBox(0),fiter(0),Nnodes(0),debug(0),first(0)
{
}
Sokoban::~Sokoban()
{
    if(first) delete first;
}
uint Sokoban::GetBranch(int dx, int dy)
{
    int child=10;
    if(dx==1)child=0;
    else if(dx==-1) child=1;
    else if(dy==1) child=2;
    else if(dy==-1) child=3;
    else printf("Error in child \n"); 
    return child;
        
}
void Sokoban::GetChild(uint branch, int& dx, int& dy)
{
    switch(branch)
    {
        case 0: dx= 1;dy= 0; return;
        case 1: dx=-1;dy= 0; return ;
        case 2: dx= 0;dy= 1; return;
        case 3: dx= 0;dy=-1; return;
        default: printf("Error: wrong branch %i\n",branch);
    }
}
int Sokoban::ReadBoard(std::string filename)
{
 std::ifstream file(filename.c_str());
 if(file.is_open())
 {
  std::string line;
  uint irow=0;
  inPosTar=0;
  inPosBox=0;
  uint inpos=0;
  while(getline(file,line))
  {
   if(line[0]=='#') continue;
   if(line[0]=='r')
   {
     std::vector<std::string> tokens;
     splitstring(line,tokens," ");
     Nrow = string2int(tokens[1]);
     Ncol = string2int(tokens[2]);
     printf("Nrow=%i Ncol=%i \n",Nrow,Ncol);
     first = new TreeNode;
     board = first->GetBoard();
     continue;
   }
   else
   {
    //printf("%s \n",line.c_str());
    if(line.size() != Ncol)
    {
     printf("Error Ncol != line.size %i %i\n",Ncol,(uint)line.size());
     return 1;
    }
    for(uint i=0;i<Ncol;i++)
    {
      board[irow][i] = char2int(line[i]);
      if(isSoko(irow,i)) first->SetXY(irow,i);
      if(isPos(irow,i))inPosTar++;
      if(isBox(irow,i))inPosBox++;
      if(isPosBox(irow,i))inpos++;
    }
    irow++;
   }
  }
  first->SetInPos(inpos);
  if(irow != Nrow)
  {
    printf("Error irow != nrow %i %i\n", irow, Nrow);
    return 1;
  }
  if(inPosTar != inPosBox)
  {
   printf("Error: targets != boxes %i %i \n",inPosTar,inPosBox);
   return 1;
  }
  fiter=0;
  //inPosTar=1;
  first->PrintBoard();
  printf("InPosTar: %i \n \n",inPosTar);
  return 0;
 }
 else
 {
  printf("Cannot open file %s\n",filename.c_str());
  return 1;
 }
}
void Sokoban::PrintBoard()
{
 for(uint i=0;i<Nrow;i++)
 {
   for(uint j=0;j<Ncol;j++)printf("%1x",board[i][j]);
   printf("\n");
 }
}
bool Sokoban::isSoko(uint &i, uint &j)
{
  return (board[i][j] & fsoko) == fsoko;
}
bool Sokoban::isBox(uint i, uint j)
{
    return (board[i][j]&(fin+fbox))==(fin+fbox);
}
bool Sokoban::isBoxEdge(uint i, uint j)
{
    bool box = (board[i][j]&(fin+fbox))==(fin+fbox);
    bool edge = (board[i][j] == (fout));
    return box || edge;
}
/*
 * i,j - sokoban position
 * dx, dy
 * 1 0  /-1 0
 * 0 1  / 0 -1 */
bool Sokoban::isPos(uint i,uint j)
{
    return (board[i][j]&(fin+ftar))==(fin+ftar);
 
}
bool Sokoban::isPosBox(uint i,uint j)
{
    return (board[i][j]&(fin+ftar+fbox))==(fin+ftar+fbox);
 
}
bool Sokoban::isSquare(uint i, uint j,int dx,int dy)
{
    //                                     direct
    return isBoxEdge(i+dy,j+dx) && isBoxEdge(i+dx,j+dy) && isBoxEdge(i+dx+dy,j+dx+dy);
   
}
TreeNode* Sokoban::go(TreeNode* node)
{
 //printf("Iters: %i \n",node->GetIters());
 //if(node->GetBranch()>3) return node;   
 if(node->GetIters()> NITER)
 {
  if(debug)printf("Too many iters \n");
  //node->SetBranch(4);
  node->IncreaseBranch();
  //printf("branch: %u \n",node->GetBranch());
  return node;
 } 
 uint ibranch = node->GetBranch();
 int dx,dy;
 this->GetChild(ibranch,dx,dy);
 //printf("go: node %p branch: %i dx dy: %i %i \n",node,ibranch,dx,dy);
 uint i = node->GetX();
 uint j = node->GetY();
 board = node->GetBoard();
 //node->PrintBoard();
 //PrintBoard();
 if(debug)
 {
    printf("go start %p {%i %i}",node,i,j);
    printf("{%i %i}  iter= %i",i+dx, j+dy,node->GetIters());
 }
 i+=dx;
 j+=dy;
 switch (board[i][j])
 {
  case (fout):
  {
     // edge
     if(debug)printf("Edge \n");
     node->IncreaseBranch(); 
     return node;
  }   
  case (fin):
  case (fin+ftar):
  {
     // empty field
     if(node->GetParent())
     {
        uint ip=node->GetParent()->GetX();
        uint jp=node->GetParent()->GetY();
	//printf("Parent position  %i %i \n",ip,jp);
	if((ip==i) && (jp==j) && node->GetType())
	{
	 if(debug)printf("Empty repetition \n");
         node->IncreaseBranch();   
	 return node;
	}
     }
     TreeNode *newpos = new TreeNode(i,j,1,node);
     //newpos->GetParent()->AddChild(newpos,ibranch);
     Nnodes++;
     board=newpos->GetBoard();
     board[i][j]+=fsoko;
     board[i-dx][j-dy]-=fsoko;
     newpos->Iter();
     if(debug)printf(" Empty moved to NEW node %p\n",newpos);
     return newpos;
  }
  case(fin+fbox+ftar):
  case(fin+fbox):
  {
     // box 
     if(board[i+dx][j+dy] == fout) 
     { 
       if(debug)printf("Box on edge \n");
       node->IncreaseBranch();  
       return node;
     }  
     if(isBox(i+dx,j+dy))
     { 
       //PrintBoard();
       if(debug)printf("Box on box %i %i \n",i+dx,j+dy);
       node->IncreaseBranch();  
       return node;
     }  
     if(isSquare(i+dx,j+dy,dx,dy) && (!isPos(i+dx,j+dy)))
     {
         if(debug)printf("Box in square %i %i %i %i\n",i,j,dx,dy);
         //PrintBoard();
         node->IncreaseBranch();
         return node;
     }
     // move box
     TreeNode *newpos = new TreeNode(i,j,0,node);
     Nnodes++;
     //newpos->GetParent()->AddChild(newpos,ibranch);
     board = newpos->GetBoard();
     if(isPos(i,j))newpos->InPosition(-1);
     if(isPos(i+dx,j+dy))newpos->InPosition(+1);
     //printf("inPositions: %i %i \n",newpos->GetInPos(),inPosTar);
     board[i+dx][j+dy]+=fbox;
     board[i][j] -= fbox;
     board[i][j]+=fsoko;
     board[i-dx][j-dy]-=fsoko;
     if(debug)printf("Box moved NEW node %p type:%i\n",newpos,newpos->GetType());
     newpos->Iter();
     return newpos;
 }
 default:
     printf("ERRRRRRRRRRor %i %i %i \n",i,j,board[i][j]);
     return 0;
 }
}
void Sokoban::PrintNode(TreeNode* node)
{
  TreeNode* iter=node;
  printf("{%i %i}",iter->GetX(),iter->GetY());
  int nn=0;
  while((iter=iter->GetParent()))
  {
    printf("{%i %i}",iter->GetX(),iter->GetY());
    nn++;
  }
  printf(" %i \n", nn);
}
/*
void Sokoban::PrintChildren(TreeNode* node)
{
 int nch = node->GetNChildren();
 for(int i=0;i<nch;i++)
 {
   TreeNode* child = node->GetChild(i);
   printf("{%i %i}",node->GetX(),node->GetY());
 }
}
 */ 
/*
void Sokoban::PrintTree(TreeNode* node)
{
  std::queue<TreeNode*> fifo;
  int ii=0;
  fifo.push(node);
  //int NMAX=10;
  int ix=0;
  printf("TOP: {%i %i %p}\n",node->GetX(),node->GetY(),node);
  while(fifo.size() && (ix<NMAX))
  {
   ix++;
   TreeNode* no = fifo.front();
   fifo.pop();
   printf("fifo size: %i \n",fifo.size());
   if(ii != no->GetIters())
   {
     ii=no->GetIters();
     printf("\n Level: %i | \n",ii);
   }
   int nch = no->GetNChildren();
   printf("node %p # children: %i \n",no,nch);
   for(int i=0;i<nch;i++)
   {
     TreeNode* child = no->GetChild(i);
     printf("{%i %i %p}",child->GetX(),child->GetY(),child);
     fifo.push(child);
   }
   printf("\n");
  }
  if(ix==NMAX)printf("WARNING PrintTree cut\n");
}
 */
void Sokoban::InputBoard()
{
 first = new TreeNode;
 board = first->GetBoard();
 for(uint i=0;i<Nrow;i++)
 {
 	for(uint j=0;j<Ncol;j++)board[i][j]=fout;
 }
 // 1,1, behinng of board
 board[1][1]+=fin;
 board[1][2]+=fin;
 board[1][3]+=fin;
 board[2][1]+=fin;
 board[2][2]+=fin;
 board[2][3]+=fin;
 board[1][1]+=fsoko;
 first->SetXY(1,1);
 board[2][2]+=fbox;
 board[2][3]+=ftar;
 fiter=0;
 inPosTar=1;
}
int Sokoban::FindSokoban(uint &i, uint &j)
{
 for (i=0;i<Nrow;i++)
 {
  for (j =0; j<Ncol;j++)
  {
   //printf("%i %i %i \n",i,j,soko(i,j));
   if(isSoko(i,j)) return 1;
  }
 }
 return 0;
}
int Sokoban::DoBoard(std::string file)
{
 if(ReadBoard(file))return 1;
 Explore(first);
 //PrintTree(first);
 return 0;
}
int Sokoban::Explore(TreeNode* node)
{
 //fflush(stdout);
 fiter++;
 if((fiter%100000000)==0)printf("Nnodes= %i iters: %i \n",Nnodes,fiter);
 if(debug)printf("Explore: node %p InpPos: %i %i \n",node,node->GetInPos(),inPosTar);
 if(node->GetInPos() == inPosTar)
 {
     printf("FOUND: ");
     PrintNode(node);
     //node->PrintBoard();
     if(NITER > node->GetIters())NITER = node->GetIters();
     //printf("# iters: %i %i \n",fiter,node->GetIters());
     fiter--;
     return 0;
 }
 while (node && (node->GetBranch() > 3))
 {
     //printf("inside \n");
     TreeNode* parent = node->GetParent();
     if(parent==0) return 0;
     delete node;
     node=parent;
     node->IncreaseBranch();
     Nnodes--;
 }
 TreeNode* next=go(node);
 if(next==0)
 {
     printf("here \n");
     fiter--;
     return 0;
 }
 //printf("Explore: next: %p branch: %i \n",next, next->GetBranch());
 return Explore(next);
 //fiter--;
 //return 0;
}
int main()
{
 int NITER=33;
 Sokoban ss(NITER);
 ss.SetDebug(0);
 clock_t begin=clock();
 //ss.DoBoard("soko3.txt");
 //ss.DoBoard("aceasy07.txt");
 //ss.DoBoard("akk01.txt");
 //ss.DoBoard("aenigma01.txt");
 ss.DoBoard("sokomob28.txt");
 clock_t end=clock();
 //printf("sizeof TreeNode %i \n",sizeof(TreeNode));
 printf("Nnodes: %i Iters: %i \n",ss.GetNnodes(),ss.GetNIters());
 double elapsedsecs =  double(end-begin)/CLOCKS_PER_SEC;
 printf("Time: %i %i %f \n",(uint)end,(uint)begin,elapsedsecs);
 return 0;
}
