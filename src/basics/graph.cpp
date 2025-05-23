/**
 * @author  Christian Schulz
 * @version 3.0
 */
 
#include <stdlib.h>
#include "graph.hpp"

using namespace std;

//**************************************************************************

void Lisa_Graph::write(ostream & strm)const{
  
  if(strm){
    G_ExceptionList.lthrow("No valid stream in Lisa_Graph::write().",
                           Lisa_ExceptionList::ANY_ERROR);
    return;
  }
  
  Lisa_Matrix<int> out(get_vertices(),get_vertices());
  get_adjacency_matrix(&out);

  strm << endl << "<GRAPH>" << endl;
  strm << "vertices= "<< get_vertices() << endl;
  strm << "adjacency_matrix= " << out << endl; 
  strm << "</GRAPH>" <<endl;

}

//**************************************************************************

void Lisa_Graph::read(istream & strm){
  
  if(strm){
    G_ExceptionList.lthrow("No valid stream in Lisa_Graph::read().",
                           Lisa_ExceptionList::ANY_ERROR);
    return;
  }

  string S;
  int new_size;
  Lisa_Matrix<int>* adj_Matrix=0;
  for (;;){
    S=""; 
    strm >> S;
    if (S==""){ 
      G_ExceptionList.lthrow((string) "Unexpected end of file while looking for starttag <GRAPH>.",
                             Lisa_ExceptionList::END_OF_FILE);
      return;
    } 
    if (S=="<GRAPH>") break;
  }
  
  S=""; 
  strm >> S; 
  if (S=="vertices=") {  
    strm >> new_size; 
    init(new_size);
    adj_Matrix=new Lisa_Matrix<int>(new_size,new_size);
  }else{
    G_ExceptionList.lthrow("'vertices=' expected in <GRAPH>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  S=""; 
  strm >> S; 
  if (S=="adjacency_matrix="){  
    strm >> *adj_Matrix;
    set_adjacency_matrix(adj_Matrix);
    if(adj_Matrix) delete adj_Matrix;
  }else{
    G_ExceptionList.lthrow("'adjacency_matrix=' expected in <GRAPH>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  S=""; 
  strm >> S; 
  if (S!="</GRAPH>"){
    G_ExceptionList.lthrow("'</GRAPH>' expected in <GRAPH>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;  
  } 

}

//**************************************************************************
//**************************************************************************
//**************************************************************************

int Lisa_MatrixListGraph::signum(const int start,const int end)const{
  if(start==end) return 0;
  
  if(((*matrix)[start][end].x>0)&&((*matrix)[start][end].y>0)){
    return 1;
  }else{
    return -1;
  }
}

//**************************************************************************

void Lisa_MatrixListGraph::ins_edge(const int start,const int end){
  
  int end_of_NB_List=(*matrix)[start][start].y;
  
  if( end_of_NB_List == size+1){
    (*matrix)[start][start].x=end;
  }else{
    (*matrix)[start][end_of_NB_List].x=end;
  }
  
  (*matrix)[start][start].y=end;
  (*matrix)[start][end].x=size+1;
  (*matrix)[start][end].y=end_of_NB_List;
  
  end_of_NB_List=(*matrix)[end][end].y;
  
  if( end_of_NB_List == size+1){
    (*matrix)[end][end].x=start;
  }else{
    (*matrix)[end][end_of_NB_List].x=start;
  }
  
  (*matrix)[end][end].y=start;
  (*matrix)[end][start].x=size+1;
  (*matrix)[end][start].y=end_of_NB_List;
    
}
  
//**************************************************************************  

void Lisa_MatrixListGraph::ins_arc(const int start,const int end){
  
  int end_of_Succ_List=(*matrix)[start][0].y;
  if( end_of_Succ_List== size+1){
    (*matrix)[start][0].x=end;
  }else{
    (*matrix)[start][end_of_Succ_List].x=end;
  }
  
  (*matrix)[start][0].y=end;
  (*matrix)[start][end].x=size+1;
  (*matrix)[start][end].y=end_of_Succ_List;
  
  
  
  int end_of_Pred_List=(*matrix)[0][end].y;
  if( end_of_Pred_List == size+1){
    (*matrix)[0][end].x=start;
  }else{
    (*matrix)[end][end_of_Pred_List].x=-start;
  }
  
  (*matrix)[0][end].y=start;
  (*matrix)[end][start].x=-(size+1);
  (*matrix)[end][start].y=-end_of_Pred_List;
  
}
  
//**************************************************************************
  
void Lisa_MatrixListGraph::rem_edge(const int start,const int end){
  int succ_of_edge=(*matrix)[start][end].x;
  int pred_of_edge=(*matrix)[start][end].y;
  
  if(succ_of_edge==size+1){
    (*matrix)[start][start].y=pred_of_edge;
  }else{
    (*matrix)[start][succ_of_edge].y=pred_of_edge;
  }
  
  if(pred_of_edge==size+1){
    (*matrix)[start][start].x=succ_of_edge;
  }else{
    (*matrix)[start][pred_of_edge].x=succ_of_edge;
  }
  
  (*matrix)[start][end].x=0;
  (*matrix)[start][end].y=0;
    
  succ_of_edge=(*matrix)[end][start].x;
  pred_of_edge=(*matrix)[end][start].y;
  
  if(succ_of_edge==size+1){
    (*matrix)[end][end].y=pred_of_edge;
  }else{
    (*matrix)[end][succ_of_edge].y=pred_of_edge;
  }
  
  if(pred_of_edge==size+1){
    (*matrix)[end][end].x=succ_of_edge;
  }else{
    (*matrix)[end][pred_of_edge].x=succ_of_edge;
  }
  
  (*matrix)[end][start].x=0;
  (*matrix)[end][start].y=0;
}
  
//**************************************************************************

void Lisa_MatrixListGraph::rem_arc(const int start,const int end){
  int succ_of_arc=(*matrix)[start][end].x;
  int pred_of_arc=(*matrix)[start][end].y;
  
  if(succ_of_arc==size+1){
    (*matrix)[start][0].y=pred_of_arc;
  }else{
    (*matrix)[start][succ_of_arc ].y=pred_of_arc;
  }
  
  if(pred_of_arc==size+1){
    (*matrix)[start][0].x=succ_of_arc;
  }else{
    (*matrix)[start][pred_of_arc].x=succ_of_arc;
  }
  
  (*matrix)[start][end].x=0;
  (*matrix)[start][end].y=0;
  
  succ_of_arc=(*matrix)[end][start].x;
  pred_of_arc=(*matrix)[end][start].y;
  
  if(succ_of_arc==-(size+1)){
    (*matrix)[0][end].y=-pred_of_arc;
  }else{
    (*matrix)[end][-succ_of_arc].y=pred_of_arc;
  }
  
  if(pred_of_arc==-(size+1)){
    (*matrix)[0][end].x=-succ_of_arc;
  }else{
    (*matrix)[end][-pred_of_arc ].x=succ_of_arc ;
  }
  
  (*matrix)[end][start].x=0;
  (*matrix)[end][start].y=0;
}
  
//**************************************************************************

Lisa_MatrixListGraph::Lisa_MatrixListGraph(const int number_of_knots){
  matrix=0;
  succ_pred_pointer=0;
  edge_pointer=0;
  init(number_of_knots);
}

//**************************************************************************

Lisa_MatrixListGraph::Lisa_MatrixListGraph(const Lisa_MatrixListGraph *const othergraph){
  matrix=0;
  succ_pred_pointer=0;
  edge_pointer=0;
  init(othergraph->get_vertices());
  
  for(int i=0; i<=size; i++){
    for(int j=0; j<=size; j++){
      (*matrix)[i][j].x=(*(othergraph->matrix))[i][j].x;
      (*matrix)[i][j].y=(*(othergraph->matrix))[i][j].y;
    }
  }
}
  
//**************************************************************************

Lisa_MatrixListGraph::Lisa_MatrixListGraph(const Lisa_Graph *const othergraph){
  matrix=0;
  succ_pred_pointer=0;
  edge_pointer=0;
  init(othergraph->get_vertices());
  
  Lisa_Matrix<int> out(size,size);
  othergraph->get_adjacency_matrix(&out);
  set_adjacency_matrix(&out);
}
  
//**************************************************************************

Lisa_MatrixListGraph::Lisa_MatrixListGraph(const Lisa_MatrixListGraph& othergraph){
  matrix=0;
  succ_pred_pointer=0;
  edge_pointer=0;
  init(othergraph.get_vertices());
  
  for(int i=0; i<=size; i++){
    for(int j=0; j<=size; j++){
      (*matrix)[i][j].x=(*(othergraph.matrix))[i][j].x;
      (*matrix)[i][j].y=(*(othergraph.matrix))[i][j].y;
    }
  }
}

//**************************************************************************

Lisa_MatrixListGraph::Lisa_MatrixListGraph(const Lisa_Graph & othergraph){
  matrix=0;
  succ_pred_pointer=0;
  edge_pointer=0;
  init(othergraph.get_vertices());
  
  Lisa_Matrix<int> out(size,size);
  othergraph.get_adjacency_matrix(&out);
  set_adjacency_matrix(&out);
}

//**************************************************************************

Lisa_MatrixListGraph::~Lisa_MatrixListGraph(){
  if(matrix) delete matrix;
  if(succ_pred_pointer) delete succ_pred_pointer;
  if(edge_pointer) delete edge_pointer;
}

//**************************************************************************

void Lisa_MatrixListGraph::init(const int n_in){  
  size = n_in;
  
  if(matrix) delete matrix;
  matrix= new Lisa_Matrix<Lisa_Pair>(size+1,size+1);
  
  if(succ_pred_pointer) delete succ_pred_pointer;
  succ_pred_pointer=new Lisa_Vector<Lisa_Pair>(size);
  
  if(edge_pointer) delete edge_pointer;
  edge_pointer=new Lisa_Vector<int>(size);
  
  clear();
}

//**************************************************************************

void Lisa_MatrixListGraph::clear(){
  
  for (int i=0; i<=size; i++){
    for (int j=0; j<=size; j++){
      if((i==0||j==0)||(i==j)){
        (*matrix)[i][j].x=size+1;
        (*matrix)[i][j].y=size+1;
      }else{
        (*matrix)[i][j].x=0;
        (*matrix)[i][j].y=0;
      }
    }
  }
  
  for(int i=0; i<size; i++){
    (*succ_pred_pointer)[i].x=i+1;
    (*succ_pred_pointer)[i].y=i+1;
    (*edge_pointer)[i]=i+1;
  }
  
}

//**************************************************************************
  
void Lisa_MatrixListGraph::get_adjacency_matrix(Lisa_Matrix<int> *const adj)const{
#ifdef LISA_DEBUG
  if(adj->get_n() != size || adj->get_m() != size){
    G_ExceptionList.lthrow("Wrong matrix size in argument to Lisa_MatrixListGraph::get_adjacency_matrix().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  for(int i=1; i<=size; i++){
    for(int j=1; j<=size; j++){
      
      if(signum(i,j)==1) {
        (*adj)[i-1][j-1]=1;
      }else{
        (*adj)[i-1][j-1]=0;
      }
      
    }
  }
  
}

//**************************************************************************

void Lisa_MatrixListGraph::set_adjacency_matrix(const Lisa_Matrix<int> *const adj){
#ifdef LISA_DEBUG
  if(adj->get_n() != size || adj->get_m() != size){
    G_ExceptionList.lthrow("Wrong matrix size in argument to Lisa_MatrixListGraph::set_adjacency_matrix().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif  
  
  clear();
  
  for(int i=0; i<size; i++){
    for(int j=i+1; j<size; j++){
    
      if( (*adj)[i][j] && (*adj)[j][i] ){
        ins_edge(i+1,j+1);
      }else if( (*adj)[i][j] ){
        ins_arc(i+1,j+1);
      }else if( (*adj)[j][i] ){
        ins_arc(j+1,i+1);
      }
      
    }
  }

}

//**************************************************************************

void Lisa_MatrixListGraph::insert_edge(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixListGraph::insert_edge().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif
  
  if(start==end) return;
  
  const int con=get_connection(start, end);
  
  if(con==ARC){
    rem_arc(start,end);
    ins_edge(start,end);
  }else if(con==CRA){
    rem_arc(end,start);
    ins_edge(start,end);
  }else if(con==NONE){
    ins_edge(start,end);
  }
  
}

//**************************************************************************

void Lisa_MatrixListGraph::insert_arc(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixListGraph::insert_arc().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  if(start==end) return;
  
  const int con=get_connection(start, end);
  
  if(con==CRA){
    rem_arc(end,start);
    ins_edge(start,end);
  }else if(con==NONE){
    ins_arc(start,end);
  }

}

//**************************************************************************

void Lisa_MatrixListGraph::remove_edge(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixListGraph::remove_edge().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  if(start==end) return;

  const int con=get_connection(start,end);
  
  if(con==EDGE){
    rem_edge(start,end);
  }else if(con==ARC){
    rem_arc(start,end);
  }else if(con==CRA){
    rem_arc(end,start);
  }

}

//**************************************************************************

void Lisa_MatrixListGraph::remove_arc(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixListGraph::remove_arc().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  if(start==end) return;
  
  //Test ob Bogen vorhanden
  const int con=get_connection(start,end);
  
  if(con==ARC){
    rem_arc(start,end);
  }else if(con==EDGE){
    rem_edge(start, end);
    ins_arc(end, start);
  }
  
}

//**************************************************************************

void Lisa_MatrixListGraph::clear(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::clear().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  //EDGES
  int next=(*matrix)[knot][knot].x;
  while(next<=size){
    rem_edge(knot,next);
    next=(*matrix)[knot][knot].x;
  }

  //ARCS
  next=(*matrix)[knot][0].x;
  while(next<=size){
    rem_arc(knot,next);
    next=(*matrix)[knot][0].x;
  } 

  //CRA
  next=(*matrix)[0][knot].x;
  while(next<=size){
    rem_arc(next,knot);
    next=(*matrix)[0][knot].x;
  } 
  
}

//**************************************************************************

int Lisa_MatrixListGraph::get_connection(const int start,const int end)const{
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixListGraph::get_connection().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return NONE;
  }
#endif

  if(start==end) return NONE;
  
  if((((*matrix)[start][end]).x==0)&&(((*matrix)[start][end]).y==0)){
    return NONE;
  }else{

    if(signum(start, end)==-1){
      return CRA;
    }else{

      if(signum(end, start)==-1){
        return ARC;
      }else{
        return EDGE;
      }
    }
  }
  
}

//**************************************************************************

void Lisa_MatrixListGraph::init_successor(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::init_successor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  (*succ_pred_pointer)[knot-1].x=knot;
}
 
//**************************************************************************

int Lisa_MatrixListGraph::next_successor(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::next_successor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return size+1;
  }
#endif

  int old_knot=(*succ_pred_pointer)[knot-1].x;
  int next_knot=(*matrix)[knot][old_knot].x;
  
  if(next_knot==size+1){
    //START OF AN EDGE LIST
    if(old_knot==knot){
      next_knot=(*matrix)[knot][0].x;
    }else{//END OF AN EDGE LIST
      if(old_knot==(*matrix)[knot][knot].y){
        next_knot=(*matrix)[knot][0].x; 
      }
    }
  }
  
  if(next_knot==size+1){
    init_successor(knot);
  }else{
    (*succ_pred_pointer)[knot-1].x=next_knot;
  }
  
  return next_knot;
}

//**************************************************************************

void Lisa_MatrixListGraph::init_predecessor(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::init_predecessor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif
  
  (*succ_pred_pointer)[knot-1].y=knot;
}

//**************************************************************************

int Lisa_MatrixListGraph::next_predecessor(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::next_predecessor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return size+1;
  }
#endif 
 
  int old_knot=(*succ_pred_pointer)[knot-1].y;
  int next_knot=abs((*matrix)[knot][old_knot].x);
  
  if(next_knot==size+1){
    //START OF AN EDGE LIST
    if(old_knot==knot){
      next_knot=abs((*matrix)[0][knot].x);
    }else{ //END OF AN EDGE LIST
      if(old_knot==(*matrix)[knot][knot].y){
        next_knot=abs((*matrix)[0][knot].x);
      }
    }
  }
  
  if(next_knot==size+1){
    init_predecessor(knot);
  }else{
    (*succ_pred_pointer)[knot-1].y=next_knot;
  }
  
  return next_knot;
}

//**************************************************************************

void Lisa_MatrixListGraph::init_neighbour(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::init_neighbour().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  (*edge_pointer)[knot-1]=knot; 
}

//**************************************************************************

int Lisa_MatrixListGraph::next_neighbour(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::next_neighbour().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return size+1;
  }
#endif 

  int old_knot=(*edge_pointer)[knot-1];
  int next_knot=(*matrix)[knot][old_knot].x;
  
  if(next_knot==size+1){
    //START OF AN EDGE LIST
    if(old_knot==knot){
      next_knot=(*matrix)[knot][0].x;
    }else{//END OF AN EDGE LIST
      if(old_knot==(*matrix)[knot][knot].y){
        next_knot=(*matrix)[knot][0].x; 
      }
    }
  }
  
  if(next_knot==size+1){
    init_neighbour(knot);
  }else{
    (*edge_pointer)[knot-1]=next_knot;
  }
  
  if((next_knot!=size+1)&&(get_connection(knot,next_knot)!=EDGE)){  
    next_knot=size+1;
  } 
  
  return next_knot;
}

//**************************************************************************

int Lisa_MatrixListGraph::get_successors(const int knot)const{
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_Graph::get_successors().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return 0;
  }
#endif 

  int succ_count=0,old_knot=knot,next_knot;
  
  for(;;){
    
    next_knot=(*matrix)[knot][old_knot].x;
    
    if(next_knot==size+1){
      //START OF AN EDGE LIST
      if(old_knot==knot){
        next_knot=(*matrix)[knot][0].x;
      }else{//END OF AN EDGE LIST
        if(old_knot==(*matrix)[knot][knot].y){
          next_knot=(*matrix)[knot][0].x; 
        }
      }
    }
    
    if(next_knot==size+1) break;

    old_knot=next_knot;
    succ_count++;
    
     
  }
  
  return succ_count;
}

//**************************************************************************

int Lisa_MatrixListGraph::get_predecessors(const int knot)const{
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_Graph::get_predecessors().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return 0;
  }
#endif
  
  int pred_count=0,old_knot=knot,next_knot;
  
  for(;;){

    next_knot=abs((*matrix)[knot][old_knot].x);
    
    if(next_knot==size+1){
      //START OF AN EDGE LIST
      if(old_knot==knot){
        next_knot=abs((*matrix)[0][knot].x);
      }else{ //END OF AN EDGE LIST
        if(old_knot==(*matrix)[knot][knot].y){
          next_knot=abs((*matrix)[0][knot].x);
        }
      }
    }
    
    if(next_knot==size+1) break;

    old_knot=next_knot;
    pred_count++;

    
  }

  return pred_count;
}

//**************************************************************************

int Lisa_MatrixListGraph::get_neighbours(const int knot)const{
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_Graph::get_neighbours().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return 0;
  }
#endif
  
  int neigh_count = 0,old_knot= knot,next_knot;
  
  for(;;){
    
    next_knot = (*matrix)[knot][old_knot].x;
    
    if(next_knot==size+1){
      //START OF AN EDGE LIST
      if(old_knot==knot){
        next_knot=(*matrix)[knot][0].x;
      }else{//END OF AN EDGE LIST
        if(old_knot==(*matrix)[knot][knot].y){
          next_knot=(*matrix)[knot][0].x; 
        }
      }
    }
    
    if(next_knot==size+1) break;

    if(get_connection(knot,next_knot)!=EDGE) break;
    
    old_knot=next_knot;
    neigh_count++;
    
    
  }
  
  return neigh_count;
}  

//**************************************************************************

void Lisa_MatrixListGraph::show(){
  
  for (int i=0; i<=size; i++){
    for (int j=0; j<=size; j++){
      cout<<(*matrix)[i][j].x<<"=="<<(*matrix)[i][j].y<<"   ";
    }
    cout<<endl;
  }
  
} 

//**************************************************************************

bool Lisa_MatrixListGraph::valid(){
  
  int el_of_list;
  int* knot_list = new int[size];
  int count=-1;
  
  for(int knot=1; knot<=size; knot++){
    //Ueberpruefen der Nachfolgerliste
    
    //Hinrichtung
    
    el_of_list=(*matrix)[knot][0].x;
    
    if(el_of_list==0){
      delete[] knot_list;
      return false;
    }
    
    if(el_of_list==size+1){	  
      if((*matrix)[knot][0].y!=size+1){
        cout<<" matrix "<<endl;
        delete[] knot_list;
        return false;
      }
    }else{
      if((*matrix)[knot][0].y==size+1){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=size+1){
        count++;
        knot_list[count]=el_of_list;
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }
        
        if(get_connection(el_of_list,knot)!=CRA){
          delete[] knot_list;
          return false;
        }
        
        el_of_list=(*matrix)[knot][el_of_list].x;
      }
      
      //Rueckrichtung
      
      el_of_list=(*matrix)[knot][0].y;
      
      if(el_of_list==0) {
        delete[] knot_list;
        return false;
      }
      
      if((*matrix)[knot][el_of_list].x!=size+1){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=size+1){
        if(count<0){
          delete[] knot_list;
          return false;
        }
        
        if(el_of_list!=knot_list[count]){
          delete[] knot_list;
          return false;
        }
        
        knot_list[count]=0;
        
        count--;
        
        el_of_list=(*matrix)[knot][el_of_list].y;
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }
      }
      count++;
      if(count!=0){
        delete[] knot_list;
        return false;
      }                      
      
    }
    
    count=-1;
    
    //Ueberpruefen der Vorgaengerliste
    
    //Hinrichtung
    
    el_of_list=(*matrix)[0][knot].x;
    
    if(el_of_list==0) {
      delete[] knot_list;  
      return false;
    }
    
    if(el_of_list==(size+1)){
      if((*matrix)[0][knot].y!=(size+1)){
        delete[] knot_list;
        return false;
      }
    }else{
      if((*matrix)[0][knot].y==(size+1)){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=(size+1)){
        count++;
        knot_list[count]=el_of_list;
        
        if(get_connection(el_of_list,knot)!=ARC){
          delete[] knot_list;
          return false;
        }
        
        el_of_list=-((*matrix)[knot][el_of_list].x);
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }	      
      }
      
      //Rueckrichtung
      
      el_of_list=(*matrix)[0][knot].y;
      
      if(el_of_list==0) {
        delete[] knot_list;
        return false;
      }
      
      if((*matrix)[knot][el_of_list].x!=-(size+1)){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=(size+1)){
        
        if(count<0){
          delete[] knot_list;
          return false;
        }
        
        if(el_of_list!=knot_list[count]){
          delete[] knot_list;
          return false;
        }
        
        knot_list[count]=0;
        
        count--;
        
        el_of_list=-((*matrix)[knot][el_of_list].y);
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }
      }
      
      count++;
      if(count!=0){
        delete[] knot_list;
        return false;
      }
    }
    count=-1;
    
    //Ueberpruefen der Kantenliste
    
    //Hinrichtung
    
    el_of_list=(*matrix)[knot][knot].x;
    
    if(el_of_list==0) {
      delete[] knot_list;
      return false;
    }
    
    if(el_of_list==(size+1)){
      if((*matrix)[knot][knot].y!=(size+1)){
        delete[] knot_list;
        return false;
      }
    }else{
      if((*matrix)[knot][knot].y==(size+1)){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=(size+1)){
        count++;
        knot_list[count]=el_of_list;
        
        if(get_connection(el_of_list,knot)!=EDGE){
          delete[] knot_list;
          return false;
        }
        
        el_of_list=(*matrix)[knot][el_of_list].x;
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }      
      }
      
      //Rueckrichtung
      
      el_of_list=(*matrix)[knot][knot].y;
      
      if(el_of_list==0) {
        delete[] knot_list;
        return false;
      }
      
      if((*matrix)[knot][el_of_list].x!=size+1){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=size+1){
        
        if(count<0){
          delete[] knot_list;
          return false;
        }
        
        if(el_of_list!=knot_list[count]){
          delete[] knot_list;
          return false;
        }
        
        knot_list[count]=0;
        
        count--;
        
        el_of_list=(*matrix)[knot][el_of_list].y;
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }
      }
      
      count++;
      if(count!=0){
        delete[] knot_list;
        return false;
      }
      
    }
    count=-1;
  }
  
  delete[] knot_list;  
  return true;
}

