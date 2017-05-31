
//
//  main.cpp
//  proj1.3
//
//  Created by Eric Lin on 2/5/17.
//  Copyright © 2017 Xing song  Lin. All rights reserved.
//

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void constructor(int data, int i, int j, int **thr_Ary){
    thr_Ary[i][j] = data;
}

void computeThreshold(int thre_val, int numRows, int numCols, int **thr_Ary, const char *argv1){
    string thre_val_string = to_string(thre_val);
    string infilename = argv1;
    string ss = infilename.substr(0, infilename.length()-4) + "_"+thre_val_string + ".txt";
    ofstream outfile(ss);
    outfile<<numRows << " " << numCols << " " << 0 << " " << 1<<"\n";

    for(int i=0; i<numRows; i++){
        for(int j=0; j<numCols; j++){
            if(thr_Ary[i][j]<thre_val){ // compare the data with the user input
                //cout<<0 << " ";
                outfile<<0 << " ";
            }
            else{
                //cout<<1 << " ";
                outfile<<1 << " ";
            }
        }
        outfile<<"\n";
    }
        outfile.close();
}


int main(int argc, char** argv){
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    
    ifstream infile(argv[1]);
    infile>>numRows;
    infile>>numCols;
    infile>>minVal;
    infile>>maxVal;
    
    int** thr_Ary;
    thr_Ary = new int*[numRows];
    for(int i = 0; i<numRows; i++){
        thr_Ary[i]= new int[numCols];
    }
    for(int i = 0;i<numRows;i++){
        for(int j =0;j<numCols;j++){
            thr_Ary[i][j] = 0;
        }
    }
    
    int thre_val;
    cout<<"Enter a thresholding value: ";
    cin>>thre_val;
    
    if(infile.is_open()){
        //cout<<numRows << " " << numCols << " " << 0 << " " << 1<<"\n";
        int data;
        for(int i = 0; i<numRows;i++){
            for(int j = 0;j<numCols;j++){
                infile>>data;
                //cout<<data<<" ";
                constructor(data,i, j,thr_Ary);
            }
        }
        infile.close();
    }
    else{
        cout<<"unable to open file";
    }
    
    computeThreshold(thre_val, numRows, numCols, thr_Ary, argv[1]);
    
    string tempifname = argv[1];
    string temp =  tempifname.substr(0, tempifname.length()-4)+"_"+to_string(thre_val) + ".txt";
    string out = tempifname.substr(0, tempifname.length()-4) +"_"+ to_string(thre_val) + "PP.txt";
    ifstream infile1(temp);
    ofstream outfile1(out);
    infile1>>numRows;
    infile1>>numCols;
    infile1>>minVal;
    infile1>>maxVal;
    int pixel_val;
    for(int i=0; i<numRows; i++){
        for(int j=0; j<numCols; j++){
            //step 3: repeat step 2 until input file is empty
            if(infile1>>pixel_val && pixel_val > 0){
                outfile1<<pixel_val<<" ";
            }
            else{
                outfile1<<"  ";
            }
        }
        outfile1<<"\n";
    }
        infile1.close();
        outfile1.close();
    return 0;
}
