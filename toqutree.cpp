
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
printf("YOOOOOOOOO");
	//root->dimension = k;
	printf("Yaman1");
	int centerX = imIn.width() / 2;
	printf("Yaman2");
	int centerY = imIn.height() / 2;
	printf("centerX: %i, centerY %i \n", centerX, centerY);

	pair<int,int> ul;
	ul.first = centerX - pow(2, k - 1);
	ul.second = centerY - pow(2, k - 1);
	pair<int,int> lr;
	lr.first = centerX + pow(2, k - 1);
	lr.second = centerY + pow(2, k - 1);
	pair<int,int> optimal_point;
	optimal_point.first = pow(2, k - 2);
	optimal_point.second = pow(2, k - 2);
	printf("OptimalX: %i, OptimalY %i\n", optimal_point.first, optimal_point.second);

	PNG rootImage = cropImage(imIn, ul, lr, pow(2, k));

	root = buildTree(&rootImage, k);
	return;
}

//A functions that returns a cropped part of an original image
PNG toqutree::cropImage(PNG & img, pair<int,int> ul, pair<int,int> lr, int mod) {
	//Create a new image to copy and return a part of the input image
	PNG croppedImage(lr.first - ul.first + 1, lr.second - ul.second + 1);
	for(int i = ul.first; i <= lr.first; i++){
		for(int j = ul.second; j <= lr.second; j++){
			HSLAPixel * emptyPixelPointer = croppedImage.getPixel(i - ul.first, j - ul.second);
			*emptyPixelPointer = *img.getPixel(i % mod, j % mod);
		}
	}
	return croppedImage;
}

int toqutree::size() {
	return traversal(root,0);
}