//**************************************************************************
//**************************************************************************
//**************************************************************************

Lisa_MatrixGraph::Lisa_MatrixGraph(const int number_of_vertices){
  matrix = 0;
  init(number_of_vertices);
}

//**************************************************************************

Lisa_MatrixGraph::Lisa_MatrixGraph(const Lisa_MatrixGraph *const othergraph){
  matrix = 0;
  init(othergraph->size);
  
  for(int i=1;i<=size;i++){
    for(int j=i+1;j<=size;j++){
      (*matrix)[i][j] = (*othergraph->matrix)[i][j];
      (*matrix)[j][i] = (*othergraph->matrix)[j][i];
    }
  }
}

//**************************************************************************

Lisa_MatrixGraph::Lisa_MatrixGraph(const Lisa_Graph *const othergraph){
  matrix = 0;
  init(othergraph->get_vertices());
  
  Lisa_Matrix<int> out(size,size);
  othergraph->get_adjacency_matrix(&out);
  set_adjacency_matrix(&out);
}

//**************************************************************************

Lisa_MatrixGraph::Lisa_MatrixGraph(const Lisa_MatrixGraph & othergraph){
  matrix = 0;
  init(othergraph.size);
  
  for(int i=1;i<=size;i++){
    for(int j=i+1;j<=size;j++){
      (*matrix)[i][j] = (*othergraph.matrix)[i][j];
      (*matrix)[j][i] = (*othergraph.matrix)[j][i];
    }
  }
}

