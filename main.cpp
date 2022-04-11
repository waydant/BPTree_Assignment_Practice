#include<bits/stdc++.h>

using namespace std;

int d,t;

class Node{
    public:
        bool isLeaf;
        vector<int> keys;
        vector<Node*> ptr;
        Node();
        Node* parent;
        int sizeOfNode;
};

class BPTree {
    
    private:
        Node* root;
        void insertInternal(int, Node*, Node*);
        Node* findParent(Node*, Node*);

    public:
        BPTree();
        void search(int);
        void insert(int);
        void display(Node*);
        Node* getRoot();
        int indexNodes;
        int dataNodes;
};

Node::Node(){
    isLeaf = true;
}

BPTree::BPTree(){
    root = nullptr;
    indexNodes = 0;
    dataNodes = 0;
}

void BPTree::insert(int x){

    if(root==nullptr){
        root = new Node;
        root->keys.push_back(x);
        root->isLeaf = true;
        root->parent = nullptr;
        dataNodes++;
        root->sizeOfNode = 1;
        // cout<<"here1\n";
    }else{
        // cout<<"here1\n";
        Node* cursor = root;
        Node* parent;
        while(cursor->isLeaf==false){
            parent = cursor;

            for(int i=0;i<cursor->sizeOfNode;i++){
                if(x<cursor->keys[i]){
                    cursor = cursor->ptr[i];
                    break;
                }
                if(i==(cursor->sizeOfNode-1)){
                    cursor = cursor->ptr[i+1];
                    break;
                }
            }
        }
        // cout<<cursor->sizeOfNode<<endl;
        // cout<<2*d<<endl;
        if(cursor->sizeOfNode < (2*d)){
            int i=0;
            while(i<cursor->sizeOfNode){
                if(x<=cursor->keys[i]){
                    break;
                }
                i++;
            }
            cursor->keys.push_back(x);
            for(int j=cursor->sizeOfNode;j>i;j--){
                cursor->keys[j] = cursor->keys[j-1];
            }
            cursor->keys[i] = x;
            cursor->sizeOfNode++;

            // cursor->ptr[cursor->sizeOfNode] = cursor->ptr[cursor->sizeOfNode - 1] = nullptr;
        }else{
            Node* newLeaf = new Node;

            vector<int> virtualNode;
            for(int i=0; i < (2*d) ;i++){
                virtualNode.push_back(cursor->keys[i]);
            }
            virtualNode.push_back(x);

            int i=0,j;
            while( i < (2*d) ){
                if(x<=virtualNode[i]){
                    break;
                }
                i++;
            }
            for( j = 2*d + 1;j>i;j--){
                virtualNode[j] = virtualNode[j-1];
            }
            virtualNode[i] = x;
            newLeaf->isLeaf = true;
            dataNodes++;
            cursor->sizeOfNode = d;
            newLeaf->sizeOfNode = d + 1;

            (cursor->keys).clear();

            for(i =0;i<d;i++){
                cursor->keys.push_back(virtualNode[i]);
            }

            for(i=0;i<d+1;i++){
                newLeaf->keys.push_back(virtualNode[i+d]);
            }

            if(cursor==root){
                Node* newRoot = new Node;
                newRoot->keys.push_back(newLeaf->keys[0]);
                newRoot->ptr.push_back(cursor);
                newRoot->ptr.push_back(newLeaf);
                newRoot->isLeaf = false;
                newRoot->sizeOfNode = 1;
                indexNodes++;
                cursor->parent = newRoot;
                newLeaf->parent = newRoot;
                root = newRoot;
            }else{
                // recursively inserte the number in the index B tree
                // cout<<"now we will insert "<<newLeaf->keys[0]<<" inside the index node"<<endl;
                insertInternal(newLeaf->keys[0],parent,newLeaf);
            }
        }
    }
}

