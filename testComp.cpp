#define CATCH_CONFIG_MAIN
#include "cs221util/catch.hpp"
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "stats.h"
#include "toqutree.h"

using namespace std;
using namespace cs221util;


TEST_CASE("stats::basic rectArea","[weight=1][part=stats]"){

    PNG data; data.resize(2,2);
    stats s(data);
    pair<int,int> ul(0,0);
    pair<int,int> lr(1,1);
    long result = s.rectArea(ul,lr);

    REQUIRE(result == 4);
    
}

TEST_CASE("stats::basic getAvg","[weight=1][part=stats]"){
    PNG data; data.resize(2,2);
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 2; j++){
            HSLAPixel * p = data.getPixel(i,j);
            p->h = 135*j + i * 90;
            p->s = 1.0;
            p->l = 0.5;
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(0,0);
    pair<int,int> lr(1,1);
    HSLAPixel result = s.getAvg(ul,lr);
    HSLAPixel expected(112.5,1.0, 0.5);

    REQUIRE(result == expected);
}

TEST_CASE("stats::basic entropy","[weight=1][part=stats]"){
    PNG data; data.resize(2,2);
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 2; j++){
            HSLAPixel * p = data.getPixel(i,j);
            p->h = 135*j + i * 90;
            p->s = 1.0;
            p->l = 0.5;
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(0,0);
    pair<int,int> lr(1,1);
    long result = s.entropy(ul,lr);

    REQUIRE(result == 2);
}

// TEST_CASE("toqutree::basic crop_func","[weight=1][part=toqutree]"){
//     PNG img;
//     img.readFromFile("images/stanleySquare.png");
//     printf("Inside");
//     toqutree t1(img,9);
//     printf("outside");


//     //REQUIRE(out==img);
// }

// TEST_CASE("toqutree::basic buildtree/Yaman","[weight=1][part=toqutree]"){
//     PNG img;
//     img.readFromFile("images/stanleySquare.png");
//     printf("Inside");
//     toqutree t1(img,9);
//     printf("outside");


//     //REQUIRE(out==img);
// }

TEST_CASE("stats::bryson wrapper","[weight=1][part=stats]"){
    PNG data; data.resize(8,8);
    for (int i = 0; i < 8; i ++){
        for (int j = 0; j < 8; j++){
            if(i == 0 || i == 1 || i == 6 || i == 7){
                if(j == 0 || j == 1 || j == 6 || j == 7){
                    HSLAPixel * p = data.getPixel(i,j);
                    p->h = 35.0;
                    p->s = 1.0;
                    p->l = 0.5;
                    p->a = 1.0;
                }
            }else{
            HSLAPixel * p = data.getPixel(i,j);
            p->h = 0.0;
            p->s = 0.0;
            p->l = 0.0;
            p->a = 1.0;
        }
        }
    }
    stats s(data);
    pair<int,int> ul(6,6);
    pair<int,int> lr(1,1);
    vector<int> result = s.getDistn(ul,lr);
    vector<int> expected(36); 
    expected[3] = 16;  

    REQUIRE(result == expected);
}

TEST_CASE("stats::bryson wrapper x >","[weight=1][part=stats]"){
    PNG data; data.resize(8,8);
    for (int i = 0; i < 8; i ++){
        for (int j = 0; j < 8; j++){
            if(i == 0 || i == 1 || i == 2 || i == 7){
                if(j == 2 || j == 3 || j == 4 || j == 5){
                    HSLAPixel * p = data.getPixel(i,j);
                    p->h = 35.0;
                    p->s = 1.0;
                    p->l = 0.5;
                    p->a = 1.0;
                }
            }else{
            HSLAPixel * p = data.getPixel(i,j);
            p->h = 0.0;
            p->s = 0.0;
            p->l = 0.0;
            p->a = 1.0;
        }
        }
    }
    stats s(data);
    pair<int,int> ul(7,2);
    pair<int,int> lr(2,5);
    vector<int> result = s.getDistn(ul,lr);
    vector<int> expected(36); 
    expected[3] = 16;  

    REQUIRE(result == expected);
}

