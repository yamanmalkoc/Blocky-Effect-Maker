
#include "stats.h"

stats::stats(PNG & im){
    // initialize all private vectors so that, for each color channel, 
	// (x,y) is the cumulative sum of the the color values from (0,0)
	// to (x,y), and the hist structure is as defined above.
    // Note that the hue (h) value of each pixel is represented by
    // its cartesian coordinates: X = s*cos(h) and Y = s*sin(h). 
    // This is done to simplify distance computation. We calculate
    // the cumulative sums for X and Y separately, and then combine
    // them when we are doing color difference computation.
/* your code here */

}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
/* your code here */
    long xDiff = lr.first - ul.first;
    long yDiff = lr.second - ul.second; 
    return XDiff*yDiff; 

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
	// given a rectangle, return the average color value over the rect.
	/* Each color component of the pixel is the average value of that 
	* component over the rectangle.
	* @param ul is (x,y) of the upper left corner of the rectangle 
	* @param lr is (x,y) of the lower right corner of the rectangle */
    // The average hue value can be computed from the average X and
    // Y values using the arctan function. You should research the 
    // details of this. Finally, please set the average alpha channel to 
    // 1.0.
    HSLAPixel * pixel = getPixel(unsigned int x, unsigned int y);

/* your code here */
}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){

/* your code here */
}

// takes a distribution and returns entropy
// partially implemented so as to avoid rounding issues.
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0.;

/* your code here */

    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 ) 
            entropy += ((double) distn[i]/(double) area) 
                                    * log2((double) distn[i]/(double) area);
    }

    return  -1 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

/* your code here */

}
