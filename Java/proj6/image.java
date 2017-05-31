package proj6;

import java.util.Scanner;

public class image{
	public
	int numRows, numCols, minVal, maxVal;
	int zeroFramedAry[][];
	
	public image(int r, int c, int min, int max){
	    numRows = r;
	    numCols = c;
	    minVal = min;
	    maxVal = max;
	    zeroFramedAry = new int[r+2][c+2];
	    for(int i=0; i<numRows+2; i++){
			for(int j=0; j<numCols+2; j++){
				zeroFramedAry[i][j] =0;
			}
		}
	}
	
	void zeroFramed(int[][] array){
	    for(int i=0; i<numRows+2; i++){
	        for(int j=0; j<numCols+2; j++){
	            array[i][j] =0;
	        }
	    }
	}
	
	void loadImage(Scanner input){
	    int data = input.nextInt();
	    for(int i=0; i<numRows; i++){
	        for(int j=0; j< numCols;j++){
	            zeroFramedAry[i+1][j+1] = data;
	            if(input.hasNext()){
					data= input.nextInt();
				}
	        }
	    }
	}
}
