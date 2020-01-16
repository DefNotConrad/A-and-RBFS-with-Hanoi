//
//  RBFS.cpp
//  aStar
//
//  Created by Britain Mackenzie on 10/29/19.
//  Copyright © 2019 Britain Mackenzie. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <math.h>
#include <sys/time.h>


/*_______________ACTION DEF_______________*/

struct Action{                  //An action is a move array [src,trgt]
    int mv[2];
};

/*_______________TREE NODE DEF_______________*/

//node of search tree/solution tree
struct Node{
    uint name;                  //Unique identifier for node to find if its already on open/closed
    int hn;                     //heuristic value passed
    int gn;                     //gn, is gn++ of parent
    int fn;                     //gn+hn, value to determine if is best next move
    int move[2];                //holds move needed from parent to get to this node, holds last move when passed as parent node
    int correct;
    Node* parent;               //Pointer to parent node
    std::vector<Node*> children;   //successors to a node
    Node* bestChild;
    Node* alternative;
    std::vector <int> tower1;  //vector that holds current disks on first tower and their values
    std::vector <int> tower2;  //vector that holds current disks on second tower and their values
    std::vector <int> tower3;
    
};


/*_______________GLOBAL VARS_______________*/
const int POSS_MVS = 6;     //Six possible moves
const std::vector<Action> posActions;
int n;                      //the number of discs
int totalMoves = 0;         //total moves used
bool goal =false;           //false on first move
bool firstMove = true;      //true on first used
int goalPeg = 2;            //the peg that is not used for the goal
uint nodeName=0;            //node creation count
Node * goalNode;             //node pointer for goal node when found
Node * bestChild;
int fLimit;
int generated = 0;
int expanded = 0;


/*_______________FUNCTION DEFS_______________*/

/*get number of disks user requests for this run */
void getN(){
    n=0;
    while(n<=0){
        std::cout<<"Please enter number of disks: ";
        std::cin>>n;
        if(n<=0){
            std::cout<<"\nInvalid input. Needs to be greater than 0.\n";
        }
    }
    fLimit = pow(2,n)+1;                   //initialize to the optimal set of moves, with admissable h(n) should not reach
}

/*function to initialize tower1 and disks to number requested*/
void initTower(Node* node){
    //init the tower with larges on bottom (n), smallest on top (0)
    for(unsigned i = 0; i < unsigned(n); i++){
        node->tower1.push_back(n - i);
    }
    
}

/*get the number of correctly placed discs on the goal tower*/
void getCorrect(Node* node){
    if(!(node->tower2.empty())){
        if((node->tower2.at(0))==(n-1)){
            node->correct = 1;
            
            if(node->tower2.size()>1){
                for(int i = 0; i<((int)(node->tower2.size())-1);i++){
                    //check if disk is one larger than one above by 1
                    if((node->tower2.at(i))-1==(node->tower2.at(i+1))){
                        node->correct++;
                    }
                }
            }
        }else{
            node->correct=0;
        }
    }else{
        node->correct=0;
    }
}

/* Heuristic function that recieves pointer to node to process, initializes hn of the node*/
void getH (Node* node){
    //determine if goal node
    if(node->tower2.size() == (unsigned)n){
        node->hn=-1;
        goal=true;
        goalNode=node;                  // set goalNode as Node
    }else{
        //heuristic function to determine value
        
#ifdef BRITAIN
        
        /*
         if (n-1 on goal ) + n-1
         if (disk(n) is not on goal){ n + (# on first tower)      (+ # on goal )(maybe)
         else (n-correct on goal) + # stacked (size - 1) + (correct on goal-size.goal())
         
         may possibly add if the disks aren't stacked in proper order hn+1 for each tower as one smaller is optimal
         */
        int hn = 0;
        int correct = 0;
        
        //if goal nod has n-1 disk
        if(!(node->tower2.empty())){
            if((node->tower2.at(0))==n-1){
                hn+=(n-1);
            }
            //if largest not first on goal
            if((node->tower2.at(0))!=n){
                //number of disks
                hn+=n;
                //number of disks still on tower 1
                hn+=(int)(node->tower1.size());
            }else{
                if(node->tower2.size()>1){
                    hn+=(n-node->correct);
                }
                //plus the number stacked
                hn+=(int)(node->tower1.size())-1;
                hn+=(int)(node->tower3.size())-1;
                
                hn+=(node->correct-(int)(node->tower2.size()));
                
            }
        }else{
            //number of disks
            hn+=n;
            //number of disks still on tower 1
            hn+=(int)(node->tower1.size());
        }
        node->hn = hn;
        
#endif
    }
}

