
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
    
    this->sumHueX = sums(im, 0);
	this->sumHueY = sums(im, 1);
    this->sumSat = sums(im, 2);
    this->sumLum = sums(im, 3);

    // this.hist = buildHist(im);
}

vector<vector<double>> stats::sums(PNG & im, int cComponent){
    double sum = 0; 
    vector<vector<double>> ret; 
    //Nested for loop to cover all pixels
    for(int x = 0; x < im.width(); x++){
        for(int y = 0; y < im.height(); y++){
            HSLAPixel * p = im.getPixel(x,y); 
            double componentVal = 0.0;
            if(cComponent == 0){
                double h = p->h;
                componentVal =  cos(h*PI/180);
            }else if(cComponent == 1){
                double h = p->h;
                componentVal =  sin(h*PI/180);
            }else if(cComponent == 2){
                componentVal = p->s; 
            }else if(cComponent == 3){
                componentVal = p->l; 
            }else{
                cout<<"ERROR cComponent is not between 0-3!!\n"<<endl; 
            } 

            double entry = sum + componentVal; 
            ret.at(x).push_back(entry);
            sum += componentVal; 
        }
    }
    return ret; 
}


long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    long xDiff = lr.first - ul.first;
    long yDiff = lr.second - ul.second; 
    return xDiff*yDiff; 
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

    long area = rectArea(ul,lr); 
    HSLAPixel ret(0.0, 0.0, 0.0, 1.0);

    //average sat 
    double totalSat = this->sumSat.at(lr.first).at(lr.second) - this->sumSat.at(ul.first).at(ul.second); 
    ret.s = totalSat / (double)area; 

    //average lum
    double totalLum = this->sumLum.at(lr.first).at(lr.second) - this->sumLum.at(ul.first).at(ul.second); 
    ret.l = totalLum / (double)area; 

    //average Hue
    double averageHueY = (this->sumHueY.at(lr.first).at(lr.second) - this->sumHueY.at(ul.first).at(ul.second))/(double)area;
    double averageHueX = (this->sumHueX.at(lr.first).at(lr.second) - this->sumHueX.at(ul.first).at(ul.second))/(double)area;

    ret.h = atan2(averageHueY, averageHueX) * 180 / PI;

    return ret;
}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){

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
