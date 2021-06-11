#pragma once
#include <iostream>
#include <queue>
#include <map>
using namespace std;


const std::string RESET       = "\033[0m";       /* Default White*/
const std::string BLACK       = "\033[30m";      
const std::string RED         = "\033[31m";      
const std::string YELLOW      = "\033[33m";      
const std::string BLUE        = "\033[34m";      
const std::string GREEN       = "\033[32m";     
const std::string MAGENTA     = "\033[35m";      

const std::string BOLDMAGENTA = "\033[1m\033[35m";      
const std::string BOLDCYAN    = "\033[1m\033[36m";      

namespace ariel 
{
    template<typename T> class BinaryTree {

        /**
        * Node struct for holding the Tree vals and future descendants
        * the value of the Node will be the same as the BinaryTree class template
        */


        class Node {
            public:
                T value;
                Node *right = nullptr, *left = nullptr;
                Node(T val) : value(val), right(nullptr), left(nullptr){}
        };


        Node* root; // For Traverse on the BinaryTree
        map<T, Node*> childs; // For O(1) Access when adding
        enum Order { INORDER, PREORDER, POSTORDER };

// `-`-`-`-`-`-`-`-`-`-`-`-` Iterator - START `-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`

        class iterator {

            private:

                Node* root;
                queue<Node*> byOrder;

                void createBy(Order traverse, Node* node){ 
                    switch (traverse)
                    {
                        case INORDER: {

                            if(node == nullptr) { return; }
                            createBy(traverse, node->left);
                            byOrder.push(node);
                            createBy(traverse, node->right);                        
                            break;

                        } case PREORDER: { 

                            if(node == nullptr) { return; }
                            byOrder.push(node);
                            createBy(traverse, node->left);
                            createBy(traverse, node->right);
                            break;

                        } case POSTORDER: {

                            if(node == nullptr) { return; }
                            createBy(traverse, node->left);
                            createBy(traverse, node->right);
                            byOrder.push(node);
                            break;

                        }
                    }
                }
// --------------------------------------- PUBLIC Iterator -----------------------------------------------------
            public:

                iterator(Order traverseHow, Node* ptr = nullptr): root(ptr){ 

                createBy(traverseHow,ptr);
                byOrder.push(nullptr); 
                root = byOrder.front();
                byOrder.pop();

                }
    
                const T* operator->() const
                { return &(root->value); } 

                T& operator*() const
                { return root->value;} 
                
                iterator& operator++() {

                    if (byOrder.empty()){return *this;}

                    root = byOrder.front();
                    byOrder.pop();
                    return *this;
                }
                iterator& operator++(int) {

                    if (byOrder.empty()){return *this;}

                    root = byOrder.front();
                    byOrder.pop();
                    return *this;
                }


                bool operator==(const iterator& other) const 
                {return root == other.root;}

                bool operator!=(const iterator& other) const 
                {return root != other.root;} 

// --------------------------------------- PUBLIC Iterator END -----------------------------------------------------
                    
            }; 

            public:

                iterator begin_inorder() { return iterator(INORDER, root); } iterator  end_inorder() { return iterator(INORDER, nullptr); }
                iterator  begin_preorder() { return iterator(PREORDER, root); } iterator  end_preorder(){ return iterator(PREORDER, nullptr); }
                iterator  begin_postorder(){ return iterator(POSTORDER, root); } iterator  end_postorder(){ return iterator(POSTORDER, nullptr); }
                iterator begin() {return iterator(INORDER, root); } iterator  end() { return iterator(INORDER, nullptr); }
                
// `-`-`-`-`-`-`-`-`-`-`-`-` Iterator - ENDING `-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`

                void copy(Node& copyTo, Node& copyFrom) const{

                    if(copyFrom.left != nullptr) {

                        copyTo.left = new Node(copyFrom.left->value);
                        copy(*(copyTo.left),*(copyFrom.left));
                    }

                    if(copyFrom.right != nullptr) {

                        copyTo.right = new Node(copyFrom.right->value);
                        copy(*(copyTo.right), *(copyFrom.right));
                    }
                }

                BinaryTree():root(nullptr){}

                BinaryTree<T>(const BinaryTree<T>& other) { 

                    if(other.root != nullptr){

                        root = new Node(other.root->value);
                        copy(*root,*(other.root));
                    }
                }
                /**
                 * To Avoid Creating a Copy Constructor at the Node Struct 
                 * I created this method to Delet the tree in Recursion
                 */

                // void Del(Node* shoresh){
                //     if (shoresh != nullptr){
                //         Del(shoresh->left);
                //         Del(shoresh->right);
                //         delete root;
                //     } 
                // }
                ~BinaryTree() { if(root != nullptr){delete root;}}

                /**
                 * Skeleton from our Course Github
                */
                BinaryTree<T> &operator = (BinaryTree<T>&& other) noexcept {
                    delete root;
                    root = other.root;
                    other.root = nullptr; 
                }
                BinaryTree<T> (BinaryTree<T>&& other) noexcept {
                    root = other.root;
                    other.root = nullptr; 
                }
                BinaryTree<T>& operator=(BinaryTree<T> other){
                    if (this == &other) { return *this;}
                    delete root;
                    root = new Node(other.root->value);
                    copy(*root,*other.root);
                    return *this;
                }


                BinaryTree& add_root(T shoresh){ 
                    if (root == nullptr){

                        root = new Node(shoresh);
                        childs[shoresh] = root;

                    } else{

                        childs.erase(childs.find(root->value));
                        root->value = shoresh;
                        childs[shoresh] = root;

                    }
                    return *this;  
                }

                BinaryTree& add_right(T exist, T add){

                    if (root == nullptr)
                    { throw invalid_argument ( " Empty - (<) No Place to Add (>)"); } 
                    
                    if (childs.find(exist) != childs.end()){

                        if (childs[exist]->right == nullptr){   
                            
                            childs[exist]->right = new Node(add);
                            childs[add] = childs[exist]->right;

                        }else{

                            childs.erase(childs.find(childs[exist]->right->value)); 
                            childs[exist]->right->value = add;
                            childs[add] = childs[exist]->right;  

                        }
                        
                        return *this;
                    }

                    throw invalid_argument (" Not Exist in this Tree, Buddy :) ");
                }

                BinaryTree& add_left(T exist, T add){ 
                    
                    if (root == nullptr)
                    { throw invalid_argument ( " Empty - (<) No Place to Add (>)"); } 
                    
                    if (childs.find(exist) != childs.end()){

                        if (childs[exist]->left == nullptr){

                            childs[exist]->left = new Node(add);
                            childs[add] = childs[exist]->left;

                        } else{

                            childs.erase(childs.find(childs[exist]->left->value)); 
                            childs[exist]->left->value = add;
                            childs[add] = childs[exist]->left;
                        }

                        return *this;
                    }

                    throw invalid_argument (" Not Exist in this Tree, Buddy :) ");
                }

                /**
                 * I looked at the web for pretty print 
                 * this was the shortest code so I took it :)
                 * https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
                 * 
                */

                static void printBT(const std::string& prefix, const Node* node, bool isLeft)
                {
                    if( node != nullptr ) {

                        cout << RESET;
                        cout << prefix;
                        if (isLeft)
                        {
                            cout << "├~~>";
                        }else { cout << "└~~>";}
                        
                        // print the value of the node
                        cout << RED;
                        cout << node->value << endl;

                        // enter the next tree level - left and right branch
                        printBT( prefix + (isLeft ? "│   " : "    "), node->right, true);
                        printBT( prefix + (isLeft ? "│   " : "    "), node->left, false);
                    }
                }
                static void printBT(const Node* node){ printBT("", node, false); }

                friend ostream& operator<<(ostream& os, const BinaryTree& bt){

                    os << GREEN << "BT" << "\n";
                    bt.printBT(bt.root);
                    os << RESET << endl;
                    return os;
                }     
        };
};