//Generate a unique name for hash since it will take less time for large n
void getName(Node * node){
    int hash = 0;
    int size1 = (int)node->tower1.size();
    int size2 = (int)node->tower2.size();
    int size3 = (int)node->tower3.size();
    int w1 =0;
    int w2 =0;
    int w3 =0;
    if(size1 > 0){
        for(unsigned i = 0; i < node->tower1.size(); i++){
            w1 += node->tower1.at(i);
        }
    }
    hash += (100010000 * (w1));
    
    if(size2 > 0){
        for(unsigned i = 0; i < node->tower2.size(); i++){
            w2 += node->tower2.at(i);
        }
    }
    hash += (10000 * (w2));
    
    if(size3 > 0){
        for(unsigned i = 0; i < node->tower3.size(); i++){
            w3 += node->tower3.at(i);
        }
    }
    hash += (1 * (w3));
    if(w2 > w1 + w2)
        hash *= -1;
    
    node->name = hash;
}

//create all legal next moves as child nodes with pointer to parent and add too open list
//Step 2. a, b
Node* getChild(Node * parent, int src, int trg){
    int x = 0;
    int y = 0;
    
    generated++; //since child is generated, add to counter.
    Node* node = new Node;
    
    //fn and hn ar below tower init and move
    node->gn=((parent->gn)+1);        //gn is one step from parent == 1 step from root
    
    node->move[0]=src;          //set from side of move
    node->move[1]=trg;          //set to side of move
    
    node->parent=parent;            //set pointer to parent node
    
    node->bestChild=NULL;
    node->alternative=NULL;
    
    //save parent state of game to node
    node->tower1=parent->tower1;
    node->tower2=parent->tower2;
    node->tower3=parent->tower3;
    x = src;
    y = trg;
    
    //make the move for this node
    switch(x){
        case 1:
            switch(y){
                case 2:
                    node->tower2.push_back((node->tower1).back());
                    node->tower1.pop_back();
                    break;
                case 3:
                    node->tower3.push_back((node->tower1).back());
                    node->tower1.pop_back();
                    break;
                default:
                    std::cout<<"Error within to move in the findMove funct";
                    break;
            }
            break;
        case 2:
            switch(y){
                case 1:
                    node->tower1.push_back(node->tower2.back());
                    node->tower2.pop_back();
                    break;
                case 3:
                    node->tower3.push_back(node->tower2.back());
                    node->tower2.pop_back();
                    break;
                default:
                    std::cout<<"Error within to move in the findMove funct";
                    break;
            }
            break;
        case 3:
            switch(y){
                case 1:
                    node->tower1.push_back((node->tower3).back());
                    node->tower3.pop_back();
                    break;
                case 2:
                    node->tower2.push_back(node->tower3.back());
                    node->tower3.pop_back();
                    break;
                default:
                    std::cout<<"Error within to move in the findMove funct";
                    break;
            }
            break;
        default:
            std::cout<<"Error within from move in the findMove funct";
            break;
    }
    
    getCorrect(node);
    getH(node);                //set hn through heuristic function after move has been made
    getName(node);
    
    if(node->hn == -1){
        node->fn=-1;
    }else{
        node->fn=(node->gn)+(node->hn);     //set fn after hn is set
    }
    return node;
}