//**************************************************************************

Lisa_MatrixGraph::Lisa_MatrixGraph(const Lisa_Graph& othergraph){
  matrix = 0;
  init(othergraph.get_vertices());
  
  Lisa_Matrix<int> out(size,size);
  othergraph.get_adjacency_matrix(&out);
  set_adjacency_matrix(&out);
}

//**************************************************************************

Lisa_MatrixGraph::~Lisa_MatrixGraph(){
  if(matrix) delete matrix;
}

//**************************************************************************

void Lisa_MatrixGraph::init(const int number_of_vertex){
  size = number_of_vertex;
  
  if(matrix) delete matrix;
  matrix = new Lisa_Matrix<int>(size+1,size+1);
  
  clear();
}

//**************************************************************************

void Lisa_MatrixGraph::clear(){
  matrix->fill(0);
}

//**************************************************************************

void Lisa_MatrixGraph::get_adjacency_matrix(Lisa_Matrix<int> *const adj) const{
#ifdef LISA_DEBUG
  if(adj->get_n() != size || adj->get_m() != size){
    G_ExceptionList.lthrow("Wrong matrix size in argument to Lisa_MatrixGraph::get_adjacency_matrix().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  for(int i=1;i<=size;i++){
    (*adj)[i-1][i-1] = 0;
    for(int j=i+1;j<=size;j++){
      (*adj)[i-1][j-1] = (*matrix)[i][j];
      (*adj)[j-1][i-1] = (*matrix)[j][i];
    }
  }
}

//**************************************************************************

void Lisa_MatrixGraph::set_adjacency_matrix(const Lisa_Matrix<int> *const adj){
#ifdef LISA_DEBUG
  if(adj->get_n() != size || adj->get_m() != size){
    G_ExceptionList.lthrow("Wrong matrix size in argument to Lisa_MatrixGraph::set_adjacency_matrix().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif  
  
  for(int i=1;i<=size;i++){
    for(int j=i+1;j<=size;j++){
      (*matrix)[i][j] = (*adj)[i-1][j-1] ? 1 : 0;
      (*matrix)[j][i] = (*adj)[j-1][i-1] ? 1 : 0;
    }
  }
}

//**************************************************************************

void Lisa_MatrixGraph::insert_edge(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixGraph::insert_edge().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif
  
  if(start==end) return;
  
  (*matrix)[start][end] = 1;
  (*matrix)[end][start] = 1;
}

//**************************************************************************

void Lisa_MatrixGraph::insert_arc(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixGraph::insert_arc().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif
  
  if(start==end) return;
  
  (*matrix)[start][end] = 1;
}

//**************************************************************************

void Lisa_MatrixGraph::remove_edge(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixGraph::remove_edge().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif
  
  if(start==end) return;
  
  (*matrix)[start][end] = 0;
  (*matrix)[end][start] = 0;
}

//**************************************************************************

void Lisa_MatrixGraph::remove_arc(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixGraph::remove_arc().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif
  
  if(start==end) return;
  
  (*matrix)[start][end] = 0;
}

//**************************************************************************

void Lisa_MatrixGraph::clear(const int vertex){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_MatrixGraph::clear().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  int i=1;
  while(i<vertex){
    (*matrix)[i][vertex] = 0;
    (*matrix)[vertex][i] = 0;
    i++;
  }
  i++;
  while(i<=size){
    (*matrix)[i][vertex] = 0;
    (*matrix)[vertex][i] = 0;
    i++;
  }
  
}

//**************************************************************************

int Lisa_MatrixGraph::get_connection(const int start,const int end)const{
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixGraph::get_connection().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return NONE;
  }
#endif
  
  if(start==end) return NONE;
  
  if((*matrix)[start][end]){
    if((*matrix)[end][start]) return EDGE;
    return ARC;
  }
  
  if((*matrix)[end][start]) return CRA;
  
  return NONE;
}

//**************************************************************************

void Lisa_MatrixGraph::init_successor(const int vertex){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_MatrixGraph::init_successor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  (*matrix)[vertex][0] = 0;
}

//**************************************************************************

int Lisa_MatrixGraph::next_successor(const int vertex){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_MatrixGraph::next_successor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return size+1;
  }
#endif
  
  int currpos = (*matrix)[vertex][0];
  
  do{
    currpos++;
    
    if(currpos == vertex) currpos++;
    
    if(currpos == size+1){
      (*matrix)[vertex][0] = 0;
      return size+1;
    }
  
  }while((*matrix)[vertex][currpos] == 0);
    
  (*matrix)[vertex][0] = currpos;

  return currpos;
}

//**************************************************************************

void Lisa_MatrixGraph::init_predecessor(const int vertex){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_MatrixGraph::init_predecessor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  (*matrix)[0][vertex] = 0;
}

//**************************************************************************

int Lisa_MatrixGraph::next_predecessor(const int vertex){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_MatrixGraph::next_predecessor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return size+1;
  }
#endif

  int currpos = (*matrix)[0][vertex];
  
  do{
    currpos++;
    
    if(currpos == vertex) currpos++;
    
    if(currpos == size+1){
      (*matrix)[0][vertex] = 0;
      return size+1;
    }
  
  }while((*matrix)[currpos][vertex] == 0);
    
  (*matrix)[0][vertex] = currpos;

  return currpos;
}

