
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
    
    sums(im);
	// this->sumHueY = sums(im, 1);
    // this->sumSat  = sums(im, 2);
    // this->sumLum  = sums(im, 3);

    // this.hist = buildHist(im);
}

// vector<vector<double>> stats::sums(PNG & im, int cComponent){
//     double sum = 0.0; 
//     vector<vector<double>> ret(im.width(), vector<double>(0));
//     // printVector(ret);
//     // cout<<"ret: "<<ret.at(0).at(1)<<endl;
//     // //Nested for loop to cover all pixels
//     for(int x = 0; x < im.width(); x++){
//         for(int y = 0; y < im.height(); y++){
//             HSLAPixel * p = im.getPixel(x,y); 
//             double componentVal = 0.0;
//             if(cComponent == 0){
//                 double h = p->h;
//                 componentVal =  cos(h*PI/180);
//             }else if(cComponent == 1){
//                 double h = p->h;
//                 componentVal =  sin(h*PI/180);
//             }else if(cComponent == 2){
//                 componentVal = p->s; 
//             }else if(cComponent == 3){
//                 componentVal = p->l; 
//             }else{
//                 cout<<"ERROR cComponent is not between 0-3!!\n"<<endl; 
//             } 
//             double entry = sum + componentVal; 
//             ret.at(x).push_back(entry);
//             sum += componentVal; 
//         }   
//     }
//     return ret; 
// }

void stats::sums(PNG & im){
    vector<vector<double>> sum(im.width(), vector<double>(0));
    vector<vector<double>> lum(im.width(), vector<double>(0));
    vector<vector<double>> hX(im.width(), vector<double>(0));
    vector<vector<double>> hY(im.width(), vector<double>(0));
    this->sumSat = sum; 
    this->sumLum = lum; 
    this->sumHueX = hX; 
    this->sumHueY = hY; 
    for(int x = 0; x < im.width(); x++){
        for(int y = 0; y < im.height(); y++){
            HSLAPixel * p = im.getPixel(x,y); 
            double currSat = p->s; 
            double currLum = p->l; 
            double currHueX = cos(p->h*PI/180); ;
            double currHueY = sin(p->h*PI/180); 
            double sum_entry = 0.0;
            double lum_entry = 0.0;
            double hX_entry = 0.0;
            double hY_entry = 0.0; 
            if(x == 0 && y == 0){
                sum_entry = currSat;
                lum_entry = currLum;
                hX_entry  = currHueX; 
                hY_entry  = currHueY;
            }else if(x == 0){
                sum_entry = this->sumSat[x][y-1] + currSat; 
                lum_entry = this->sumLum[x][y-1] + currLum; 
                hX_entry = this->sumHueX[x][y-1] + currHueX; 
                hY_entry = this->sumHueY[x][y-1] + currHueY; 
            }else if(y == 0){
                sum_entry = this->sumSat[x-1][y] + currSat; 
                lum_entry = this->sumLum[x-1][y] + currLum; 
                hX_entry = this->sumHueX[x-1][y] + currHueX; 
                hY_entry = this->sumHueY[x-1][y] + currHueY;
            }else{
                sum_entry = this->sumSat[x][y-1] + this->sumSat[x-1][y] - this->sumSat[x-1][y-1] + currSat;
                lum_entry = this->sumLum[x][y-1] + this->sumLum[x-1][y] - this->sumLum[x-1][y-1] + currLum; 
                hX_entry = this->sumHueX[x][y-1] + this->sumHueX[x-1][y] - this->sumHueX[x-1][y-1] + currHueX; 
                hY_entry = this->sumHueY[x][y-1] + this->sumHueY[x-1][y] - this->sumHueY[x-1][y-1] + currHueY; 
            }
            this->sumSat.at(x).push_back(sum_entry);
            this->sumLum.at(x).push_back(lum_entry); 
            this->sumHueX.at(x).push_back(hX_entry);
            this->sumHueY.at(x).push_back(hY_entry);
        }   
    } 
    printVector(this->sumSat);
}

void stats::printVector( vector<vector<double>> ret){
    cout<<"Printing Vector..."<<endl;
    int x = ret.size();
    int y = ret.at(0).size();
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            cout<<i<<","<<j<<": "<<ret.at(i).at(j)<<endl;
        }
    }
    cout<<"Done printing Vector"<<endl;
}


long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    long xDiff = lr.first - ul.first;
    long yDiff = lr.second - ul.second; 
    return ++xDiff*++yDiff; 
}

//Assumes that the ul corner is always (0,0); 
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
    // printVector(this->sumSat);
    //average sat 
    double totalSat = this->sumSat.at(lr.first).at(lr.second); 
    ret.s = totalSat / (double)area;

    //average lum
    double totalLum = this->sumLum.at(lr.first).at(lr.second); 
    ret.l = totalLum / (double)area; 

    //average Hue
    double averageHueY = (this->sumHueY.at(lr.first).at(lr.second));
    double averageHueX = (this->sumHueX.at(lr.first).at(lr.second));

    ret.h = atan2(averageHueY, averageHueX) * 180 / PI;

    return ret;
}


vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){

}

/* hist[i][j][k]: hist[i][j] contains a histogram of the hue values 
*   0 to 360 into bins of width 10, over the pixels in the rectangle
*   defined by (0,0) through (i,j). For example, hist[i][j][k] contains
*   the number of pixels whose hue value h, is: k*10 <= h < (k+1)*10. 
*/ 
// vector<vector<vector<int>>> stat::initHist(PNG & im){

//     vector<vector<vector<int>>> ret(im.width(), im.height(), vector<int>(36)); 
    

//        for(int x = 0; x < im.width(); x++){
//         for(int y = 0; y < im.height(); y++){
//             if( )
//         }   
//     }

// }

int stats::findBin(PNG & im, int x, int y){
    HSLAPixel * p = im.getPixel(x,y);
    double h = p->h; 
    for(double i = 0; i < 36; i++){
        if(i*10 <= h  && h < (i+1)*10){
            return i; 
        }
    }
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