void BPTree::insertInternal(int x, Node* cursor, Node* child){
    // cout<<t<<endl;
    if(cursor->sizeOfNode <= (2*t)){
        // cout<<"not inside risky "<<x<<endl;;
        int i=0;
        while(i<cursor->sizeOfNode){
            if(x <= cursor->keys[i]){
                break;
            }
            i++;
        }
        cursor->keys.push_back(x);
        for(int j = cursor->sizeOfNode;j>i;j--){
            cursor->keys[j] = cursor->keys[j-1];
        }
        for(int j=cursor->sizeOfNode+1;j>i+1;j--){
            cursor->ptr[j] = cursor->ptr[j-1];
        }
        cursor->keys[i] = x;
        cursor->sizeOfNode++;
        cursor->ptr[i+1] = child;
    }
    else{
        // cout<<"inside something risky\n";
        Node* newIndexNode = new Node;
        vector<int> virtualKey;
        vector<Node*> virtualPtr;
        for(int i=0;i<=2*t;i++){
            virtualKey.push_back(cursor->keys[i]);
        }
        for(int i=0;i<=2*t+1;i++){
            virtualPtr.push_back(cursor->ptr[i]);
        }

        int i=0,j;
        virtualKey.push_back(x);
        virtualPtr.push_back(child);
        while(i<= 2*t){
            if(x<=virtualKey[i]){
                break;
            }
            i++;
        }

        for(int j=2*t+2;j>i;j--){
            virtualKey[j] = virtualKey[j-1];
        }
        virtualKey[i] = x;
        for(int j=2*t+3;j>i+1;j--){
            virtualPtr[j] = virtualPtr[j-1];
        }
        virtualPtr[i+1] = child;
        newIndexNode->isLeaf = false;
        cursor->sizeOfNode = t;
        newIndexNode->sizeOfNode = t+1;
        cursor->keys.clear();
        cursor->ptr.clear();
        for(i = 0;i<=t;i++){
            if(i<t)
                cursor->keys.push_back(virtualKey[i]);
            cursor->ptr.push_back(virtualPtr[i]);
        }
        for(i=0;i<=t+1;i++){
            if(i<=t)
                newIndexNode->keys.push_back(virtualKey[t+i+1]);
            newIndexNode->ptr.push_back(virtualPtr[i+t+1]);
        }
        indexNodes++;
        newIndexNode->parent = cursor->parent;
        if(cursor == root){
            // cout<<cursor->sizeOfNode<<" "<<virtualKey[t]<<endl;
            Node* newRoot = new Node;
            indexNodes++;
            newRoot->keys.push_back(virtualKey[t]);
            newRoot->ptr.push_back(cursor);
            newRoot->ptr.push_back(newIndexNode);
            newRoot->isLeaf = false;
            newRoot->sizeOfNode = 1;
            root = newRoot;
            cursor->parent = newRoot;
            newIndexNode->parent = newRoot;
        }
        else{
            insertInternal(cursor->keys[t],findParent(root,cursor),newIndexNode);
        }
    }
}

Node *BPTree::findParent(Node* cursor, Node* child){
    Node* parent;
    if(cursor->isLeaf || cursor->ptr[0]->isLeaf){
        return nullptr;
    }
    for(int i=0;i<cursor->sizeOfNode+1;i++){
        if(cursor->ptr[i]==child){
            parent = cursor;
            return parent;
        }else{
            parent = findParent(cursor->ptr[i],child);
            if(parent!=nullptr){
                return parent;
            }
        }
    }
    return parent;
}

/*
0 1 100 200 300 400 
trying to print
1 2 300 
trying to print
1 3 300 500 
trying to print
1 3 300 500 
trying to print
1 4 200 300 500 
trying to print
inside something risky
1 5 200 300 500 
trying to print
*/

Node *BPTree::getRoot(){
    return root;
}

int main(){
    int option,value;
    cin>>d>>t;
    // cout<<d<<t<<endl;
    BPTree tree;
    while(1){
        cin>>option;
        if(option==1){
            cin>>value;
            tree.insert(value);
            // cout<<"inserted?\n";
        }else if(option==2){
            cout<<tree.indexNodes<<" "<<tree.dataNodes<<" ";
            Node* root = tree.getRoot();
            for(int i=0;i<root->keys.size();i++){
                cout<<root->keys[i]<<" ";
            }
            cout<<endl;
            // cout<<"trying to print\n";
        }else if(option==3){
            // cout<<"breaking\n";
            break;
        }
    }
}

/*
1
1
1 100
2
1 200
2
1 400
2
1 300
2
1 500
2
1 600
2
1 700
2
1 800
2
1 150
2
1 350
2
1 250
2
1 251
2
1 252
2
1 253
2
1 650
2
1 625
2
1 900
2
1 750
2
1 850
2
1 1000
2
1 1100
2
1 1200
2
1 1150
2
1 1300
2
1 1400
2
1 1350
2
3

0 1 100 
0 1 100 200 
1 2 200 
1 3 200 300 
1 4 200 300 400 
3 5 300 
3 6 300 
4 7 300 500 
4 7 300 500 
4 7 300 500 
4 7 300 500 
4 8 300 500 
4 9 300 500 
5 10 250 300 500 
5 10 250 300 500 
5 11 250 300 500 
8 12 300 
8 12 300 
8 13 300 
9 14 300 
9 15 300 
9 16 300 
10 17 300 
10 17 300 
10 18 300 
10 19 300

*/