//**************************************************************************

void Lisa_MatrixGraph::init_neighbour(const int vertex){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_MatrixGraph::init_neighbour().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  (*matrix)[vertex][vertex] = 0; 
}

//**************************************************************************

int Lisa_MatrixGraph::next_neighbour(const int vertex){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_MatrixGraph::next_neighbour().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return size+1;
  }
#endif

  int currpos = (*matrix)[vertex][vertex];
  
  do{
    currpos++;
    
    if(currpos == vertex) currpos++;
    
    if(currpos == size+1){
      (*matrix)[vertex][vertex] = 0;
      return size+1;
    }
  
  }while((*matrix)[currpos][vertex] == 0 || (*matrix)[vertex][currpos] == 0);
    
  (*matrix)[vertex][vertex] = currpos;

  return currpos;
}

//**************************************************************************

int Lisa_MatrixGraph::get_successors(const int vertex)const{
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_MatrixGraph::get_successors().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return 0;
  }
#endif
  int count = 0;
  if((*matrix)[vertex][vertex]) count--;
  
  for(int i=1;i<=size;i++){
    if((*matrix)[vertex][i]) count++;
  }

  return count;
}

//**************************************************************************

int Lisa_MatrixGraph::get_predecessors(const int vertex)const{
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_MatrixGraph::get_predecessors().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return 0;
  }
