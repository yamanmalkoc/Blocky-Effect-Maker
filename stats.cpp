
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
}

void stats::sums(PNG & im){
    vector<vector<double>> sum(im.width(), vector<double>(0));
    vector<vector<double>> lum(im.width(), vector<double>(0));
    vector<vector<double>> hX(im.width(), vector<double>(0));
    vector<vector<double>> hY(im.width(), vector<double>(0));
    vector<vector<vector<int>>> hist(im.width(), vector<vector<int>>(im.height(), vector<int>(36)));
    this->sumSat = sum; 
    this->sumLum = lum; 
    this->sumHueX = hX; 
    this->sumHueY = hY; 
    this->hist = hist;
    for(int x = 0; x < im.width(); x++){
        for(int y = 0; y < im.height(); y++){
            HSLAPixel * p = im.getPixel(x,y); 
            double currSat = p->s; 
            double currLum = p->l; 
            double currHueX = cos(p->h*PI/180);
            double currHueY = sin(p->h*PI/180); 
            double sum_entry = 0.0;
            double lum_entry = 0.0;
            double hX_entry  = 0.0;
            double hY_entry  = 0.0; 
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
            this->hist[x][y] = histMaker(im,x,y);
        }   
    } 
    printVector(this->sumSat);
}

vector<int> stats::histMaker(PNG & im, int x, int y){
    vector<int> ret(36);
    ret[findBin(im, x, y)]++; 
       for(int k = 0; k < 36; k++){
            if(x == 0 && y == 0){
                return ret;
            }else if(x == 0){
                ret[k] = this->hist[x][y-1][k] + ret[k]; 
            }else if(y == 0){
                ret[k] = this->hist[x-1][y][k] + ret[k];
            }else{
                ret[k] = this->hist[x][y-1][k] + this->hist[x-1][y][k] - this->hist[x-1][y-1][k] + ret[k]; 
            } 
        }
    return ret;
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
    double totalSat;
    double totalLum;
    double totalHueX;
    double totalHueY;

    if(ul.first == 0 && ul.second == 0){
        totalSat = this->sumSat[lr.first][lr.second];
        totalLum = this->sumLum[lr.first][lr.second];
        totalHueX = this->sumHueX[lr.first][lr.second];
        totalHueY = this->sumHueY[lr.first][lr.second]; 
    }else if(ul.first == 0){
        totalSat = this->sumSat[lr.first][lr.second] - this->sumSat[lr.first][ul.second - 1]; 
        totalLum = this->sumLum[lr.first][lr.second] - this->sumLum[lr.first][ul.second - 1];
        totalHueX = this->sumHueX[lr.first][lr.second] - this->sumHueX[lr.first][ul.second - 1];
        totalHueY = this->sumHueY[lr.first][lr.second] - this->sumHueY[lr.first][ul.second - 1]; 
    }else if(ul.second == 0){
        totalSat = this->sumSat[lr.first][lr.second] - this->sumSat[ul.first - 1][lr.second];
        totalLum = this->sumLum[lr.first][lr.second] - this->sumLum[ul.first - 1][lr.second];
        totalHueX = this->sumHueX[lr.first][lr.second] - this->sumHueX[ul.first - 1][lr.second];
        totalHueY = this->sumHueY[lr.first][lr.second] - this->sumHueY[ul.first - 1][lr.second];
    }else{
        totalSat = this->sumSat[lr.first][lr.second] - this->sumSat[lr.first][ul.second -1] - this->sumSat[ul.first - 1][lr.second] + this->sumSat[lr.first - 1][lr.second - 1];
        totalLum = this->sumLum[lr.first][lr.second] - this->sumLum[lr.first][ul.second -1] - this->sumLum[ul.first - 1][lr.second] + this->sumLum[lr.first - 1][lr.second - 1];
        totalHueX = this->sumHueX[lr.first][lr.second] - this->sumHueX[lr.first][ul.second -1] - this->sumHueX[ul.first - 1][lr.second] + this->sumHueX[lr.first - 1][lr.second - 1];
        totalHueY = this->sumHueY[lr.first][lr.second] - this->sumHueY[lr.first][ul.second -1] - this->sumHueY[ul.first - 1][lr.second] + this->sumHueY[lr.first - 1][lr.second - 1]; 
    }
    ret.s = totalSat / (double)area;
    ret.l = totalLum / (double)area;
    ret.h = atan2(totalHueY, totalHueX) * 180 / PI;
    return ret;
}


vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){ 
    if(ul.first == 0 && ul.second == 0)
        return this->hist[lr.first][lr.second];

    vector<int> ret(36);
    for(int k = 0; k < 36; k++){
        if(ul.first == 0){
            ret[k] = this->hist[lr.first][lr.second][k] - this->hist[lr.first][ul.second - 1][k]; 
        }else if(ul.second == 0){
            ret[k] = this->hist[lr.first][lr.second][k] - this->hist[ul.first - 1][lr.second][k];
        }else{
            ret[k] = this->hist[lr.first][lr.second][k] - this->hist[lr.first][ul.second -1][k] - this->hist[ul.first - 1][lr.second][k] + this->hist[lr.first - 1][lr.second - 1][k]; 
        }
    }
    return ret;
}


int stats::findBin(PNG & im, int x, int y){
    HSLAPixel * p = im.getPixel(x,y);
    double h = p->h; 
    for(double i = 0; i < 36; i++){
        if(i*10 <= h  && h < (i+1)*10){
            return i; 
        }
    }
    return -1;
}

// takes a distribution and returns entropy
// partially implemented so as to avoid rounding issues.
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0.;

    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 ) 
            entropy += ((double) distn[i]/(double) area) 
                                    * log2((double) distn[i]/(double) area);
    }

    return  -1 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

    int area = rectArea(ul,lr);
    vector<int> hist = buildHist(ul,lr);
    return entropy(hist, area);

}
