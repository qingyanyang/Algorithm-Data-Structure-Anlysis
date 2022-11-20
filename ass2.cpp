#include <iostream>
#include <vector>
#include <sstream>
#include<algorithm>
using namespace std;
//class Node
class Node{
    int value;
    Node* left;
    Node* right;
public:
    //constructor
    Node() : value(0), left(nullptr),right(nullptr){};
    Node(int value) : value(value), left(nullptr),right(nullptr){};

    int getValue() {
        return value;
    }

    void setValue(int value) {
        Node::value = value;
    }

    Node *getLeft() {
        return left;
    }

    void setLeft(Node *left) {
        Node::left = left;
    }

    Node *getRight() {
        return right;
    }

    void setRight(Node *right) {
        Node::right = right;
    }

private:
    //print node method
    void printNode(){
        cout << value;
    }
};
//class AVL tree
class AVLTree{
private:
    Node* root;
public:
    vector<Node*> vec;
    //constructor
    AVLTree() : root(nullptr){};

    Node *getRoot() const {
        return root;
    }

    void setRoot(Node *root) {
        AVLTree::root = root;
    }

    Node** find(Node* current, Node* pre, int value){
        if(current != nullptr){
            if(value == current->getValue()){
                static Node* array[2];
                array[0]=pre;
                array[1]=current;
                return array;
            }else if(value > current->getValue()){
                pre = current;
                return find(current->getRight(), pre, value);
            }else{
                pre = current;
                return find(current->getLeft(), pre, value);
            }
        }else{
            return nullptr;
        }
    }

