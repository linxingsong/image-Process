//
//  main.cpp
//  proj4
//
//  Created by Eric Lin on 2/26/17.
//  Copyright © 2017 Xing song  Lin. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class eightDistance{
private:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newMin;
    int newMax;
public:
    int tempMax;
    int** zeroFramedAry;
    int** skeltonAry;
    int skeltonNeighbor[9];
    int neighborAry[5];
    eightDistance(int row, int col, int min, int max){
        numRows=row;
        numCols=col;
        minVal=min;
        maxVal=max;
        newMin=min;
        tempMax = 0;
        newMax=0;
        zeroFramedAry = new int*[row+2];
        skeltonAry = new int*[row+2];
        for(int i=0; i<row+2; i++){
            zeroFramedAry[i] = new int[col+2];
            skeltonAry[i] = new int[col+2];
        }
        for(int i=0; i<row+2; i++){
            for(int j=0; j<col+2; j++){
                zeroFramedAry[i][j]=0;
                skeltonAry[i][j]=0;
            }
        }
    }
    
    void mirrorFramed(eightDistance *ed){
        for(int i = 0; i <= ed->numRows+1; i++ ){// framing left and right
            ed->zeroFramedAry[i][0]=0;
            ed->zeroFramedAry[i][numCols+1] = 0;
        }
        for(int j = 0; j <= ed->numCols+1; j++){  // framing top and bottom
            ed->zeroFramedAry[0][j] =0;
            ed->zeroFramedAry[numRows+1][j] = 0;
        }
    }
    
    void loadNeighbor_Pass1(int row, int col){
        neighborAry[0]= zeroFramedAry[row-1][col-1];
        neighborAry[1]= zeroFramedAry[row-1][col];
        neighborAry[2]= zeroFramedAry[row-1][col+1];
        neighborAry[3]= zeroFramedAry[row][col-1];
    }
    
    void loadNeighbor_Pass2(int row, int col){
        neighborAry[0]= zeroFramedAry[row+1][col+1];
        neighborAry[1]= zeroFramedAry[row+1][col];
        neighborAry[2]= zeroFramedAry[row+1][col-1];
        neighborAry[3]= zeroFramedAry[row][col+1];
        neighborAry[4]= zeroFramedAry[row][col];
    }
    
    void pass1(eightDistance *ed){
        for(int i =1; i<= ed->numRows; i++){
            for(int j = 1;j<= ed->numCols;j++){
                if(ed->zeroFramedAry[i][j] > 0){
                    ed->loadNeighbor_Pass1(i,j);
                    ed->zeroFramedAry[i][j] =min(ed->neighborAry[0],min(ed->neighborAry[1],min(ed->neighborAry[2], ed->neighborAry[3])))+1;
                }
            }
        }
    }
    void pass2(eightDistance *ed){
        for(int i = ed->numRows; i>=1; i--){
            for(int j = ed->numCols;j>=1;j--){
                if(ed->zeroFramedAry[i][j] > 0){
                    ed->loadNeighbor_Pass2(i,j);
                    ed->zeroFramedAry[i][j] =min(ed->neighborAry[4],min(min(ed->neighborAry[1]+1,
                                                                              ed->neighborAry[2]+1),min(ed->neighborAry[3]+1,ed->neighborAry[0]+1)));
                }
                if (ed->zeroFramedAry[i][j]<newMin) //keep tracking the newMin
                    ed->newMin = ed->zeroFramedAry[i][j];
                if (ed->zeroFramedAry[i][j] >newMax) // keep tracking the newMax
                    ed->newMax = ed->zeroFramedAry[i][j];
            }				
        }			
    }

    void prettyPrint(ofstream &outfile, eightDistance *ed, int num){
        for(int i=1; i<=ed->numRows+1; i++){
            for(int j=1; j<=ed->numCols+1; j++){
                switch(num){
                case 1:
                    if(ed->zeroFramedAry[i][j]>0){
                        if(ed->zeroFramedAry[i][j]>=10){
                            outfile<<ed->zeroFramedAry[i][j];
                        }
                        else{
                            outfile<<ed->zeroFramedAry[i][j]<<" ";
                        }
                    }
                    else{
                        outfile<<"  ";
                        }
                        break;
                case 2:
                    if(ed->skeltonAry[i][j]>0){
                        if(ed->skeltonAry[i][j]>=10){
                            outfile<<ed->skeltonAry[i][j];
                        }
                        else{
                            outfile<<ed->skeltonAry[i][j]<<" ";
                        }
                    }
                    else{
                        outfile<<"  ";
                    }
                        break;
            }
            }
            outfile<<"\n";
        }
    }

    void printImage(ofstream &outfile, eightDistance *ed, int num){
        if (outfile.is_open()){
            outfile << ed->numRows << " " << ed->numCols << " " << ed->newMin << " " << ed->newMax << "\n";
            switch(num){
                case 1:
                for (int i = 1; i <=ed->numRows; i++){
                    for (int j = 1; j <=ed->numCols; j++){
                        if(ed->zeroFramedAry[i][j]>=10)
                            outfile<<ed->zeroFramedAry[i][j];
                        else
                            outfile<<ed->zeroFramedAry[i][j]<<" ";
                    }
                    outfile << "\n";
                }
                break;
                case 2:
                    for (int i = 1; i <=ed->numRows; i++){
                        for (int j = 1; j <=ed->numCols; j++){
                            if(ed->skeltonAry[i][j]>=10)
                                outfile<<ed->skeltonAry[i][j];
                            else
                                outfile<<ed->skeltonAry[i][j]<<" ";
                        }
                        outfile << "\n";
                }
                break;
            }
        }
        else cout << "unable to open file";
    }	
    
    void loadSkeltonNeighbor(int row, int col){
        skeltonNeighbor[0] = zeroFramedAry[row][col];
        skeltonNeighbor[1] = zeroFramedAry[row-1][col-1];
        skeltonNeighbor[2] = zeroFramedAry[row-1][col];
        skeltonNeighbor[3] = zeroFramedAry[row-1][col+1];
        skeltonNeighbor[4] = zeroFramedAry[row][col-1];
        skeltonNeighbor[5] = zeroFramedAry[row][col+1];
        skeltonNeighbor[6] = zeroFramedAry[row+1][col-1];
        skeltonNeighbor[7] = zeroFramedAry[row+1][col];
        skeltonNeighbor[8] = zeroFramedAry[row+1][col+1];
    }
    
    int findMaxFromSkeltonNeighbor(eightDistance *ed){
        int max = 0;
        for(int i=1; i<9; i++){
            if(ed->skeltonNeighbor[i]>=max){
                max = ed->skeltonNeighbor[i];
            }
        }
        return max;
    }
    
    void computeSkeleton(eightDistance *ed){
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                ed->loadSkeltonNeighbor(i, j);
                ed->tempMax = findMaxFromSkeltonNeighbor(ed);
                if(ed->zeroFramedAry[i][j]>0 && ed->zeroFramedAry[i][j]>=ed->tempMax){
                    ed->skeltonAry[i][j] = ed->zeroFramedAry[i][j];
                    //cout<<ed->tempMax<<" "<<ed->zeroFramedAry[i][j]<<endl;
                }
                else{
                    ed->skeltonAry[i][j]=0;
                }
            }
        }
    }
};