#endif

  int count = 0;
  if((*matrix)[vertex][vertex]) count--;
  
  for(int i=1;i<=size;i++){
    if((*matrix)[i][vertex]) count++;
  }

  return count;
}

//**************************************************************************

int Lisa_MatrixGraph::get_neighbours(const int vertex)const{
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_MatrixGraph::get_neighbours().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return 0;
  }
#endif

  int count = 0;
  if((*matrix)[vertex][vertex]) count--;
  
  for(int i=1;i<=size;i++){
    if((*matrix)[i][vertex] && (*matrix)[vertex][i]) count++;
  }

  return count;
}
//**************************************************************************
//**************************************************************************
//**************************************************************************

Lisa_WeightedGraph::Lisa_WeightedGraph(const int number_of_vertices){
  adj = 0;
  weights = 0;
  init(number_of_vertices);
}

//**************************************************************************

Lisa_WeightedGraph::Lisa_WeightedGraph(const Lisa_WeightedGraph *const othergraph){
  size = othergraph->size;
  adj = new Lisa_Matrix<int>(*othergraph->adj);
  weights = new Lisa_Matrix<TIMETYP>(*othergraph->weights);
}

//**************************************************************************

Lisa_WeightedGraph::Lisa_WeightedGraph(const Lisa_WeightedGraph& othergraph){
  size = othergraph.size;
  adj = new Lisa_Matrix<int>(*othergraph.adj);
  weights = new Lisa_Matrix<TIMETYP>(*othergraph.weights);  
}

