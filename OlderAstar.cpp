//
//  main.cpp
//  aStar
//
//  Created by Britain Mackenzie on 10/29/19.
//  Copyright © 2019 Britain Mackenzie. All rights reserved.
//

#include <iostream>
#include <vector>

//#define ANDREW
//#define BRITAIN

/*_______________TREE NODE DEF_______________*/

//node of search tree/solution tree
struct Node{
    uint name;
    bool goal = false;
    int hn;                     //heuristic value passed
    int gn;                     //gn, is gn++ of parent
    int fn;                     //gn+hn, value to determine if is best next move
    int move[2];                //holds move needed from parent to get to this node, holds last move when passed as parent node

    Node* parent;               //Ponter to parent node

    std::vector <int*> tower1;  //vector that holds current disks on first tower and their values
    std::vector <int*> tower2;  //vector that holds current disks on second tower and their values
    std::vector <int*> tower3;  //vector that holds current disks on third tower and their values
};

/*_______________GLOBAL VARS_______________*/

int n;                      //the number of discs
int totalMoves = 0;         //total moves used
bool goal =false;           //false on first move
bool firstMove = true;      //true on first used
int goalPeg = 2;         //the peg that is not used for the goal
uint nodeName=0;

std::vector <int> disk;     //the array of disks to be pointed to by the tower vectors

std::vector <Node*> open;   //open list
std::vector <Node*> closed; //closed list, back holds most recent node processed



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
}

/*function to initialize tower1 and disks to number requested*/
void initTower(Node* node){

    //init the disks where index==value
    for(int i=0; i<n; i++){
        disk.push_back(i);
    }

    //init the tower with larges on bottom (n), smallest on top (0)
    for(int i = n-1; i>=0;i--){
        node->tower1.push_back(&disk[i]);
    }
}


