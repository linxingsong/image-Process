//
//  main.cpp
//  proj10
//
//  Created by Eric Lin on 5/2/17.
//  Copyright © 2017 XingSong  Lin. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class decomposition{
    public :
    int numRows, numCols, minVal, maxVal, cycleCount;
    bool changeFlag;
    int** objectAry;
    int** firstAry;
    int** secondAry;
    
    decomposition(int r, int c, int min, int max){
        numRows = r;
        numCols = c;
        minVal = min;
        maxVal = max;
        cycleCount =0;
        changeFlag = true;
        
        // dynamiclly allocate object, first and second array
        objectAry = new int*[numRows+2];
        firstAry = new int*[numRows+2];
        secondAry = new int*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            objectAry[i]= new int[numCols+2];
            firstAry[i]= new int[numCols+2];
            secondAry[i]= new int[numCols+2];
        }
    }
    
    void zeroFramed(int **array){
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                array[i][j] =0;
            }
        }
    }
    
    void loadImage(ifstream &input, int** ary){
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols;j++){
                int data;
                input >> data;
               // printf("%2d", data);
                ary[i][j] = data;
            }
           // printf("\n");
        }
    }
    
    void process(ofstream &outfile2){
        //step10
        while(changeFlag){
            //step 5
            if(cycleCount==0 || cycleCount ==3 || cycleCount ==5){
                outfile2<<"=============================================="<<endl;
                outfile2<<"For "<<cycleCount<<": "<<endl;
                PrettyPrint(outfile2,firstAry);
            }
            //step 4
            changeFlag = false;
            cycleCount++;
            //step 6
            northExpansion();
            copyAry();
            //step 7
            southExpansion();
            copyAry();
            //step 8
            westExpansion();
            copyAry();
            //step 9
            eastExpansion();
            copyAry();
        }
    }
    
    void copyAry(){
        for(int i = 1; i <= numRows; i++){
            for(int j = 1; j <= numCols; j++){
                if(secondAry[i][j] > 0)
                    firstAry[i][j] = secondAry[i][j];
            }
        }
    }
    
    void northExpansion(){
        for(int i = 1; i <= numRows; i++){
            for(int j =1; j <= numCols; j++){
                if(firstAry[i][j] > 0 && firstAry[i-1][j] == 0){
                    doExpansion(i-1,j, firstAry[i][j]);
                }
            }
        }
    }
    
     void southExpansion(){
        for(int i = 1; i <= numRows; i++)
            for(int j =1; j <= numCols; j++){
                if(firstAry[i][j] > 0 && firstAry[i+1][j] == 0)
                    doExpansion(i+1,j, firstAry[i][j]);
        }
    }
    
     void westExpansion(){
        for(int i = 1; i <= numRows; i++)
            for(int j =1; j <= numCols; j++)
                if(firstAry[i][j] > 0 && firstAry[i][j-1] == 0)
                     doExpansion(i,j-1, firstAry[i][j]);
    }
    
    void eastExpansion(){
        for(int i = 1; i <= numRows; i++)
            for(int j =1; j <= numCols; j++)
                if(firstAry[i][j] > 0 && firstAry[i][j+1] == 0)
                     doExpansion(i,j+1, firstAry[i][j]);
    }
    
    void doExpansion(int row, int col, int label){
        if(objectAry[row][col]>0){
            secondAry[row][col] = label;
            changeFlag = true;
        }
    }
    
    void PrettyPrint(ofstream &output ,int **ary){
        for (int i = 1; i <= numRows; i++){
            for (int j = 1; j <= numCols; j++){
                if(ary[i][j] > 0){
                    output << ary[i][j] << " ";
                }
                else{
                    output << "  ";
                }
            }
            output << "\n";
        }
    }
    
    void finalImg(ofstream &output){
        for(int i=1; i<= numRows; i++){
            for(int j=1; j<= numCols; j++){
                output<<firstAry[i][j]<<" ";
            }
            output<<endl;
        }
    }
};


int main(int argc, const char * argv[]) {
    //step 1: open file
    ifstream infile;
    infile.open(argv[1]);
    ifstream infile2;
    infile2.open(argv[2]);
    ofstream outfile1;
    outfile1.open(argv[3]);
    ofstream outfile2;
    outfile2.open(argv[4]);
    
    int row, col, min, max;
    infile>>row>>col>>min>>max;
    infile2>>row>>col>>min>>max;
    decomposition  deImg(row, col, min, max);
    deImg.zeroFramed(deImg.objectAry);
    deImg.zeroFramed(deImg.firstAry);
    deImg.zeroFramed(deImg.secondAry);
    

    
    //step 2: scan file

    deImg.loadImage(infile, deImg.objectAry);
    deImg.loadImage(infile2, deImg.firstAry);
    //step 3
    deImg.PrettyPrint(outfile2, deImg.objectAry);
    //step 10
    deImg.process(outfile2);

    outfile2<<"=============================================="<<endl;
    outfile2<<"Final: "<<endl;
    deImg.PrettyPrint(outfile2,deImg.firstAry);
    //step 12
    outfile1<<deImg.numRows<<" "<<deImg.numCols<<" "<<deImg.minVal<<" "<<deImg.maxVal<<endl;
    deImg.finalImg(outfile1);
    //step 13: close file
    infile.close();
    infile2.close();
    outfile1.close();
    outfile2.close();
    return 0;
}