int toqutree::traversal(Node *root, int sum){
	//base case
	if(root == NULL){
		printf("baseCase\n");
		return 1;
	} 

	//recursive case
	int sumCopy = sum; 
	sumCopy += traversal(root->NE, sumCopy); 
	printf("first");
	sumCopy += traversal(root->NW, sumCopy); 
	printf("second");
	sumCopy += traversal(root->SE, sumCopy); 
	printf("third");
	sumCopy += traversal(root->SW, sumCopy); 
	printf("forth");

	return sumCopy;
	//check the current node
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

//ToDoList:
//the image passed to this function will be a square
//calculate the average entropy of these 4 blocks
	stats image(*im);
	
	if(k < 2){

		pair<int,int> default_center_lr(1,1);
		pair<int,int> zero(0,0);
		HSLAPixel last_parent_avg = image.getAvg(zero, default_center_lr);
		Node last_parent(default_center_lr, k, last_parent_avg);

		Node SEleaf(zero, k, *im->getPixel(1,1));
		last_parent.SE = &SEleaf;
		Node SWleaf(zero, k, *im->getPixel(0,1));
		last_parent.SW = &SWleaf;
		Node NEleaf(zero, k, *im->getPixel(1,0));
		last_parent.NE = &NEleaf;
		Node NWleaf(zero, k, *im->getPixel(0,0));
		last_parent.NW = &NWleaf;

		SEleaf.SE = NULL;
		SEleaf.SW = NULL;
		SEleaf.NE = NULL;
		SEleaf.NW = NULL;
		SWleaf.SE = NULL;
		SWleaf.SW = NULL;
		SWleaf.NE = NULL;
		SWleaf.NW = NULL;
		NEleaf.SE = NULL;
		NEleaf.SW = NULL;
		NEleaf.NE = NULL;
		NEleaf.NW = NULL;
		NWleaf.SE = NULL;
		NWleaf.SW = NULL;
		NWleaf.NE = NULL;
		NWleaf.NW = NULL;

		return &last_parent;
	}

	pair<int,int> SEul;
	pair<int,int> SElr;
	pair<int,int> SWul;
	pair<int,int> SWlr;
	pair<int,int> NEul;
	pair<int,int> NElr;
	pair<int,int> NWul;
	pair<int,int> NWlr;
	pair<int,int> optimal_pointul;
	pair<int,int> optimal_SWul;
	pair<int,int> optimal_NEul;
	pair<int,int> optimal_NWul;
	pair<int,int> optimal_pointlr;
	pair<int,int> optimal_SWlr;
	pair<int,int> optimal_NElr;
	pair<int,int> optimal_NWlr;

	
	double min_entropy = 1000000.0;
	double average_entropy = 100.0;
	int offset = pow(2, k - 2);
	int inner_square_size = pow(2,k-1);
	int mod = (int)pow(2, k);

	//Two for loops creates all possible 4 image nodes and finds the optimal one
	for(int i = offset; i < inner_square_size + offset; i++) {
		for(int j = offset; j < inner_square_size + offset; j++){
			//Crop the image for SE part
			SEul.first = (i) % mod;
			SEul.second = (j) % mod;
			SElr.first = (i + (int)pow(2, k - 1) - 1) % mod;
			SElr.second = (j + (int)pow(2, k - 1) - 1) % mod;
			//Crop the image for SW part
			SWul.first = (i + (int)pow(2, k - 1)) % mod;
			SWul.second = (j) % mod;
			SWlr.first = (i + (int)pow(2, k - 1) + (int)pow(2, k - 1) - 1) % mod;
			SWlr.second = (j + (int)pow(2, k - 1) - 1) % mod;
			//PNG partSWimg = cropImage(*im, SWul, SWlr, pow(2,k));
			//Crop the image for NE part
			NEul.first = (i) % mod;
			NEul.second = (j + (int)pow(2, k - 1)) % mod;
			NElr.first = (i + (int)pow(2, k - 1) - 1) % mod;
			NElr.second = (j + (int)pow(2, k - 1) + (int)pow(2, k - 1) - 1) % mod;
			//PNG partNEimg = cropImage(*im, NEul, NElr, pow(2,k));
			//Crop the image for the NW part
			NWul.first = (i + (int)pow(2, k - 1)) % mod;
			NWul.second = (j + (int)pow(2, k - 1)) % mod;
			NWlr.first = (i + (int)pow(2, k - 1) + (int)pow(2, k - 1) - 1) % mod;
			NWlr.second = (j + (int)pow(2, k - 1) + (int)pow(2, k - 1) - 1) % mod;
			//PNG partNWimg = cropImage(*im, NWul, NWlr, pow(2,k));

			//printf("Before entropy");
			double SE_entropy = image.entropy(SEul, SElr);
			//printf("SE entropy: %f\n", SE_entropy);
			double SW_entropy = image.entropy(SWul, SWlr);
			//printf("SW entropy: %f\n", SW_entropy);
			double NE_entropy = image.entropy(NEul, NElr);
			//printf("NE entropy: %f\n", NE_entropy);
			double NW_entropy = image.entropy(NWul, NWlr);
			//printf("NW entropy: %f\n", NW_entropy);
			// Calcuate the average entopy of these 4 images
			average_entropy = (SE_entropy + SW_entropy + NE_entropy + NW_entropy) / 4;
			//Compare their entropy to the minimum of the previous images' entropy
			if(average_entropy < min_entropy) {
				min_entropy = average_entropy;
				optimal_pointul.first = SEul.first;
				optimal_pointul.second = SEul.second;
				optimal_SWul.first = optimal_pointul.first + inner_square_size;
				optimal_SWul.second = optimal_pointul.second;
				optimal_NEul.first = optimal_pointul.first;
				optimal_NEul.second = optimal_pointul.second + inner_square_size;
				optimal_NWul.first = optimal_pointul.first + inner_square_size;
				optimal_NWul.second = optimal_pointul.second + inner_square_size;
			}
			//printf("I:  %i  J: %i Offset: %i\n", i,j,offset);
		}
	}
	pair<int,int> origin(0,0);
	pair<int,int> right_bottom(im->width() - 1, im->height() - 1);
	HSLAPixel avg_pixel = image.getAvg(origin, right_bottom);
	Node current_node(optimal_pointul, k, avg_pixel);
	//printf("Out of the for loop");
	
	optimal_pointlr.first = optimal_pointul.first + inner_square_size - 1;
	optimal_pointlr.second = optimal_pointul.second + inner_square_size - 1;
	optimal_SWlr.first = optimal_SWul.first + inner_square_size - 1;
	optimal_SWlr.second = optimal_SWul.second + inner_square_size - 1;
	optimal_NElr.first = optimal_NEul.first + inner_square_size - 1;
	optimal_NElr.second = optimal_NEul.second + inner_square_size - 1;
	optimal_NWlr.first = optimal_NWul.first + inner_square_size - 1;
	optimal_NWlr.second = optimal_NWul.second + inner_square_size - 1;

	PNG SEimage = cropImage(*im, optimal_pointul, optimal_pointlr, pow(2,k));
	PNG SWimage = cropImage(*im, optimal_SWul, optimal_SWlr, pow(2,k));
	PNG NEimage = cropImage(*im, optimal_NEul, optimal_NElr, pow(2,k));
	PNG NWimage = cropImage(*im, optimal_NWul, optimal_NWlr, pow(2,k));
	//Call the function recursively, ith passing the 4 images that have the minimal entropy
	current_node.SE = buildTree(&SEimage, k-1);
	current_node.SW = buildTree(&SWimage, k-1);
	current_node.NE = buildTree(&NEimage, k-1);
	current_node.NW = buildTree(&NWimage, k-1);

	return &current_node;
}


void toqutree::printNode(Node *node){
	if(node == NULL){
		printf("This node is NULL\n"); 
	}else{
		printf("This node is Not NULL");
		if(node->dimension != NULL){
			printf("Dim: %i\n", node->dimension);
		}else{
			printf("Dimension is NULL\n"); 
		}
		if(&node->center != NULL){
			printf("Center: %i, %i\n", node->center.first, node->center.second);
		}else{
			printf("Center is NULL\n"); 
		}
		if(&node->avg != NULL){
			printf("Avg: Hue %f, Sat %f, Lum %f\n", node->avg.h, node->avg.s, node->avg.l);
		}else{
			printf("Avg is NULL\n"); 
		}
		if(node->SE != NULL){
			printf("SE is not NULL");
		}else{
			printf("SE is NULL\n"); 
		}
		if(node->SW != NULL){
			printf("SW is not NULL");
		}else{
			printf("SW is NULL\n"); 
		}
		if(node->NE != NULL){
			printf("NE is not NULL");
		}else{
			printf("NE is NULL\n"); 
		}
		if(node->NW != NULL){
			printf("NW is not NULL");
		}else{
			printf("NW is NULL\n"); 
		}
	}
	printf("\n\n");

}

      
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
	
	// prune(root,tol);

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
	ret.SE = other->SE;
	ret.NE = other->NE;
	ret.SW = other->SW;
	ret.NW = other->NW;
    return &ret; 
}