    Node* insertNodeRec(Node* current, int value) {
        if(current==nullptr){
            //create a pointer point the new object
            return new Node(value);
        }else{
            if(value > current->getValue()){//the object.value which the current pointer point
                current->setRight(insertNodeRec(current->getRight(), value));
            }else if(value < current->getValue()){
                current->setLeft(insertNodeRec(current->getLeft(), value));
            }
            return current;
        }
    }
    void deleteNode(int value){
        Node** array = find(root, nullptr, value);
        if(array!=nullptr){
            Node* pre = *array;
            Node* current = *(array + 1);
            if(current->getRight() == nullptr && current->getLeft() == nullptr){
                if(pre != nullptr){
                    if(pre->getRight() == current){
                        pre->setRight(nullptr);
                    }else{
                        pre->setLeft(nullptr);
                    }
                }else{
                    setRoot(nullptr);
                }
            }else if(current->getRight() == nullptr && current->getLeft() != nullptr) {
                if(pre != nullptr){
                    if(pre->getRight() == current){
                        pre->setRight(current->getLeft());
                    }else{
                        pre->setLeft(current->getLeft());
                    }
                }else{
                    setRoot(current->getLeft());
                }
            }else if(current->getLeft() == nullptr && current->getRight() != nullptr) {
                if(pre != nullptr){
                    if(pre->getRight() == current){
                        pre->setRight(current->getRight());
                    }else{
                        pre->setLeft(current->getRight());
                    }
                }else{
                    setRoot(current->getRight());
                }
            }else if(current->getLeft() != nullptr && current->getRight() != nullptr){
                Node* leftNodeTmp = current->getLeft();
                Node* rightMostNode = leftNodeTmp;
                Node* rightMostPre = current;
                int count = 0;
                while(leftNodeTmp->getRight() != nullptr){
                    rightMostPre = leftNodeTmp;
                    rightMostNode = leftNodeTmp->getRight();
                    leftNodeTmp = leftNodeTmp->getRight();
                    count++;
                }
                current->setValue(rightMostNode->getValue());
                if(count == 0){
                    current->setLeft(rightMostNode->getLeft());
                }else{
                    rightMostPre->setRight(rightMostNode->getLeft());
                }
            }
        }
    }
    //calculate level of node
    int computeLevel(Node* node){
        if(node != nullptr){
            return max(computeLevel(node->getLeft()), computeLevel(node->getRight()))+1;
        }else{
            return 0;
        }
    }
    //compute balance of node
    int computeBalance(Node* node){
        int balance = computeLevel(node->getLeft())-computeLevel(node->getRight());
        return balance;
    }
    //balanceCheck
    void checkBalance(Node* node){
        if(node != nullptr){
            checkBalance(node->getLeft());
            checkBalance(node->getRight());
            if( abs(computeBalance(node)) >= 2){
                vec.push_back(node);
            }
        }
    }
    //left rotate
    void leftRotate(Node* node){
        Node* right = node->getRight();
        Node* pre = *find(root,nullptr,node->getValue());
        if(pre!=nullptr){
            if(pre->getLeft() == node ){
                pre->setLeft(right);
                node->setRight(right->getLeft());
                right->setLeft(node);
            }else if(pre->getRight() == node){
                pre->setRight(right);
                node->setRight(right->getLeft());
                right->setLeft(node);
            }
        }else{
            root = right;
            node->setRight(right->getLeft());
            root->setLeft(node);
        }
    }
    //right rotate
    void rightRotate(Node* node){
        Node* left = node->getLeft();
        Node* pre = *find(root,nullptr,node->getValue());
        if(pre!=nullptr){
            if(pre->getLeft() == node){
                pre->setLeft(left);
                node->setLeft(left->getRight());
                left->setRight(node);
            }
            if(pre->getRight() == node){
                pre->setRight(left);
                node->setLeft(left->getRight());
                left->setRight(node);
            }
        }else{
            root = left;
            node->setLeft(left->getRight());
            root->setRight(node);
        }
    }
    //balance
    void balance(Node* node){
        if(computeBalance(node)==-2){
            if(computeBalance(node->getRight())<=0){
                leftRotate(node);
            }else{
                rightRotate(node->getRight());
                leftRotate(node);
            }
        }else{
            if(computeBalance(node->getLeft())>=0){
                rightRotate(node);
            }else{
                leftRotate(node->getLeft());
                rightRotate(node);
            }
        }
    }
    //insert AVL tree
    void insertAVL(int value){
        root = insertNodeRec(root, value);
        checkBalance(root);
        while(!vec.empty()){
            balance(vec[0]);
            vec.clear();
            checkBalance(root);
        }
    }
    //delete AVL tree
    void deleteAVL(int value){
        deleteNode(value);
        checkBalance(root);
        while(!vec.empty()){
            balance(vec[0]);
            vec.clear();
            checkBalance(root);
        }
    }
    //traverse
    //inorder
    void inOrderDisplay(){
        if(root!=nullptr){
            inOrderDisplayRec(root);
        }else{
            cout<<"EMPTY"<<endl;
        }
    }
    void inOrderDisplayRec(Node* node){
        if(node != nullptr){
            inOrderDisplayRec(node->getLeft());
            cout << node->getValue();
            cout<<" ";
            inOrderDisplayRec(node->getRight());
        }
    }
    //preorder
    void preOrderDisplay(){
        if(root!=nullptr){
            preOrderDisplayRec(root);
        }else{
            cout<<"EMPTY"<<endl;
        }
    }
    void preOrderDisplayRec(Node* node){
        if(node != nullptr){
            cout << node->getValue();
            cout<<" ";
            preOrderDisplayRec(node->getLeft());
            preOrderDisplayRec(node->getRight());
        }
    }
    //postorder
    void postOrderDisplay(){
        if(root!=nullptr){
            postOrderDisplayRec(root);
        }else{
            cout<<"EMPTY"<<endl;
        }
    }
    void postOrderDisplayRec(Node* node){
        if(node != nullptr){
            postOrderDisplayRec(node->getLeft());
            postOrderDisplayRec(node->getRight());
            cout << node->getValue();
            cout<<" ";
        }
    }
};


int main()
{
    vector<string> operationsSet;
    string line;
    string traverseCommand;
    getline(cin, line);
    istringstream iss(line);
    for (string d; iss >> d; operationsSet.push_back(d)) {};
    traverseCommand = operationsSet.back();
    operationsSet.pop_back();

    AVLTree myAVLTree;
    for(string operation : operationsSet){
        if(operation[0]=='A'){
            int value = stoi(operation.substr(1, operation.find(" ")));
            //myAVLTree.root = myAVLTree.insertNodeRec(myAVLTree.root, value);
            myAVLTree.insertAVL(value);
        }else{
            int value = stoi(operation.substr(1, operation.find(" ")));;
            //myAVLTree.deleteNode(value);
            myAVLTree.deleteAVL(value);
        }
    }
    if(traverseCommand=="IN"){
        myAVLTree.inOrderDisplay();
    }else if(traverseCommand=="PRE"){
        myAVLTree.preOrderDisplay();
    }else{
        myAVLTree.postOrderDisplay();
    }
}