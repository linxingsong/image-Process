//
//  main.cpp
//  proj2
//
//  Created by Eric Lin on 2/12/17.
//  Copyright © 2017 Xing song  Lin. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class corner_Preserving_filter{
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newMin;
    int newMax;
    int** mirrorFramedAry;
    int** tempAry;
    int neighborARY[9];
    double neighborAVG[8];
    
    corner_Preserving_filter(int row, int col, int min, int max){
        numRows = row;
        numCols = col;
        minVal = min;
        maxVal = max;
        newMin = max;
        newMax = min;
        
        tempAry = new int*[row+4];
        mirrorFramedAry = new int*[row+4];
        for(int i=0; i<row+4; i++){
            mirrorFramedAry[i] = new int[col+4];
            tempAry[i] = new int[col+4];
        }
        for(int i=0; i<row+4; i++){
            for(int j=0; j<col+4; j++){
                mirrorFramedAry[i][j] =0;
                tempAry[i][j] =0;
            }
        }
    }
    //step 3: process the mirrorframedAry begin at [1][1], copy data to top, bot, left, right side.
    void create_mirrorFramedAry(){
        for(int i=0; i<=numRows+2; i++){
            mirrorFramedAry[i][0] =mirrorFramedAry[i][2];
            mirrorFramedAry[i][1] =mirrorFramedAry[i][2];
            mirrorFramedAry[i][numCols+3] = mirrorFramedAry[i][numCols+1];
            mirrorFramedAry[i][numCols+2] = mirrorFramedAry[i][numCols+1];
        }
        for(int j=0; j<=numCols+3; j++){
            mirrorFramedAry[0][j] = mirrorFramedAry[2][j];
            mirrorFramedAry[1][j] = mirrorFramedAry[2][j];
            mirrorFramedAry[numRows+3][j] = mirrorFramedAry[numRows+1][j];
            mirrorFramedAry[numRows+2][j] = mirrorFramedAry[numRows+1][j];
        }
    }
    
    void loadneighbor(int group, int i, int j){
        int index = 0;
        switch(group) {
            case 0: //top-left corner
                for(int r = -2; r <= 0; r++) {
                    for(int c = -2; c <= 0; c++) {
                        neighborARY[index++] = mirrorFramedAry[i + r][j + c];
                    }
                }
                break;
                
            case 1: //top-right corner
                for(int r = -2; r <= 0; r++) {
                    for(int c = 0; c <= 2; c++) {
                        neighborARY[index++] = mirrorFramedAry[i + r][j + c];
                    }
                }
                break;
                
            case 2: //bottom-left corner
                for(int r = 0; r <= 2; r++) {
                    for(int c = -2; c <= 0; c++) {
                        neighborARY[index++] = mirrorFramedAry[i + r][j + c];
                    }
                }
                break;
                
            case 3: //bottom-right corner
                for(int r = 0; r <= 2; r++) {
                    for(int c = 0; c <= 2; c++) {
                        neighborARY[index++] = mirrorFramedAry[i + r][j + c];
                    }
                }
                break;
                
            case 4://left side
                for(int r = -2; r <= 2; r++) {
                    for(int c = -2; c <= r && c <= -r; c++) {
                        neighborARY[index++] = mirrorFramedAry[i + r][j + c];
                    }
                }
                break;
                
            case 5://right side
                for(int r = -2; r <= 2; r++) {
                    for(int c = 2; c >= -r && c >= r; c--) {
                        neighborARY[index++] = mirrorFramedAry[i + r][j + c];
                    }
                }
                break;
                
            case 6://top side
                for(int r =- 2;  r<= 0; r++) {
                    for(int c = r; c <= -r; c++) {
                        neighborARY[index++] = mirrorFramedAry[i + r][j + c];
                    }
                }
                break;
                
            case 7://bottom side
                for(int r=0; r<=2; r++) {
                    for(int c=-r; c<= r; c++) {
                        neighborARY[index++] = mirrorFramedAry[i + r][j + c];
                    }
                }
                break;	
        }
    }
    
    double computeAVG(){  //compute 8 group avg and store to neighborAVG ary.
        double sum = 0;
        for(int i = 0 ; i <9; i++) {
            sum += neighborARY[i];
        }
        return sum / 9;
    }
    
    int minAVG(){
        int current_min=neighborAVG[0];
        for(int i=0; i<8; i++){
            if(current_min>neighborAVG[i]){
                current_min = neighborAVG[i];
            }
        }
        return current_min;
    }
    
    void findNewMinMax(){
        for(int i=2; i<numRows+2; i++){
            for(int j=2; j<numCols+2; j++){
                if(newMin>tempAry[i][j]){
                    newMin= tempAry[i][j];
                }
                if(newMax < tempAry[i][j]){
                    newMax = tempAry[i][j];
                }
            }
        }
    }
    
    void outPutImage(ofstream &outfile){
        //output the header of output and result of temAry from tempAry[1][1]
        outfile<<numRows<<" "<<numCols<<" "<<newMin<<" "<<newMax<<endl;
        for(int i=2; i<numRows+2; i++){
            for(int j=2; j<numCols+2; j++){
                outfile<<tempAry[i][j]<<" ";
            }
            outfile<<" \n";
        }
    }
};

int main(int argc, const char * argv[]) {
    //step 0: open the input file and output file.
    ifstream infile(argv[1]);
    ofstream outfile(argv[2]);
    
    if(infile.is_open()){
        //stpe 1: load image
        int numRows, numCols, minVal, maxVal, data;
        infile>>numRows>>numCols>>minVal>>maxVal;
        //construct all requirement data stracture.
        corner_Preserving_filter *cpf = new corner_Preserving_filter(numRows, numCols, minVal, maxVal);
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                infile>>data;
                cpf->mirrorFramedAry[i+2][j+2] = data;  //begin at [2,2], and store the value to mirroframedAry.
            }
        }
      
        //step 2: mirrowFramed the mirrorFramedAry
        cpf->create_mirrorFramedAry();
        //for(int i=0; i<numRows+4; i++){
        //    for(int j =0; j<numCols+4; j++){
        //        cout<< cpf->mirrorFramedAry[i][j]<< " ";
        //    }
        //    cout<<endl;
        //}

        //step 4: which <--0, name it group
        //step 7: repeat while gourp <9
            //step 5: loadNeighbors(grou, i j)
            //        computeAVG(which, ...)
        //step 9: preocess all pixels
        for(int i=2; i<numRows+2; i++){
            for(int j =2; j<numCols+2; j++){
                int group=0;
                while(group<8){
                    cpf->loadneighbor(group, i, j);
                    //step 6: which++, computer neighborAVG base on neighborARY.
                    cpf->neighborAVG[group] = cpf->computeAVG();
                    //step8: tempAVG[i,j]<-minAVG(neighborAVG)
                    group++;
                }
                  cpf->tempAry[i][j] = cpf->minAVG();
            }
        }
        
        //step 10: findNewMinMax
        cpf->findNewMinMax();
        //step 11: outputImage with new min and max
        cpf->outPutImage(outfile);
        
    }
    else{
        cout<< "Fail to open file! No such file exist!"<<endl;
    }
    
    //step 12: close all file.
    infile.close();
    outfile.close();
    return 0;
}