/* Heuristic function that recieves pointer to node to process, initializes hn of the node*/
void getH (Node* node){
#ifdef BRITAIN

#endif

    //#define HEUR

    //determine if goal node
    if(node->tower1.empty() && node->tower3.empty()){
        node->hn= -1;
        node->goal=true;
        goal=true;
    }else{
        //heuristic function to determine value
#ifdef ANDREW
        /*
         h(n) = x + 2*y
         x = number of disks not on the goal
         2*y = 2* number of disks on the goal which are smaller than a disk on another tower
         */
        int h1=0;
        int h2=0;
        h1 = (n - node->tower2.size()); //number of disks that aren't on goal.

        for(unsigned j = 0; j < node->tower2.size(); j++){//For all elements on goal
          if(*(node->tower2.at(j)) < int(n - j)){            //If element is smaller than it should be.
            h2 = node->tower2.size() - j;                 //h2 is the number of element that are incorrect.
            break;
          }
        }

        node->hn = h1 + ( 2 * h2);
#endif
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
                   z
                    hn+=(n-correct);
                }
                //plus the number stacked
                hn+=(int)(node->tower1.size())-1;
                hn+=(int)(node->tower3.size())-1;
                
                hn+=(correct-(int)(node->tower2.size()));

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


//create all legal next moves as child nodes with pointer to parent and add too open list
Node* getChild(Node * parent, int move[]){
    int x = 0;
    int y = 0;

    Node* node = new Node;

    node->name=nodeName++;          //set node name from global var to make sure no repeats
                                    //fn and hn ar below tower init and move
    node->gn=((parent->gn)+1);        //gn is one step from parent == 1 step from root

    node->move[0]=move[0];          //set from side of move
    node->move[1]=move[1];          //set to side of move

    node->parent=parent;            //set pointer to parent node


    //save parent state of game to node
    node->tower1=parent->tower1;
    node->tower2=parent->tower2;
    node->tower3=parent->tower3;

    x = move[0];
    y = move[1];

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

    getH(node);                //set hn through heuristic function after move has been made
    if(node->hn == -1){
        node->fn=-1;
    }else{
        node->fn=(node->gn)+(node->hn);     //set fn after hn is set
    }

    return node;
}


//put onto open list
void putOnOpen(Node * node){
    if(open.empty()||(node->fn)<(open.back()->fn)){
        open.push_back(node);
    }else{
        //use insert to add node pointer in correct order... may implement min heap in order to acomplish
        for(int i = (int)(open.size())-1; i>=0; i--){
            //if the fn is lower for this node save closer to back of vector which is front of queue
            if(node->fn<open.at(i)->fn){
                open.insert(open.begin()+i,node);
                break;
            }else if(i==0){
                open.insert(open.begin(),node);
            }
        }
    }
}

/*determine next legal moves and add to nextMoves array*/
void findMove(Node* parent){
#ifdef HEUR
    int legal [3][2] = {};           //2d array to hold at most 3 sets of legal moves
#else
    int legal [6][2] = {};
#endif
    int j=0;                    //counter for legal array instantiation
    int i=0;                    //counter for nextMoves instantiation

    int A,B,C;                  //hold the towers top piece value or -1 if empty
    A = -1;
    B = -1;
    C = -1;
    //if disk number is less then move is legal

    //check tower1 and tower2

    if (firstMove==true){
        if(n%2){
            legal[j][0]=1;
            legal[j][1]=goalPeg;
        }else{
            legal[j][0]=1;
            legal[j][1]=goalPeg+1;
        }
        firstMove=false;
        putOnOpen(getChild(parent, legal[j]));
    }else{

        //Tower 1 top piece or empty
        if(parent->tower1.empty()){
            A = n+1;
        }else{
            A = (*(parent->tower1).back());
        }
        //Tower 2 top peiece or empty
        if(parent->tower2.empty()){
            B = n+1;
        }else{
            B = (*(parent->tower2).back());
        }
        //Tower 3 top peice or empty
        if(parent->tower3.empty()){
            C = n+1;
        }else{
            C = (*(parent->tower3).back());
        }


        if (A < B){
            //move 1->2 legal
            legal[j][0]= 1;
            legal[j][1]= 2;
            j++;

        }else if(B!=(n+1) && A!=B){
            //move 2->1 is legal
            legal[j][0]= 2;
            legal[j][1]= 1;
            j++;
        }

        //check tower2 and tower3
        if(B < C){
            //move 2->3 is legal
            legal[j][0]= 2;
            legal[j][1]= 3;
            j++;
        }else if(C!=(n+1) && B != C){
            //move 3->2 is legal
            legal[j][0]= 3;
            legal[j][1]= 2;
            j++;
        }

        //check tower1 and tower3
        if(A < C){
            //move 1->3 is legal
            legal[j][0]= 1;
            legal[j][1]= 3;
            j++;
        }else if(C!=(n+1) && A!=C){
            //move 3->1 is legal
            legal[j][0]= 3;
            legal[j][1]= 1;
            j++;
        }
 //   }
        //if not last moved piece is legal, only the amount of legal moves checked
    while(j!=0){
        j--;
        //if not moving most recent piece moved, create child and determine it's position on open.
        if((legal[j][0])!=parent->move[1]){
            putOnOpen(getChild(parent, legal[j]));
            i++;
        }
        //check next legal move

    }
}

}

/*The A* function. Takes the next node with lowest f(n), make current node being processed,
    then remove from open list and determine legal moves, generate children, generate their h(n),
    add them to open list, front is lowest (closest to goal) f(n)
 */
void aStar(){
    Node * current = open.back();
    //take off open list and put on closed list
    open.pop_back();
    findMove(current);
    closed.push_back(current);
}

/*_______________TESTS_______________*/

//determine if disks are the number requested and in correct order
int testDisks (){
    int i;

    //if the vector has more elements than asked for
    if(disk.size()!=unsigned(n)){
        return -2;
    }

    //if any of the values aren't at right index
    for(i=0; i<n; i++){
        if(disk.at(i)!=i){
            return -1;
        }
    }

    //else vector is set up properly
    return 0;
}

//determine if tower1 is a list of decreasing disk sizes
int testTower1(Node *node){
    int j = 0;

    //if they contain a differing number of elements
    if(disk.size()!=node->tower1.size()){
        return -2;
    }

    //if they are not containing the same value
    for(int i= n-1;i>=0;i--){
        if(disk.at(j)!=*node->tower1.at(i)){
            return -1;
        }
    }

    //if they are equivalent
    return 0;
}

/*_______________MAIN_______________*/

int main(int argc, char* argv[]) {
  if(argc != 2) {
      std::cerr << "\nUSAGE: " << argv[0] << " num_disks\n";
      std::cout << "\ngot : " << argv[1] << "\n";
      getN();
  }
  else
      n = atoi(argv[1]);  //convert first argument to int n.

    std::vector <int> goalPath;
    int i=0;
    //int minMoves;
    //int totalMoves;

    Node starting;
      starting.name=nodeName++;
      starting.hn=0;
      starting.gn=0;
      starting.fn=0;
      starting.move[0]=0;
      starting.move[1]=0;
      starting.parent=NULL;

    initTower(&starting);
    open.push_back(&starting);
    getH(&starting);
    int finalPath=0;                //
    Node *goalNode;                 //node pointer for goal node when found

    //the aStar loop that pulls from open list and then generates next moves. returns so that when goal is found loop may exit
    while(goal==0){
        if(open.empty()){
            std::cout<<"ERROR: reached end of open list before goal node found.";
            return 0;
        }
        aStar();
    }

    //The method I am using when a goal node is found is puting it at the back of the open list
    goalNode=open.back();

    //backtrack through parents for shortest path
    while(goalNode->parent!=NULL){
        goalPath.push_back(goalNode->move[1]);
        goalPath.push_back(goalNode->move[0]);
        goalNode=(goalNode->parent);
    }

    //display shortest path
    //std::cout<<"Moves to goal: \n";
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

    return 0;
}
