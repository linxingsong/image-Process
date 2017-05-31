package proj8;

import java.util.Scanner;

public class image{
	public
	int numRows, numCols, minVal, maxVal;
	int imgAry[][];
	
	public image(int r, int c, int min, int max){
	    numRows = r;
	    numCols = c;
	    minVal = min;
	    maxVal = max;
	    imgAry = new int[r][c];
	    for(int i=0; i<numRows; i++){
			for(int j=0; j<numCols; j++){
				imgAry[i][j] =0;
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
	            imgAry[i][j] = data;
	            if(input.hasNext()){
					data= input.nextInt();
				}
	        }
	    }
	}
}