int main(int argc, const char * argv[]) {
    ifstream input(argv[1]);
    ofstream output1(argv[2]);
    ofstream output2(argv[3]);
    ofstream output3(argv[4]);
    
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int data;
    
    if(input.is_open()){
        input>>numRows>>numCols>>minVal>>maxVal;
        eightDistance *ed = new eightDistance(numRows, numCols, minVal, maxVal);
        for(int i=0; i<numRows; i++){
            for(int j=0; j< numCols;j++){
                input>>data;
                ed->zeroFramedAry[i+1][j+1] = data;
            }
        }
        //cout<<"1\n";
        ed->mirrorFramed(ed);
        //cout<<"2\n";
        ed->pass1(ed);
        output3<<"Pass: 1 \n";
        ed->prettyPrint(output3, ed, 1);
        //cout<<"3\n";
        ed->pass2(ed);
        //cout<<"4\n";
        output3<<"Pass: 2 \n";
        ed->prettyPrint(output3, ed, 1);
        //cout<<"5\n";
        ed->printImage(output1, ed, 1);
        //cout<<"6\n";
        ed->computeSkeleton(ed);
        //cout<<"7\n";
        output3<<"Skeleton: \n";
        ed->prettyPrint(output3, ed, 2);
        //cout<<"8\n";
        ed->printImage(output2, ed, 2);
    }
    else{
        cout<<"Unable to open file.\n";
    }
    
    input.close();
    output1.close();
    output2.close();
    output3.close();
    return 0;
}
