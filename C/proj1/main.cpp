//
//  main.cpp
//  proj1
//
//  Created by Eric Lin on 3/1/17.
//  Copyright © 2017 Xing song  Lin. All rights reserved.
//

#include <iostream>
#include <fstream>

using namespace std;

void prettyPrint(const char *argv1, const char *argv2){
    //step 0: open input file and output file
    ifstream input;
    ofstream output;
    
    input.open(argv1);
    output.open(argv2);
    if(!input.is_open()){
        cout<<"Fail to open the file!"<<endl;
    }
    
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int pixel_val;
    // int thr_value;
    //cout<<"Enter a thr_value you choose: "<<endl;
    //cin>>thr_value;
    
    //step 1: read the image header from input file, the four numbers.
    input>>numRows>>numCols>>minVal>>maxVal;
    //output<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
    
    //step 2: process input file from left to right , top to buttom
    for(int i=0; i<numRows; i++){
        for(int j=0; j<numCols; j++){
            //step 3: repeat step 2 until input file is empty
            input>>pixel_val;
            if(pixel_val > 0){
                output<<pixel_val<<" ";
            }
            else{
                output<<"  ";
            }
        }
        output<<"\n";
    }
    
    //step 4: close all files.
    input.close();
    output.close();
    
}

int main(int argc, const char *argv[]) {
    
    prettyPrint(argv[1], argv[2]);
    
    return 0;
}
