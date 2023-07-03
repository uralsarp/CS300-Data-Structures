#include "BVHTree.h"


using namespace std;

BVHTree::BVHTree(){
	root=NULL;
}

BVHTree::~BVHTree(){
	Destroy(root);
} 


void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level) {
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}
std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
	tree.printNode(out, tree.root, 0);
	return out;
}

void BVHTree::addBVHMember(AABB objectArea, std::string name){
	BVHTreeNode *newNode = new BVHTreeNode(objectArea,name,true);
	string branchname="branch";
	if (root==NULL){ //tree is empty
		root=newNode;
		root->leftChild=NULL;
		root->rightChild=NULL;
		root->parent=NULL;

		map[name]=newNode; //add root to the hashmap
	}
	else if (root->rightChild==NULL && root->leftChild==NULL){ //there is exactly one node in the tree
		BVHTreeNode *oldRoot=root; 
		AABB area=((oldRoot->aabb)+(newNode->aabb));
		
		BVHTreeNode *branchNode = new BVHTreeNode (area,branchname,false);
		root=branchNode;
		oldRoot->parent=root;
		oldRoot->isLeaf=true;
		newNode->parent=root;
		root->rightChild=oldRoot;
		root->leftChild=newNode;

		map[name]=newNode;
	}
	else{ //there is more than one node in the tree
		BVHTreeNode *existingLeaf;
		BVHTreeNode *temp;
		temp=root;
		while (temp->isLeaf!=true){
			int increaseInRightTreeSize = AABB::unionArea(newNode->aabb,temp->rightChild->aabb) - temp->rightChild->aabb.getArea();
			int increaseInLeftTreeSize = AABB::unionArea(newNode->aabb,temp->leftChild->aabb) - temp->leftChild->aabb.getArea();
			if (increaseInRightTreeSize < increaseInLeftTreeSize){
				temp=temp->rightChild;
			}
			else{
				temp=temp->leftChild;
			}
		}

		existingLeaf=temp;
		AABB area=((existingLeaf->aabb)+(newNode->aabb));
		BVHTreeNode *newbranch = new BVHTreeNode(area,branchname,false);
		
		newbranch->parent=existingLeaf->parent;

		if(newbranch->parent->leftChild==existingLeaf){
			newbranch->parent->leftChild=newbranch;
		}
		else{
			newbranch->parent->rightChild=newbranch;
		}
		newbranch->leftChild=newNode;
		newbranch->rightChild=existingLeaf;
		existingLeaf->parent=newbranch;
		newNode->parent=newbranch;
		
		BVHTreeNode *temp2;
		temp2=newbranch->parent;
		while(newbranch->parent!=NULL){ 
			newbranch->parent->aabb = newbranch->parent->leftChild->aabb + newbranch->parent->rightChild->aabb;
			newbranch->parent=newbranch->parent->parent;
		}

		root->aabb=root->leftChild->aabb+root->rightChild->aabb;
		newbranch->parent=temp2;
		map[name]=newNode;
	}
}

void BVHTree::moveBVHMember(std::string name, AABB newLocation){
	BVHTreeNode *toMove;
	toMove=map[name];

	if (toMove==root){ // if its the root
		toMove->aabb=newLocation;
	}
	//if its inside the boundaries of its parent
	else if (newLocation.minY>=toMove->parent->aabb.minY && newLocation.minX>=toMove->parent->aabb.minX && newLocation.maxX<=toMove->parent->aabb.maxX && newLocation.maxY<=toMove->parent->aabb.maxY){
		toMove->aabb=newLocation;
	}
	//outside of its parents boundaries
	else{
		removeBVHMember(name);
		addBVHMember(newLocation,name);
	}
}

void BVHTree::removeBVHMember(std::string name){
	BVHTreeNode *toRemove;
	toRemove=map[name];

	if (toRemove==toRemove->parent->rightChild){
		BVHTreeNode *temp;
		temp=toRemove->parent;
		
		toRemove->parent->leftChild->parent=toRemove->parent->parent;

		if(toRemove->parent==toRemove->parent->parent->rightChild){
			toRemove->parent->parent->rightChild=toRemove->parent->leftChild;
		}
		else{
			toRemove->parent->parent->leftChild=toRemove->parent->leftChild;
		}
		
		toRemove->parent=toRemove->parent->leftChild;
		delete temp;
	}
	else{
		BVHTreeNode *temp;
		temp=toRemove->parent;
	
		toRemove->parent->rightChild->parent=toRemove->parent->parent;

		if(toRemove->parent==toRemove->parent->parent->rightChild){
			toRemove->parent->parent->rightChild=toRemove->parent->rightChild;
		}
		else{
			toRemove->parent->parent->leftChild=toRemove->parent->rightChild;
		}

		toRemove->parent=toRemove->parent->rightChild;
		delete temp;
	}
	delete toRemove;
	map.erase(name);
}

std::vector<std::string> BVHTree::collides(BVHTreeNode *x, AABB object, std::vector<std::string> names){

	std::vector<std::string>rightnames;
	std::vector<std::string>leftnames;

	if (x==NULL){ 
		return names;
	}
	else if(x->isLeaf==true && x->aabb.collide(object)==true){ // if the node collides and its a hitbox
		names.push_back(x->name);
		return names; // ???
	}
	else if(x->isLeaf==false && x->aabb.collide(object)==true){ // if the node collides and its a branch node
		if(x->rightChild!=NULL){ //if right child of node exists
			rightnames=collides(x->rightChild,object,names);
		}
		if(x->leftChild!=NULL){ //if left child of node exists
			leftnames=collides(x->leftChild,object,names);
		}
		names=leftnames;
		for(int i=0;i<rightnames.size();i++){
			names.push_back(rightnames[i]);
		}
		return names;
	}
	else{
		return names;
	}
}

std::vector<std::string> BVHTree::getCollidingObjects(AABB object){
	std::vector<std::string> names;

	if (root->aabb.collide(object)==true && root->name!="branch"){ // collides with the root and root is the only node in the tree
		names.push_back(root->name);
		return names;
	}
	else if (root->aabb.collide(object)==true && root->name=="branch"){ // collides with the root and root is a branch node
		names=collides(root,object,names);
		return names;
	}
	else{
		return names;
	}
}

void BVHTree::Destroy(BVHTreeNode *cur){
	if(cur != NULL){

		if((cur->leftChild == NULL) && (cur->rightChild == NULL)){ //if root is the only node in the tree
			delete cur;
			cur = NULL;
		} 
		else{
			Destroy(cur->leftChild);
			Destroy(cur->rightChild);
		} 
	} 
}