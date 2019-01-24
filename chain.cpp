#include "chain.h"
#include "chain_given.cpp"

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain(){
  clear();
  delete head_;
  head_ = NULL;
}

/**
 * Inserts a new node at the end of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block & ndata){
  Node * newnode = new Node(ndata); //must have pointer to heap mem
  Node * last = head_->prev;
  last->next = newnode;
  newnode->prev = last;
  head_->prev = newnode;
  newnode->next = head_;
  length_++; //need to increment length after you add a new node
}

/**
 * Modifies the Chain by moving the subchain of len Nodes,
 * starting at position startPos, dist positions toward the
 * end of the chain.  If startPos + len - 1 + dist > length
 * then dist = length - startPos - len + 1 (to prevent gaps
 * in the new chain).
 * The subchain occupies positions (startPos + dist) through
 * (startPos + dist + len - 1) of the resulting chain.
 * The order of subchain nodes is not changed in the move.
 * You may assume that: 1 <= startPos <= length - len + 1,
 * 0 <= dist <= length, and 0 <= len <= length. 
 */
void Chain::moveBack(int startPos, int len, int dist){
  if (startPos + len - 1 + dist > size()){
    dist = size() - startPos - len + 1;
  }
  Node * subStart = walk(head_, startPos); // pointer to Node that is the start of subchain
  Node * beforeSub = subStart->prev; // pointer to Node right before start of subchain
  Node * subEnd = walk(subStart, (len-1)); // pointer to Node at end of subchain
  Node * afterSub = subEnd->next; // pointer to Node right after subchain
  Node * newPos = walk(afterSub, (dist-1)); // pointer to new position where subchain starts
  Node * afterNewPos = newPos->next; // pointer to new position where subchain ends
  subStart->prev = newPos;
  subEnd->next = afterNewPos;
  beforeSub->next = afterSub;
  newPos->next = subStart;
  afterNewPos->prev = subEnd;
  afterSub->prev = beforeSub;

}

/**
 * Rolls the current Chain by k nodes: reorders the current list
 * so that the first k nodes are the (n-k+1)th, (n-k+2)th, ... , nth
 * nodes of the original list followed by the 1st, 2nd, ..., (n-k)th
 * nodes of the original list where n is the length.
 */
void Chain::roll(int k){
  moveBack(0, k, size() - k);
}

/**
 * Modifies the current chain by reversing the sequence
 * of nodes from pos1 up to and including pos2. You may
 * assume that pos1 and pos2 exist in the given chain,
 * and pos1 <= pos2.
 * The positions are 1-based.
 */
void Chain::reverseSub(int pos1, int pos2){
  int lengthSubChain = pos2 - pos1 + 1;
  int currStartPos = pos1;
  for(int i = lengthSubChain - 1; i > 0; i--){ // why i>1 not i>0? seg fault...
    moveBack(currStartPos, 1, i);
    // currStartPos++; wrong the starting position will always be the same
  }
}

/*
* Modifies both the current chain and the "other" chain by removing
* nodes from the other chain and adding them between the nodes
* of the current chain -- one "other" node between two current nodes --
* until one of the two chains is exhausted.  Then the rest of the
* nodes in the non-empty chain follow.
* The length of the resulting chain should be the sum of the lengths
* of current and other. The other chain should have only
* the head_ sentinel at the end of the operation.
* The weave fails and the original
* chains should be unchanged if block sizes are different.
* In that case, the result of the function should be:
* cout << "Block sizes differ." << endl;
*/
void Chain::weave(Chain & other) { // leaves other empty.
  if (other.size() == 0) {
    return;
  }

  Node * currOriginal = head_->next; // pointer to current Node in original chain
  Node * nextOriginal = currOriginal->next; 
  Node * currOther = other.removeFront(); // pointer to curent Node that has been removed in other chain

  int widthOriginalBlock = currOriginal->data.width();
  int heightOriginalBlock = currOriginal->data.height();
  int widthOtherBlock = currOther->data.width();
  int heightOtherBlock = currOther->data.height(); 

  if(widthOriginalBlock != widthOtherBlock || heightOriginalBlock != heightOtherBlock){
    cout << "Block sizes differ." << endl;
    return;
  }

  while(nextOriginal != head_ && other.size() != 0){
    //inserting the Node from other

    currOriginal->next = currOther;
    currOther->prev = currOriginal;
    currOther->next = nextOriginal;
    nextOriginal->prev = currOther;

    //updating pointers
    currOriginal = nextOriginal;
    nextOriginal = nextOriginal->next;
    currOther = other.removeFront();

    length_++;
  }

  if(other.size() != 0){
    Node * first = other.head_->next;
    Node * last = other.head_->prev;
    Node * lastOriginal = head_->prev;

    lastOriginal->next = first;
    first->prev = lastOriginal;
    head_->prev = last;
    last->next = head_;
  }
}

 /**
  * Removes a Node from the front of the Chain, excluding the Sentinel
  * Returns pointer to said Node
  */
Chain::Node * Chain::removeFront(){
  if (length_ == 0) {
    return NULL;
  }

  Node * removed = head_->next;
  Node * nextRemove = removed->next;
  if(nextRemove != NULL){
  head_->next = nextRemove;
  
  nextRemove->prev = head_;
  length_--;
  }
  return removed;
 }


/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class except for the sentinel head_. Sets length_
 * to zero.  After clear() the chain represents an empty chain.
 */
void Chain::clear() {
  Node * current = head_->next; //pointer to first element after head
  while(current != head_){ //check if current pointer points to head, if so stops execution, don't delete head
    Node* nextnode = current->next;
    delete current; 
    current = nextnode; //update current
  }
  length_ = 0;
  //if you call delete on a pointer it deallocates all the things it points to
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const& other) {
  clear();
  if (other.head_->next == NULL) return;
  Node * currOther = other.head_->next;
  width_ = other.width_;
  height_ = other.height_;
  length_ = other.length_;
  for(int i = other.size(); i > 1; i--){
    insertBack(currOther->data);
    currOther = currOther->next;
  }
}
