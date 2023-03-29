#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertfix(AVLNode<Key,Value>* a, AVLNode<Key,Value>* n);
	void rotateLeft(AVLNode<Key, Value>* n, AVLNode<Key, Value>* a);
	void rotateRight(AVLNode<Key, Value>* n, AVLNode<Key, Value>* a);
	void removefix(AVLNode<Key, Value>* n, int8_t diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{

    // TODO
    if(static_cast<AVLNode<Key, Value>*>(this->root_) == NULL){
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        static_cast<AVLNode<Key, Value>*>(this->root_)->setBalance(0);
        return;
    }
    AVLNode<Key, Value>* cur = static_cast<AVLNode<Key, Value>*>(this->root_);
    bool s = true;
    while(s){
        
        if(cur->getKey() < new_item.first){
            if(cur->getRight() !=NULL){
                cur=cur->getRight();
            }
            else{
                cur->setRight(new AVLNode<Key,Value>(new_item.first,new_item.second,cur));
                cur=cur->getRight();
                s=false;
            }

        }
        else if(cur->getKey() > new_item.first){
            if(cur->getLeft() !=NULL){
                cur=cur->getLeft();
            }
            else{
                cur->setLeft(new AVLNode<Key,Value>(new_item.first,new_item.second,cur));
                cur = cur->getLeft();
                s= false;
            }
        }else{
            cur->setValue(new_item.second);
            s=false;
        }
    }
    cur->setBalance(0);
    if(cur->getParent()!=NULL){
        int8_t parentbalan = cur->getParent()->getBalance();
        if(parentbalan==-1){
            cur->getParent()->setBalance(0);

        }
        else if(parentbalan == 1){
            cur->getParent()->setBalance(0);

        }
        else if(parentbalan == 0){
            if(cur->getParent()->getLeft()==cur){
                cur->getParent()->setBalance(-1);
                insertfix(cur->getParent(),cur);

            }
            else{
                cur->getParent()->setBalance(1);
                insertfix(cur->getParent(),cur);
            }
        }
    }


}
template<class Key, class Value>
void AVLTree<Key, Value>:: insertfix(AVLNode<Key,Value>* a, AVLNode<Key,Value>* b){
    if(a ==NULL || a->getParent()==NULL){
        return;
    }
    AVLNode<Key,Value>* c = a->getParent();
    //if a is the left child
    if(a ==c->getLeft()){
        c->updateBalance(-1);
        if(c->getBalance()==0){
            return;
        }
        else if(c->getBalance()==-1){
            insertfix(c,a);

        }
        else if(c->getBalance()==-2){
            if(b==a->getLeft()){
                rotateRight(a,c);
                a->setBalance(0);
                c->setBalance(0);

            }
            else{
                rotateLeft(b,a);
                rotateRight(b,c);
                if(b->getBalance()==-1){
                    a->setBalance(0);
                    c->setBalance(1);
                    b->setBalance(0);

                }
                else if(b->getBalance()==0){
                    a->setBalance(0);
                    c->setBalance(0);
                    b->setBalance(0);
                }
                else if(b->getBalance()==1){
                    a->setBalance(-1);
                    c->setBalance(0);
                    b->setBalance(0);
                }
            }

        }
    }
    else{
        c->updateBalance(1);
        if(c->getBalance()==0){
            return;
        }
        else if(c->getBalance()==1){
            insertfix(c,a);
         }
        else if(c->getBalance()==2){
            if(b==a->getRight()){
                rotateLeft(a,c);
                a->setBalance(0);
                c->setBalance(0);
            }
            else{
                rotateRight(b,a);
                rotateLeft(b,c);
                if(b->getBalance()==-1){
                    a->setBalance(1);
                    c->setBalance(0);
                    b->setBalance(0);
                }
                else if(b->getBalance()==0){
                    a->setBalance(0);
                    c->setBalance(0);
                    b->setBalance(0);
                }
                else if(b->getBalance()==1){
                    a->setBalance(0);
                    c->setBalance(-1);
                    b->setBalance(0);
                }
            }
         }

}
}

template<class Key, class Value>
void AVLTree<Key, Value>:: rotateRight(AVLNode<Key, Value>* a, AVLNode<Key, Value>* b){
    // b = a->getLeft()
    if(b->getParent()){
		AVLNode<Key, Value>* c = b->getParent();
		if(b == c->getLeft()) {
            c->setLeft(a);
        }
		else {
        
            c->setRight(a);
        }
		a->setParent(c);
	}
	else{
		a->setParent(NULL);
		this->root_ = a;
	}
	if(a->getRight()){
		b->setLeft(a->getRight());
		a->getRight()->setParent(b);
	}
	else{
		b->setLeft(NULL);
	}
	a->setRight(b);
	b->setParent(a);
}


template<class Key, class Value>
void AVLTree<Key, Value>:: rotateLeft(AVLNode<Key, Value>* a, AVLNode<Key, Value>* b){
	if(b->getParent()){
		AVLNode<Key, Value>* c = b->getParent();
		if(b == c->getLeft()){
            c->setLeft(a);
        } 
		else {
            c->setRight(a);
        }
		a->setParent(c);
	}
	else{
		a->setParent(NULL);
		this->root_ = a;
	}
	if(a->getLeft()){
		b->setRight(a->getLeft());

		a->getLeft()->setParent(b);
	}
	else{
		b->setRight(NULL);
	}
	a->setLeft(b);
	b->setParent(a);
}
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key)) == NULL){
			return;
	}
   AVLNode<Key, Value>* cur = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
	
	if(cur->getLeft() != NULL && cur->getRight() != NULL){
		AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(cur));
		nodeSwap(pred, cur);
	}
	
	int8_t n;//difference
    
	AVLNode<Key, Value>* a = cur->getParent();//parent
	if(a != NULL){
		if(a->getLeft() == cur){
            n = 1;
        } 
		else{
            n = -1;
        } 
	}

	//child
	AVLNode<Key, Value>* c = NULL;

	if(cur->getLeft() == NULL) {
        c = cur->getRight();
    }
	else {
        c = cur->getLeft();
    }
	if(a != NULL){
		if(a->getLeft() == cur){
            a->setLeft(c);
        } 
		else{
            a->setRight(c);
        } 
	}
	else {
        this->root_ = c;
    }
	if(c != NULL) c->setParent(a);

	delete cur;
	cur = NULL;
	removefix(a, n);
}