TEST_CASE("stats::bryson wrapper2 x >","[weight=1][part=stats]"){
    PNG data; data.resize(8,8);
    for (int i = 0; i < 8; i ++){
        for (int j = 0; j < 8; j++){
            if(i == 0 || i == 5 || i == 6 || i == 7){
                if(j == 2 || j == 3 || j == 4 || j == 5){
                    HSLAPixel * p = data.getPixel(i,j);
                    p->h = 35.0;
                    p->s = 1.0;
                    p->l = 0.5;
                    p->a = 1.0;
                }
            }else{
            HSLAPixel * p = data.getPixel(i,j);
            p->h = 0.0;
            p->s = 0.0;
            p->l = 0.0;
            p->a = 1.0;
        }
        }
    }
    stats s(data);
    pair<int,int> ul(5,2);
    pair<int,int> lr(0,5);
    vector<int> result = s.getDistn(ul,lr);
    vector<int> expected(36); 
    expected[3] = 16;  

    REQUIRE(result == expected);
}

TEST_CASE("stats::bryson wrapper y >","[weight=1][part=stats]"){
    PNG data; data.resize(8,8);
    for (int i = 0; i < 8; i ++){
        for (int j = 0; j < 8; j++){
            if((i == 2 || i == 3 || i == 4 || i == 5) && (j == 0 || j == 1 || j == 2 || j == 7)){
                    HSLAPixel * p = data.getPixel(i,j);
                    p->h = 35.0;
                    p->s = 1.0;
                    p->l = 0.5;
                    p->a = 1.0;
            }else{
                    HSLAPixel * p = data.getPixel(i,j);
                    p->h = 0.0;
                    p->s = 0.0;
                    p->l = 0.0;
                    p->a = 1.0; 
            }
        }
    }
    stats s(data);
    pair<int,int> ul(2,7);
    pair<int,int> lr(5,2);
    vector<int> result = s.getDistn(ul,lr);
    vector<int> expected(36); 
    expected[3] = 16;  

    REQUIRE(result == expected);
}

TEST_CASE("stats::bryson wrapper2","[weight=1][part=stats]"){
    PNG data; data.resize(8,8);
    for (int i = 0; i < 8; i ++){
        for (int j = 0; j < 8; j++){
            if(((i >= 0 && i < 3) || i == 7) && ((j >= 0 && j < 3) || j == 7)){
                    HSLAPixel * p = data.getPixel(i,j);
                    p->h = 35.0;
                    p->s = 1.0;
                    p->l = 0.5;
                    p->a = 1.0;
            }else{
            HSLAPixel * p = data.getPixel(i,j);
            p->h = 0.0;
            p->s = 0.0;
            p->l = 0.0;
            p->a = 1.0;
        }
        }
    }

    stats s(data);
    pair<int,int> ul(7,7);
    pair<int,int> lr(2,2);
    vector<int> result = s.getDistn(ul,lr);
    vector<int> expected(36); 
    expected[3] = 16;  

    REQUIRE(result == expected);
}

TEST_CASE("toqutree::basic ctor render","[weight=1][part=toqutree]"){
    PNG img;
    img.readFromFile("images/stanleySquare.png");

    printf("Yaman was here");
    toqutree t1(img,9);
    printf("Bryson was here");
    
    int s = t1.size(); 
    printf("hello");
    printf("size %i: ",s);
    // PNG out = t1.render();
    // out.convert();
    REQUIRE(3 == 3);
    // REQUIRE(out==img);
}

TEST_CASE("toqutree::basic copy","[weight=1][part=toqutree]"){
    PNG img;
    img.readFromFile("images/geo.png");

    toqutree t1(img,5);
    toqutree t1copy(t1);

    PNG out = t1copy.render();

    REQUIRE(out==img);
}

TEST_CASE("toqutree::basic prune","[weight=1][part=toqutree]"){
    PNG img;
    img.readFromFile("images/ada.png");
    
    toqutree t1(img,9);

    t1.prune(0.05);
    PNG result = t1.render();

    PNG expected; expected.readFromFile("images/adaPrune.05.png");
    result.convert();

    REQUIRE(expected==result);

}