//**************************************************************************

Lisa_WeightedGraph::~Lisa_WeightedGraph(){
  if(adj) delete adj;
  if(weights) delete weights;
}

//**************************************************************************

void Lisa_WeightedGraph::init(const int number_of_vertex){
  size = number_of_vertex;
  
  if(adj) delete adj;
  if(weights) delete weights;
  
  adj = new Lisa_Matrix<int>(size+1,size+1);
  weights = new Lisa_Matrix<TIMETYP>(size+1,size+1);
  
  clear();
}

//**************************************************************************

void Lisa_WeightedGraph::clear(){
  adj->fill(0);
  weights->fill(0);
}

//**************************************************************************

void Lisa_WeightedGraph::get_adjacency_matrix(Lisa_Matrix<int> *const adj)const{
#ifdef LISA_DEBUG
  if(adj->get_n() != size || adj->get_m() != size){
    G_ExceptionList.lthrow("Wrong matrix size in argument to Lisa_WeightedGraph::get_adjacency_matrix().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  for(int i=1;i<=size;i++){
    (*adj)[i-1][i-1] = 0;
    for(int j=i+1;j<=size;j++){
      (*adj)[i-1][j-1] = (*this->adj)[i][j];
      (*adj)[j-1][i-1] = (*this->adj)[j][i];
    }
  }
}

//**************************************************************************

void Lisa_WeightedGraph::set_adjacency_matrix(const Lisa_Matrix<int> *const adj){
#ifdef LISA_DEBUG
  if(adj->get_n() != size || adj->get_m() != size){
    G_ExceptionList.lthrow("Wrong matrix size in argument to Lisa_WeightedGraph::set_adjacency_matrix().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif  
  
  for(int i=1;i<=size;i++){
    for(int j=i+1;j<=size;j++){
      (*this->adj)[i][j] = (*adj)[i-1][j-1] ? 1 : 0;
      (*this->adj)[j][i] = (*adj)[j-1][i-1] ? 1 : 0;
    }
  }
  
  weights->fill(0);
}

//**************************************************************************

void Lisa_WeightedGraph::get_weight_matrix(Lisa_Matrix<TIMETYP> *const weights)const{
#ifdef LISA_DEBUG
  if(weights->get_n() != size || weights->get_m() != size){
    G_ExceptionList.lthrow("Wrong matrix size in argument to Lisa_WeightedGraph::get_weight_matrix().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  for(int i=1;i<=size;i++){
    (*weights)[i-1][i-1] = 0;
    for(int j=i+1;j<=size;j++){
      (*weights)[i-1][j-1] = (*this->weights)[i][j];
      (*weights)[j-1][i-1] = (*this->weights)[j][i];
    }
  }
}

//**************************************************************************

void Lisa_WeightedGraph::set_weight_matrix(const Lisa_Matrix<TIMETYP> *const weights){
#ifdef LISA_DEBUG
  if(weights->get_n() != size || weights->get_m() != size){
    G_ExceptionList.lthrow("Wrong matrix size in argument to Lisa_WeightedGraph::set_weight_matrix().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif  
  
  for(int i=1;i<=size;i++){
    for(int j=i+1;j<=size;j++){
      (*this->weights)[i][j] = (*adj)[i][j] ? (*weights)[i-1][j-1] : 0;
      (*this->weights)[j][i] = (*adj)[j][i] ? (*weights)[j-1][i-1] : 0;
    }
  }
}

//**************************************************************************

void Lisa_WeightedGraph::insert_arc(const int start,const int end,const TIMETYP weight){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_WeightedGraph::insert_arc().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif
  
  if(start==end) return;
  
  (*adj)[start][end] = 1;
  (*weights)[start][end] = weight;
}

//**************************************************************************

void Lisa_WeightedGraph::remove_arc(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_WeightedGraph::remove_arc().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif
  
  if(start==end) return;
  
  (*adj)[start][end] = 0;
  (*weights)[start][end] = 0;
}

//**************************************************************************

void Lisa_WeightedGraph::clear(const int vertex){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_WeightedGraph::clear().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  for(int i=1;i<=size;i++){
    (*adj)[i][vertex] = 0;
    (*adj)[vertex][i] = 0; 
    (*weights)[i][vertex] = 0;
    (*weights)[vertex][i] = 0; 
  }
}

//**************************************************************************

int Lisa_WeightedGraph::get_connection(const int start,const int end, TIMETYP *const weight)const{
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_WeightedGraph::get_connection().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return NONE;
  }
#endif

  if(start==end) return NONE;
  
  if((*adj)[start][end]){
    *weight = (*weights)[start][end];
    return ARC;
  }
  
  if((*adj)[end][start]){
    *weight = (*weights)[end][start];
    return CRA;
  }
  
  return NONE;
}

//**************************************************************************

void Lisa_WeightedGraph::init_successor(const int vertex){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_WeightedGraph::init_successor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  (*adj)[vertex][0] = 0;
}

//**************************************************************************

int Lisa_WeightedGraph::next_successor(const int vertex,TIMETYP *const weight){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_WeightedGraph::next_successor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return size+1;
  }
#endif
  
  int currpos = (*adj)[vertex][0];
  
  do{
    currpos++;
    
    if(currpos == size+1){
      (*adj)[vertex][0] = 0;
      return size+1;
    }
  
  }while((*adj)[vertex][currpos] == 0);
    
  (*adj)[vertex][0] = currpos;

  *weight = (*weights)[vertex][currpos];
  return currpos;  
}

//**************************************************************************

void Lisa_WeightedGraph::init_predecessor(const int vertex){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_WeightedGraph::init_predecessor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  (*adj)[0][vertex] = 0;
}

//**************************************************************************

int Lisa_WeightedGraph::next_predecessor(const int vertex,TIMETYP *const weight){
#ifdef LISA_DEBUG
  if( vertex<=0 || vertex>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(vertex)+
                           " out of range in Lisa_WeightedGraph::next_predecessor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return size+1;
  }
#endif

  int currpos = (*adj)[0][vertex];
  
  do{
    currpos++;
    
    if(currpos == size+1){
      (*adj)[0][vertex] = 0;
      return size+1;
    }
  
  }while((*adj)[currpos][vertex] == 0);
    
  (*adj)[0][vertex] = currpos;

  *weight = (*weights)[currpos][vertex];
  return currpos;
}

//**************************************************************************

void Lisa_WeightedGraph::write(std::ostream& strm)const{
  
  if(strm){
    G_ExceptionList.lthrow("No valid stream in Lisa_WeightedGraph::write().",
                           Lisa_ExceptionList::ANY_ERROR);
    return;
  }
  
  Lisa_Matrix<int> out(size,size);
  get_adjacency_matrix(&out);
  
  Lisa_Matrix<TIMETYP> wout(size,size);
  get_weight_matrix(&wout);

  strm << endl << "<WEIGHTEDGRAPH>" << endl;
  strm << "vertices= "<< get_vertices() << endl;
  strm << "adjacency_matrix= " << out << endl;
  strm << "weight_matrix= " << wout << endl;
  strm << "</WEIGHTEDGRAPH>" <<endl;
}

//**************************************************************************

void Lisa_WeightedGraph::read(std::istream& strm){
  
  if(strm){
    G_ExceptionList.lthrow("No valid stream in Lisa_WeightedGraph::read().",
                           Lisa_ExceptionList::ANY_ERROR);
    return;
  }
  
  string S;
  int new_size;
  Lisa_Matrix<int>* adj_matrix=0;
  Lisa_Matrix<TIMETYP>* w_matrix=0;
  for (;;){
    S=""; 
    strm >> S;
    if (S==""){ 
      G_ExceptionList.lthrow((string) "Unexpected end of file while looking for starttag <WEIGHTEDGRAPH>.",
                             Lisa_ExceptionList::END_OF_FILE);
      return;
    } 
    if (S=="<WEIGHTEDGRAPH>") break;
  }
  
  S=""; 
  strm >> S; 
  if (S=="vertices=") {  
    strm >> new_size; 
    init(new_size);
    adj_matrix=new Lisa_Matrix<int>(new_size,new_size);
    w_matrix = new Lisa_Matrix<TIMETYP>(new_size,new_size);
  }else{
    G_ExceptionList.lthrow("'vertices=' expected in <WEIGHTEDGRAPH>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  S=""; 
  strm >> S; 
  if (S=="adjacency_matrix="){  
    strm >> *adj_matrix;
    set_adjacency_matrix(adj_matrix);
    if(adj_matrix) delete adj_matrix;
  }else{
    G_ExceptionList.lthrow("'adjacency_matrix=' expected in <WEIGHTEDGRAPH>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  S=""; 
  strm >> S; 
  if (S=="weight_matrix="){  
    strm >> *w_matrix;
    set_weight_matrix(w_matrix);
    if(w_matrix) delete w_matrix;
  }else{
    G_ExceptionList.lthrow("'weight_matrix=' expected in <WEIGHTEDGRAPH>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  S=""; 
  strm >> S; 
  if (S!="</WEIGHTEDGRAPH>"){
    G_ExceptionList.lthrow("'</WEIGHTEDGRAPH>' expected in <WEIGHTEDGRAPH>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;  
  } 
}

//**************************************************************************
//**************************************************************************
//**************************************************************************

Lisa_HammingGraphIndex::Lisa_HammingGraphIndex(const Lisa_Matrix<bool> *const SIJ)
                                              :m(SIJ->get_m()),n(SIJ->get_n()){
  
  indices = 0;
  ij2index = new int[m*n];
  
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      ij2index[i*m+j] = ((*SIJ)[i][j]) ? ++indices : 0;
    }
  }
  
  index2i = new int[indices+1];
  index2j = new int[indices+1];

  index2i[0] = index2j[0] = 0;
  
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      if(ij2index[i*m+j] == 0){
        ij2index[i*m+j] = indices+1;
      }else{
        index2i[ij2index[i*m+j]] = i;
        index2j[ij2index[i*m+j]] = j;
      }
    }
  }
 
}

//**************************************************************************

Lisa_HammingGraphIndex::~Lisa_HammingGraphIndex(){
  delete[] index2j;
  delete[] index2i;
  delete[] ij2index;
}

//**************************************************************************

void Lisa_HammingGraphIndex::write(ostream& strm)const{
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      strm.width(4);
      strm << ij2index[i*m+j];
    }
    strm << endl;
  }
  strm << endl;

  for(int i=0;i<indices+1;i++){
    strm << "(";
    strm.width(4);
    strm << index2i[i] << ",";
    strm.width(4);
    strm << index2j[i] << ") ";
  }
  strm << endl;
} 

//**************************************************************************
//**************************************************************************
//**************************************************************************

bool Lisa_GraphAlg::topsort(const Lisa_Graph *const g,
                            Lisa_Vector<int> *const knot_sequence){
#ifdef LISA_DEBUG
  if(knot_sequence->get_m() != g->get_vertices()){
    G_ExceptionList.lthrow("Argument vector size incorrect in Lisa_GraphAlgo::topsort().",
                           Lisa_ExceptionList::ANY_ERROR);
    return false;
  }
#endif
  
  Lisa_MatrixGraph top(g);

  knot_sequence->fill(0);
  
  int next=1,v=1;
  while(v<=top.get_vertices()){
    
    if(((*knot_sequence)[v-1]==0)&&(top.get_predecessors(v)==0)){
      top.clear(v);
      (*knot_sequence)[v-1]=next;
      next++;
      v=0;
    }
    
    v++;
  }
  
  return (next==top.get_vertices()+1);
}

//**************************************************************************

bool Lisa_GraphAlg::topsort_inverse(const Lisa_Graph *const g,
                                    Lisa_Vector<int> *const knot_sequence){
#ifdef LISA_DEBUG
  if(knot_sequence->get_m() != g->get_vertices()){
    G_ExceptionList.lthrow("Argument vector size incorrect in Lisa_GraphAlgo::topsort_inverse().",
                           Lisa_ExceptionList::ANY_ERROR);
    return false;
  }
#endif

  const int vert = g->get_vertices();
  
  Lisa_Vector<int> sort(vert);
  
  if(topsort(g,&sort)){
    for (int i=0;i<vert;i++)  (*knot_sequence)[sort[i]-1]=i+1;
    return true;
  }
  
  return false;
}

//**************************************************************************

void Lisa_GraphAlg::build_semigraph(Lisa_Graph *const graph){
  const int vert = graph->get_vertices();
  int c;
  
  for (int i=1;i<=vert;i++){
    for (int j=i+1;j<=vert;j++){
      c = graph->get_connection(i,j);
      if(c!=Lisa_Graph::NONE&&c!=Lisa_Graph::EDGE){ 
        if (c==Lisa_Graph::ARC) graph->insert_arc(j,i);
        else graph->insert_arc(i,j);
      }
    }
  }
}

//************************************************************************** 

void Lisa_GraphAlg::build_compgraph(Lisa_Graph *const source,Lisa_Graph *const target){
#ifdef LISA_DEBUG
  if(source->get_vertices() != target->get_vertices()){
    G_ExceptionList.lthrow("Size mismatch in Lisa_GraphAlgo::build_compgraph().",
                           Lisa_ExceptionList::ANY_ERROR);
    return;
  }
#endif
  
  const int vert = source->get_vertices();
  
  Lisa_Vector<int> queue(vert+1);
  Lisa_Vector<bool> done(vert+1);
  int qs,qe,curr;
  int succ;
  
  for (int i=1;i<=vert;i++){
    //queue.fill(0);  
    done.fill(0);
    qs = qe = 0;
    
    source->init_successor(i);
    succ=source->next_successor(i);
    while((succ<vert+1)){
      queue[qe++] = succ;
      done[succ] = 1;
      succ=source->next_successor(i);
    }  
    
    while(qs<qe){
      curr = queue[qs++];
      target->insert_edge(i,curr);
      source->init_successor(curr);
      succ=source->next_successor(curr);
      while((succ<vert+1)){
        if(!done[succ]){
          done[succ] = 1;
          queue[qe++] = succ;
        }
        succ = source->next_successor(curr);
      }
    } 
  }  
}

//**************************************************************************

void Lisa_GraphAlg::transitive_hull(Lisa_Graph *const source,Lisa_Graph *const target){
#ifdef LISA_DEBUG
  if(source->get_vertices() != target->get_vertices()){
    G_ExceptionList.lthrow("Size mismatch in Lisa_GraphAlgo::transitive_hull().",
                           Lisa_ExceptionList::ANY_ERROR);
    return;
  }
#endif
  
  const int end = source->get_vertices()+1;
  
  Lisa_Vector<int> queue(end);
  Lisa_Vector<bool> done(end);
  int qs,qe,curr;
  int succ;
  
  for (int i=1; i<end; i++){
    //queue.fill(0);  
    done.fill(0);
    qs = qe = 0;
    
    source->init_successor(i);
    succ=source->next_successor(i);
        
    while(succ<end){
      queue[qe++] = succ;
      done[succ] = 1;
      succ=source->next_successor(i);
    }  
    
    while(qs<qe){
      curr = queue[qs++];
      target->insert_arc(i,curr);
      source->init_successor(curr);
      succ=source->next_successor(curr);
      while(succ<end){
        if(!done[succ]){
          done[succ]=1;
          queue[qe++]=succ;
        }
        succ=source->next_successor(curr);
      }
    }
  }

}

//**************************************************************************

bool Lisa_GraphAlg::smaller(const Lisa_Graph *const first,const Lisa_Graph *const second){
#ifdef LISA_DEBUG
  if(first->get_vertices() != second->get_vertices()){
    G_ExceptionList.lthrow("Size mismatch in Lisa_GraphAlgo::smaller().",
                           Lisa_ExceptionList::ANY_ERROR);
    return false;
  }
#endif

  const int vert = first->get_vertices();
  int fc,sc;
  bool missing=0;
  
  for (int i=1;i<=vert;i++){
    for (int j=i+1;j<=vert;j++){
      fc = first->get_connection(i,j);
      sc = second->get_connection(i,j);
      if(fc!=sc){
        if (fc == Lisa_Graph::EDGE) return 0;
        else missing = 1;
      }
    }
  }
  
  return missing;
}

//************************************************************************** 

bool Lisa_GraphAlg::equal(const Lisa_Graph *const first,const Lisa_Graph *const second){
#ifdef LISA_DEBUG
  if(first->get_vertices() != second->get_vertices()){
    G_ExceptionList.lthrow("Size mismatch in Lisa_GraphAlgo::equal().",
                           Lisa_ExceptionList::ANY_ERROR);
    return false;
  }
#endif

  const int vert = first->get_vertices();
  
  for (int i=1;i<=vert;i++){
    for (int j=i+1;j<=vert;j++){
      if(first->get_connection(i,j) != second->get_connection(i,j)) return 0;
    }
  }
  
  return 1;
}

//**************************************************************************    
  
