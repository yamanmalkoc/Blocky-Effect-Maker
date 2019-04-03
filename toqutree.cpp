
/**
 *
 * toqutree (pa3)
 * significant modification of a quadtree .
 * toqutree.cpp
 * This file will be used for grading.
 *
 */

#include "toqutree.h"

toqutree::Node::Node(pair<int,int> ctr, int dim, HSLAPixel a)
	:center(ctr),dimension(dim),avg(a),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

toqutree::~toqutree(){
	clear(root);
}

toqutree::toqutree(const toqutree & other) {
	root = copy(other.root);
}


toqutree & toqutree::operator=(const toqutree & rhs){
	if (this != &rhs) {
		clear(root);
		root = copy(rhs.root);
	}
	return *this;
}

toqutree::toqutree(PNG & imIn, int k){ 

/* This constructor grabs the 2^k x 2^k sub-image centered */
/* in imIn and uses it to build a quadtree. It may assume  */
/* that imIn is large enough to contain an image of that size. */

/* your code here */
}

int toqutree::size() {
	// root->dimension
}


toqutree::Node * toqutree::buildTree(PNG * im, int k) {

/* your code here */ 

// Note that you will want to practice careful memory use
// In this function. We pass the dynamically allocated image
// via pointer so that it may be released after it is used .
// similarly, at each level of the tree you will want to 
// declare a dynamically allocated stats object, and free it
// once you've used it to choose a split point, and calculate
// an average.

}
   /**
    * Render returns a PNG image consisting of the pixels
    * stored in the tree. may be used on pruned trees. Draws
    * every pixel onto a PNG canvas using the 
    * average color stored in the node.
	*/
PNG toqutree::render(){
	
// My algorithm for this problem included a helper function
// that was analogous to Find in a BST, but it navigated the 
// quadtree, instead.
	return render(root); 
/* your code here */

}

PNG toqutree::render(Node *root){
	//base case
	if(root->NE == NULL) return getNodePNG(root); 

	//recursive case
	PNG NE = render(root->NE);
	PNG NW = render(root->NW);
	PNG SE = render(root->SE);
	PNG SW = render(root->SW);

	return createImage(NE,NW,SE,SW); 
}

PNG toqutree::getNodePNG(Node *n){
	int dim = pow(2,n->dimension);
	PNG ret(dim,dim); 
	for(int x = 0; x < dim; x++){
		for(int y = 0; y < dim; y++){
			HSLAPixel *p = ret.getPixel(x,y);
			*p = n->avg; 
		}
	}
	return ret;
}

PNG toqutree::createImage(PNG NE, PNG NW, PNG SE, PNG SW){
	//Create an image of the appropriate size
	//also width and height are the same 
	int width = NE.width()*2;
	int height = NE.height()*2; 
	PNG ret(width,height);

	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			HSLAPixel * p = ret.getPixel(x,y); 
			if(x < width/2 && y < height/2){
				p = NW.getPixel(x, y); 
			}else if(x >= width/2 && y < height/2){
				p = NE.getPixel(x - width, y);
			}else if(x < width/2 && y >= height/2){
				p = SW.getPixel(x, y - height); 
			}else if(x >= width/2 && y >= height/2){
				p = SE.getPixel(x - width, y - height);  
			}
		}
	}

	return ret; 

}

/* oops, i left the implementation of this one in the file! */
void toqutree::prune(double tol){

	prune(root,tol);

}

void toqutree::prune(Node *root, double tol){
	//base case
	if(root->NE == NULL) 
		return; 

	//recursive case
	if(validToPrune(root,tol,root->avg)){
		root->NE = NULL;
		root->NW = NULL;
		root->SE = NULL;
		root->SW = NULL;
	}

	prune(root->NE, tol); 
	prune(root->NW, tol); 
	prune(root->SE, tol); 
	prune(root->SW, tol); 

}

int toqutree::validToPrune(Node *root, double tol, HSLAPixel avg){
	//basecase
	if(root == NULL) return 1; 

	//recursive case
	if(validToPrune(root->NE, tol, avg) == 0) return 0;
	if(validToPrune(root->NW, tol, avg) == 0) return 0;
	if(validToPrune(root->SE, tol, avg) == 0) return 0;
	if(validToPrune(root->SW, tol, avg) == 0) return 0;
	//Check if any pixel is NOT within the tolerance. 
	if(root->avg.dist(avg) > tol){
		return 0; 
	}
}

/* called by destructor and assignment operator*/
void toqutree::clear(Node * & curr){
/* your code here */

}

/* done */
/* called by assignment operator and copy constructor */
toqutree::Node * toqutree::copy(const Node * other) {
	pair<int,int> ctr = other->center; 
	int dim = other->dimension; 
	HSLAPixel a = other->avg; 
    Node ret(ctr,dim,a);
    return &ret; 
}