/* Determines best child and sorts the children in order of lowest f(n) */
void sortChildren(Node *parent, int i){
    while(i>0){
        if(parent->bestChild == NULL){                              //if first child/only child set as best
            parent->bestChild = parent->children[i-1];
            //}else if(parent->children[i-1]->fn == parent->bestChild->fn){
            //    if(parent->children[i-1]->tower1.size() < parent->bestChild->tower1.size()){
            //           parent->bestChild = parent->children[i-1];
            //    }
        }else if(parent->children[i-1]->fn < parent->bestChild->fn){        //if current child is better than best child
            parent->alternative = parent->bestChild;
            parent->bestChild = parent->children[i-1];
        }else if(parent->alternative == NULL){                      //if alternative not init and current>best
            parent->alternative = parent->children[i-1];
        }else if(parent->children[i-1]->fn < parent->alternative->fn){//if not best and better than alt
            parent->alternative = parent->children[i-1];
        }
        if(parent->alternative!=NULL && parent->bestChild->fn == parent->alternative->fn){
            if(parent->bestChild->tower1.size() > parent->alternative->tower1.size()){
                if(parent->children[0] == parent->alternative){
                    parent->alternative = parent->bestChild;
                    parent->bestChild = parent->children[0];
                }else{
                    parent->alternative = parent->bestChild;
                    parent->bestChild = parent->children[1];
                }
            }else if(parent->bestChild == parent->alternative){
                parent->alternative=NULL;
            }
        }
        //else not best or alt
        i--;
    }
    if(parent->bestChild==NULL && parent->children.size() !=0){
        std::cout<<"Error in sort children.";
    }
}
/*determine next legal moves and add to nextMoves array
 
 generates successors and does step 2.
 */
void findMove(Node* parent){
    expanded++;                 //consider node expanded.
    std::vector<Action> legal;  //legal moves
    Node* ch;
    Action action;
    int ch_index = -1;          //gets the index of the child node on open if found
    int A,B,C;                  //hold the towers top piece value or 0 if empty
    A = 0;
    B = 0;
    C = 0;
    
    if(!(parent->tower1.empty()))             //A:Empty
        A = ((parent->tower1).back());         //A<- the value of top disk tower1
    
    //Tower 2 top peiece or empty
    if(!(parent->tower2.empty()))             //B:Empty
        B = ((parent->tower2).back());         //B<- the value of top disk tower2
    
    //Tower 3 top peice or empty
    if(!(parent->tower3.empty()))             //C:Empty
        C = ((parent->tower3).back());         //C<- the value of top disk tower3
    
    if (firstMove==true){ //First move is simple to choose, like in tic-tac-toe
        firstMove=false;
        if(n%2 ==1){
            ch = getChild(parent, 1, 2);
            parent->children.push_back(ch);
        }else{
            ch = getChild(parent, 1, 3);
            parent->children.push_back(ch);
        }
    }else{
        
        
        //Get legal moves based on tower tops
        
        if(A>0 && (A < B || B == 0)){            //If A is non-empty, and A<B then move 1->2 is legal
            action.mv[0]=1;
            action.mv[1]=2;
            legal.push_back(action);
        }else if(B!=0){//(B < A)         //If both non-empty, and B<A then move 2->1 is legal
            if((int)(parent->tower2.size())!= parent->correct){
                action.mv[0]=2;
                action.mv[1]=1;
                legal.push_back(action);
            }
        }
        
        if(A>0 && (A < C || C == 0)){             //If A is non-empty, and A<C then move 1->3 is legal
            action.mv[0]=1;
            action.mv[1]=3;
            legal.push_back(action);
        }else if(C!=0){//(C > A)         //If both  non-empty, and C>A then move 3->1 is legal
            action.mv[0]=3;
            action.mv[1]=1;
            legal.push_back(action);
        }
        
        if(B>0 && (B < C || C == 0)){             //If A is non-empty, and A<C then move 1->3 is legal
            if((int)parent->tower2.size() != parent->correct){
                action.mv[0]=2;
                action.mv[1]=3;
                legal.push_back(action);
            }
        }else if(C!=0){//(C > B)         //If both  non-empty, and C>A then move 3->1 is legal
            action.mv[0]=3;
            action.mv[1]=2;
            legal.push_back(action);
        }
        
        //generate children, using getChild, where their parent link is set to parent (BESTNODE)
        //  and children are only added to the parent's successors after seeing if they
        //  have already been generated as per step 2.
        
        while (!legal.empty()) {  //if move is backtrack, dont consider it
            if(parent->move[1] == legal.back().mv[0]){
                legal.pop_back();
                continue;
            }
            
            //else generate child from legal move
            ch = getChild(parent, legal.back().mv[0],legal.back().mv[1]);
            parent->children.push_back(ch);
            legal.pop_back();
        }
        
    }
}