template<class Key, class Value>
void AVLTree<Key, Value>::removefix(AVLNode<Key, Value>* a, int8_t num){
	if(a == NULL) return;
	
	AVLNode<Key, Value>* k = a->getParent();
	int8_t numd;
	if(k != NULL){
		if(k->getLeft() == a) {
            numd = 1;
        }
		else {
            numd = -1;
        }
	}
	//if the difference is -1
	if(num == -1){
		//b(n)+difference = -2
		if(a->getBalance() + num == -2){
			AVLNode<Key, Value>* z = a->getLeft();
			//b(n) =-1 
			if(z->getBalance() == -1){
				rotateRight(a->getLeft(), a);
				a->setBalance(0);
        z->setBalance(0);
				
				
				removefix(k, numd);
			}
			//b(n)=0
			else if(z->getBalance() == 0){
				rotateRight(a->getLeft(), a);

				a->setBalance(-1);

				z->setBalance(1);

				return;
			}
            //b(n)=1
			else if(z->getBalance() == 1){
				AVLNode<Key, Value>* m = z->getRight();

				rotateLeft(m, z);
				rotateRight(m, a);

				
				if(m->getBalance() == 0){
					a->setBalance(0);
					z->setBalance(0);
					m->setBalance(0);
				}
				else if(m->getBalance() == -1){
					a->setBalance(1);
					z->setBalance(0);
				    m->setBalance(0);
				}
                else if(m->getBalance() == 1){
					a->setBalance(0);
					z->setBalance(-1);
					m->setBalance(0);
				}
				removefix(k, numd);
			}
		}
		
		else if(a->getBalance() + num == -1) {//b(n) + difference = -1
            a->setBalance(-1);
        }
		
		else if(a->getBalance() + num == 0){//b(n)+difference = 0
			a->setBalance(0);
			removefix(k, numd);
		}
	}
	//if difference is 1
	else{
		
		if(a->getBalance() + num == 2){
			AVLNode<Key, Value>* z = a->getRight();
			
            if(z->getBalance() == 0){
				rotateLeft(a->getRight(), a);
                z->setBalance(-1);
				a->setBalance(1);

				return;
			}
			else if(z->getBalance() == 1){
				rotateLeft(a->getRight(), a);
                z->setBalance(0);
				a->setBalance(0);

				
				removefix(k, numd);
			}
			
			
			
			else if(z->getBalance() == -1){
				AVLNode<Key, Value>* m = z->getLeft();
				rotateRight(m, z);
				rotateLeft(m, a);
                if(m->getBalance() == 0){
					a->setBalance(0);
					z->setBalance(0);
					m->setBalance(0);
				}


				else if(m->getBalance() == -1){
					a->setBalance(0);

					z->setBalance(1);
					m->setBalance(0);
				}
				
				else if(m->getBalance() == 1){

					a->setBalance(-1);
					z->setBalance(0);
					m->setBalance(0);
				}
				removefix(k, numd);
			}
		}
		
		else if(a->getBalance() + num == 1){
            a->setBalance(1);
        } 
		
		else if(a->getBalance() + num == 0){
			a->setBalance(0);

			removefix(k, numd);
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
