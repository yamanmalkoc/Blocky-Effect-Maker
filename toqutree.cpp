
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

	double min_entropy = 100.0;

	PNG rootImage = cropImage(imIn, ul, lr, pow(2, k));

	buildTree(&rootImage, k);//PROBABLY NEEDS TO RETURN SOMETHING ELSE
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
/* your code here */
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
	if(k < 2){
		return NULL;
	}
	pair<int,int> SEul;
	pair<int,int> SElr;
	pair<int,int> SWul;
	pair<int,int> SWlr;
	pair<int,int> NEul;
	pair<int,int> NElr;
	pair<int,int> NWul;
	pair<int,int> NWlr;
	pair<int,int> optimal_point;

	stats image(*im);
	
	double min_entropy = 0.0;
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
			//PNG partSEimg = cropImage(*im, SEul, SElr, pow(2,k));
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
			//Get the stats for all of the cropped images
			//stats SE(partSEimg);
			printf("Stats for SE created\n");
			//stats SW(partSWimg);
			printf("Stats for SW created\n");
			//stats NE(partNEimg);
			printf("Stats for NE created\n");
			//stats NW(partNWimg);
			printf("Stats for NW created\n");
			//Set the origin and lr corner for all these same sized square images
			//pair<int,int> origin;
			//origin.first = 0;
			//origin.second = 0;
			//pair<int, int> end_corner;
			//end_corner.first = pow(2, k - 1) - 1;
			//end_corner.second = pow(2, k - 1) - 1;
			//Get the entropy of the 4 newly cropped images

			printf("Before entropy");
			double SE_entropy = image.entropy(SEul, SElr);
			printf("SE entropy: %d\n", SE_entropy);
			double SW_entropy = image.entropy(SWul, SWlr);
			printf("SW entropy: %d\n", SW_entropy);
			double NE_entropy = image.entropy(NEul, NElr);
			printf("NE entropy: %d\n", NE_entropy);
			double NW_entropy = image.entropy(NWul, NWlr);
			printf("NW entropy: %d\n", NW_entropy);
			// Calcuate the average entopy of these 4 images
			average_entropy = (SE_entropy + SW_entropy + NE_entropy + NW_entropy) / 4;
			//Compare their entropy to the minimum of the previous images' entropy
			if(average_entropy < min_entropy) {
				min_entropy = average_entropy;
				optimal_point.first = SEul.first;
				optimal_point.second = SEul.second;
				printf("UPDATED OptimalX: %i, OptimalY %i\n", optimal_point.first, optimal_point.second);

			}
		}
	}

	//Call the function recursively, ith passing the 4 images that have the minimal entropy
}

PNG toqutree::render(){

// My algorithm for this problem included a helper function
// that was analogous to Find in a BST, but it navigated the 
// quadtree, instead.

/* your code here */

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
	if(validToPrune(root,tol)){
		root->NE = NULL;
		root->NW = NULL;
		root->SE = NULL;
		root->SW = NULL;
	}

	prune(root->NE, tol, root->NE.avg); 
	prune(root->NW, tol, root->NW.avg); 
	prune(root->SE, tol, root->SE.avg); 
	prune(root->SW, tol, root->SW.avg); 

}

int toqutree::validToPrune(Node *root, double tol, HSLAPixel avg){
	//basecase
	if(root == NULL) return 1; 

	//recursive case
	if(validToPrune(root->NE) == 0) return 0;
	if(validToPrune(root->NW) == 0) return 0;
	if(validToPrune(root->SE) == 0) return 0;
	if(validToPrune(root->SW) == 0) return 0;
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

/* your code here */
}


