//
//  main.cpp
//  proj6
//
//  Created by Eric Lin on 3/11/17.
//  Copyright © 2017 XingSong  Lin. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class image{
public:
    int numRows, numCols, minVal, maxVal;
    int** zeroFramedAry;
    
    image(int r, int c, int min, int max){
        numRows = r;
        numCols = c;
        minVal = min;
        maxVal = max;
        zeroFramedAry = new int*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            zeroFramedAry[i]= new int[numCols+2];
        }
    }
    
    void zeroFramed(int **array){
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                array[i][j] =0;
            }
        }
    }
    
    void loadImage(ifstream &input){
        int data;
        for(int i=0; i<numRows; i++){
            for(int j=0; j< numCols;j++){
                input>>data;
                zeroFramedAry[i+1][j+1] = data;
            }
        }
    }
};

struct point{
    int row;
    int col;
};

class chainCode{
public:
    point neighborCoord[8];
	point startP;
    point currentP;
    point nextP;
    int lastQ, nextQ;
    int nextDirTable[8]={6,6,0,0,2,2,4,4};
    int nextDir;
    int PChain;

    
	chainCode(image img, ofstream &outfile1, ofstream &outfile2){
		//step 3: if image(r,c)>0 ....
        for(int i=0; i<img.numRows+2; i++){
			for(int j=0; j<img.numCols+2; j++){
				//declare first position, start point.
				if(img.zeroFramedAry[i][j]>0){
					outfile1<<"Start Row；"<<i<<" Start Col: "<<j<<" Label Value: "<<img.zeroFramedAry[i][j];
                    outfile2<< i<<" "<<j<<" "<<img.zeroFramedAry[i][j]<<" ";
					startP.row = i;
					startP.col = j;
					currentP.row = i;
					currentP.col = j;
					lastQ = 4;
					//outfile1<<" lastQ: "<<lastQ<<" \n";
                    //outfile2<<lastQ<<" \n";
                    goto JUMP;
				}
			}
        }
        JUMP: ;
        //step 4
        doChainCoding(img, outfile1, outfile2);
        cout<<endl;
        PrettyPrint(img);
    }
	
    void doChainCoding(image img, ofstream &outfile1, ofstream &outfile2){
        //step 8: repeat
        int count =0;
        while(!equal(nextP, startP)){
            //step 4: nextQ
            nextQ = (lastQ+1)%8;
            //outfile1<<"nextQ: "<<nextQ<<" ";
            //outfile2<<nextQ<<" ";
            //step 5: Pchain
            PChain = findNextP(currentP, nextQ, img);
            //step 6: output
            outfile1<<"PChain: "<<PChain<<" ";
            if(count%20 ==0){
                outfile2<<"\n";
            }
            outfile2<<PChain<<" ";
            //step 7: lastQ
            lastQ = nextDirTable[PChain];
            //outfile1<<"lastQ: "<<lastQ<<" ";
            //outfile2<<lastQ<<" ";
            currentP.row = nextP.row;
            currentP.col = nextP.col;
            //outfile1<<"currentP: "<<currentP.row<<" "<<currentP.col<<" ";
            //outfile2<<currentP.row<<" "<<currentP.col<<" \n";
            count++;
        }
    }
    
    bool equal(point a, point b){ // function check is two point are the same{
		if(a.row == b.row && a.col == b.col){
			return true;
		}
		else
			return false;
	}
	
	int findNextP(point current, int nextQ, image image){ // the function of find next point of chain code
		//step 5.1: loadNeighborsCoord
        loadNeighborsCoord(current);
		int chainDir;
        //step 5.2: chainDir
		while(image.zeroFramedAry[neighborCoord[nextQ].row][neighborCoord[nextQ].col]!= 1){
			nextQ = (nextQ+1)%8;
		}
        //step 5.3: nextP
		chainDir = nextQ;
		nextP.row = neighborCoord[chainDir].row;
		nextP.col = neighborCoord[chainDir].col;
        //step 5.4: return
		return chainDir;
	}
	
	void loadNeighborsCoord(point curr){ // load neighbor{
		neighborCoord[0].row = curr.row;
		neighborCoord[0].col = curr.col+1;
		neighborCoord[1].row = curr.row-1;
		neighborCoord[1].col = curr.col+1;
		neighborCoord[2].row = curr.row-1;
		neighborCoord[2].col = curr.col;
		neighborCoord[3].row = curr.row-1;
		neighborCoord[3].col = curr.col-1;
		neighborCoord[4].row = curr.row;
		neighborCoord[4].col = curr.col-1;	
		neighborCoord[5].row = curr.row+1;
		neighborCoord[5].col = curr.col-1;
		neighborCoord[6].row = curr.row+1;
		neighborCoord[6].col = curr.col;
		neighborCoord[7].row = curr.row+1;
		neighborCoord[7].col = curr.col+1;
	}
	
	void PrettyPrint(image image){
		for (int i = 1; i <= image.numRows; i++){
			for (int j = 1; j <= image.numCols; j++){
				if(image.zeroFramedAry[i][j] > 0){
					cout << image.zeroFramedAry[i][j] << " ";
					}
				else{
                    cout << "  ";
				}
			}
			cout << "\n";
		}
	}
};

int main(int argc, const char * argv[]) {
    //step 1: open file
    ifstream infile;
    infile.open(argv[1]);
    ofstream outfile1;
    outfile1.open(argv[2]);
    ofstream outfile2;
    outfile2.open(argv[3]);
    
    int row, col, min, max;
    infile>>row>>col>>min>>max;
    outfile1<<"#row: "<<row<<" #col: "<<col<<" minVal: "<<min<<" maxVal: "<<max<<endl;
    outfile2<<row<<" "<<col<<" "<<min<<" "<<max<<endl;
    image  img = *new image(row, col, min, max);
    img.zeroFramed(img.zeroFramedAry);
    //step 2: scan file
    img.loadImage(infile);
    chainCode chainC = *new chainCode(img, outfile1, outfile2);
    //step 9: close file
    infile.close();
	outfile1.close();
	outfile2.close();
    return 0;
}