/*
 The Recursive Best-First Saerch (rbfs) starts from the root. Produces the children, determines their f(n). The child with the lowest f(n) is the next root node (recursively), the root node f(n) is turned into the child's f(n) and holds a pointer to the next best child node and its parent node. If at any point the child node's f(n)s are larger than the next best node of the parent the lowest child f(n) is stored in the parent's f(n) and the new best node is expanded.
 */
int rbfs(Node* current, int fLimit){
    
    if(goal){                               //if node is goal
        return -1;                          //return goal state
    }
    
    if(current->children.empty()){
        findMove(current);                  //generate children and best child
    }
    
    if(current->children.empty()){          //if failure (no children)
        return INT_MAX;
    }
    
    if(current->children.size() == 1){                                  //if only one child is best
        current->bestChild = current->children[0];
    }else{
        sortChildren(current, (int)(current->children.size()));         //best<- the lowest f-value in successors
    }
    
    while(current->bestChild->fn <= fLimit && current->bestChild->fn != INT_MAX && !goal){
        if(current->alternative != NULL && current->alternative->fn < fLimit){                  //if next best node is better than fLimit
            current->bestChild->fn = rbfs(current->bestChild, (current->alternative)->fn);
        }else{                                                                                  //if fLimit is better
            current->bestChild->fn = rbfs(current->bestChild, fLimit);
        }
        
        if(current->children.size() == 1){                                                      //if only one child is best
            current->bestChild = current->children[0];
        }else{
            sortChildren(current, (int)(current->children.size()));                             //best<- the lowest f-value in successors
        }
    }
    current->fn = current->bestChild->fn;                                                       //store best child return value
    
    return current->fn;
}


/*_______________MAIN_______________*/

int main(int argc, char* argv[]) {
    
    if(argc != 2) {
        //std::cerr << "\nUSAGE: " << argv[0] << " num_disks\n";
        //std::cout << "\ngot : " << argv[1] << "\n";
        getN();
    }
    else
        n = atoi(argv[1]);  //convert first argument to int n.
    
    timeval start, end;         //For tracking time.
    double elapsed_time;        //Elapsed time is calculated after game is successful.
    std::string out_path = "./rbfs_output.csv";
    std::vector <int> goalPath;
    std::ofstream fout;
    int i=0;
    int finalPath=0;
    Node starting;
    
    starting.name = 0; //Appropriate hash value for starting node
    starting.hn=0;
    starting.gn=0;
    starting.fn=0;
    starting.correct=0;
    starting.move[0]=0;
    starting.move[1]=0;
    starting.parent=NULL;
    starting.bestChild=NULL;
    starting.alternative=NULL;
    
    initTower(&starting);
    getH(&starting);
    getName(&starting);
    
    gettimeofday(&start, NULL);           // start time after setup.
    
    
    //the aStar loop that pulls from open list and then generates next moves. returns so that when goal is found loop may exit
    while(!goal){
        rbfs(&starting, INT_MAX);
    }
    
    gettimeofday(&end, NULL);           // end time immediately after complete
    
    
    //backtrack through parents for shortest path
    while(goalNode->parent!=NULL){
        goalPath.push_back(goalNode->move[1]);
        goalPath.push_back(goalNode->move[0]);
        goalNode=(goalNode->parent);
    }
    
    //display shortest path
    i=(int)(goalPath.size())-1;
    while(i>0){
        finalPath++;
        //std::cout<<finalPath<<" : From "<<goalPath.back();
        std::cout<<(char)(goalPath.back() + 64);
        goalPath.pop_back();
        //std::cout<<" To "<<goalPath.back()<<std::endl;
        std::cout<<(char)(goalPath.back() + 64) << " ";
        goalPath.pop_back();
        i=i-2;
    }
    elapsed_time = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / 1000000.0;
    fout.open(out_path, std::ofstream::out | std::ofstream::app);
    
#ifdef BRITAIN
    fout << "a* , h2 ," << n << " , " << elapsed_time << " , " << generated << " , "<< expanded << "\n";
#endif
    fout.close();
    return 0;
}

