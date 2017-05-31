//
//  main.cpp
//  proj9.1
//
//  Created by Eric Lin on 4/15/17.
//  Copyright © 2017 XingSong  Lin. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class image{
public:
    int count, numRows, numCols, minVal, maxVal;
    int** imgAry;
    
    image(int r, int c, int min, int max){
        numRows = r;
        numCols = c;
        minVal = min;
        maxVal = max;
        count =0;
        imgAry = new int*[numRows];
        for(int i=0; i<numRows; i++){
            imgAry[i]= new int[numCols];
        }
        
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                imgAry[i][j] =0;
            }
        }
    }
    
    void loadImage(ifstream &input){
        int data;
        for(int i=0; i<numRows; i++){
            for(int j=0; j< numCols;j++){
                input>>data;
                if(data>0){
                    count++;
                }
                imgAry[i][j] = data;
                
            }
        }
    }
    
    void findPoint(ofstream &output){
        output<<count<<"\n"<<numRows<<" "<<numCols<<"\n";
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                if(imgAry[i][j]>0){
                    output<<i<<" "<<j<<endl;
                }
            }
        }
    }
    
};

int main(int argc, const char * argv[]) {
    ifstream infile(argv[1]);
    ofstream outfile(argv[2]);
    int numRows, numCols, minVal, maxVal;
    infile>>numRows>>numCols>>minVal>>maxVal;
    
    image img =*new image(numRows, numCols, minVal, maxVal);
    
    img.loadImage(infile);
    img.findPoint(outfile);
    
    
    infile.close();
    outfile.close();
    return 0;
}
