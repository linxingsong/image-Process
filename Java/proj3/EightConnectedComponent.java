package proj3;

import java.io.*;
import java.util.*;

public class EightConnectedComponent {
	public int numRows;
	public int numCols;
	public int minVal;
	public int maxVal;
	public int newMin;
	public int newMax;
	public int newLabel;
	public int trueLabel=0;
	public int zeroFramedARY[][];
	public int[] neighborARY; //0,1,2,3,4
	public int EQAry[];
	public property propertyARY[];
	public int count;
	public EightConnectedComponent(int row, int col, int min, int max){
		numRows = row;
		numCols = col;
		minVal = min;
		maxVal = max;
		newMax = 0;
		newMin = min;
		zeroFramedARY = new int[row+2][col+2];
		for(int i=0; i<row+2; i++){
			for(int j=0; j<col+2; j++){
				zeroFramedARY[i][j]=0;
			}
		}
		EQAry = new int[(row*col)/2];
		for(int i=0; i<(row*col)/2; i++){
			EQAry[i] = i;
		}
	}
	
	public void mirrorFramed(){
		for(int i = 0; i <= numRows+1; i++ ){// framing left and right
			zeroFramedARY[i][0]=0;
			zeroFramedARY[i][numCols+1] = 0;
		}
		for(int j = 0; j <= numCols+1; j++){  // framing top and bottom
			zeroFramedARY[0][j] =0;
			zeroFramedARY[numRows+1][j] = 0;
		}
	}
	//need to finished below
	public void loadNeighbor_Pass1(int row, int col){
		neighborARY = new int[4];
		neighborARY[0]= zeroFramedARY[row-1][col-1];
		neighborARY[1]= zeroFramedARY[row-1][col];
		neighborARY[2]= zeroFramedARY[row-1][col+1];
		neighborARY[3]= zeroFramedARY[row][col-1];
	}
	
	public void loadneighbor_Pass2(int row, int col){
		neighborARY = new int[5];
		neighborARY[0]= zeroFramedARY[row+1][col+1];
		neighborARY[1]= zeroFramedARY[row+1][col];
		neighborARY[2]= zeroFramedARY[row+1][col-1];
		neighborARY[3]= zeroFramedARY[row][col+1];
		neighborARY[4]= zeroFramedARY[row][col];
	}
	
	public int countLabel(){
		int count=0;
		for(int m=0; m<neighborARY.length-1; m++){
			if(neighborARY[m] != neighborARY[m+1]){
				count++;
			}
		}
		return count;
	}
	
	public void E_ConnectCC_pass1(EightConnectedComponent ecc){
		for(int i=1; i<=ecc.numRows; i++){
			for(int j=1; j<=ecc.numCols; j++){
				if(ecc.zeroFramedARY[i][j]>0){
					ecc.loadNeighbor_Pass1(i, j);  //load neightborARY for pass1
					Arrays.sort(ecc.neighborARY);
					ecc.count = ecc.countLabel();
					//ecc.removeDuplicated();
					if(ecc.count>=2){
						ecc.count=2;
					}
					switch(ecc.count){
						//case 1: all 0, newlabel ++
						case 0:
							if(ecc.neighborARY[0]==0){
								ecc.newLabel++;
								//ecc.EQAry[ecc.zeroFramedARY[i][j]] = ecc.newLabel;
								ecc.zeroFramedARY[i][j] = ecc.newLabel;
								break;
							}
							else {
								//ecc.EQAry[ecc.zeroFramedARY[i][j]] = ecc.neighborARY[0];
								ecc.zeroFramedARY[i][j] = ecc.neighborARY[0];
								break;
							}

						//case 2: some non-zero with all same label
						case 1:
							int temp = 9999999;
							for(int t=0; t<ecc.neighborARY.length; t++){
								if(ecc.neighborARY[t]!=0){
									temp = Math.min(temp, ecc.neighborARY[t]);
								}
							}
							//ecc.EQAry[ecc.zeroFramedARY[i][j]] = temp;
							ecc.zeroFramedARY[i][j] = temp;
							break;
							//case 3: have at least 3 different value in neighborARY[]
						case 2:
							int temp1 = 999999;
							for(int t=0; t<ecc.neighborARY.length; t++){
								if(ecc.neighborARY[t]!=0){
									temp1 = Math.min(temp1,ecc.neighborARY[t]);
								}
							}
							//ecc.EQAry[ecc.zeroFramedARY[i][j]] = temp1;
							ecc.updateEQAry(temp1);
							ecc.zeroFramedARY[i][j] = temp1;
							break;
					}
					ecc.count=0;
				}
			}
		}
	}
	
	public void E_ConnectCC_pass2(EightConnectedComponent ecc){
		for(int i= ecc.numRows; i>=1; i--){
			for(int j = ecc.numCols; j>=1; j--){
				if(ecc.zeroFramedARY[i][j]>0){
					ecc.loadneighbor_Pass2(i, j);
					Arrays.sort(ecc.neighborARY);
					//case 1: do nothing if all 0 or same lable in the neighborARY[].
					//case 2: same label, do nothing
					//case 3: 2 or more lable in neighborARY, p(i,j) = min(enighborary[]), UPDATE EQtable
					ecc.count = ecc.countLabel();
					if(ecc.count>=2){
						ecc.count=2;
					}
					switch (ecc.count){
						case 0:
							if(ecc.neighborARY[0]!=0){
								//ecc.EQAry[ecc.zeroFramedARY[i][j]] = ecc.neighborARY[0];
								ecc.zeroFramedARY[i][j] = ecc.neighborARY[0];
								break;
							}
							else{
								//ecc.EQAry[ecc.zeroFramedARY[i][j]] = ecc.neighborARY[0];
								ecc.zeroFramedARY[i][j] = ecc.neighborARY[0];
							}
						case 1:
							int temp = 9999999;
							for(int t=0; t<ecc.neighborARY.length; t++){
								if(ecc.neighborARY[t]!=0){
									temp = Math.min(temp, ecc.neighborARY[t]);
								}
							}
							//ecc.EQAry[ecc.zeroFramedARY[i][j]] = temp;
							ecc.zeroFramedARY[i][j] = temp;
							break;
						case 2:
							int temp1 = 99999;
							for(int t=0; t<ecc.neighborARY.length; t++){
								if(ecc.neighborARY[t]!=0){
									//System.out.print(ecc.neighborARY[t] +" "+ ecc.neighborARY[t+1]+ " ");
									temp1 = Math.min(temp1,ecc.neighborARY[t]);
									//System.out.println(temp);
								}
							}
							ecc.updateEQAry(temp1);
							ecc.zeroFramedARY[i][j] = temp1;

					}
					ecc.count=0;
				}
			}
		}
	}
	
	public void E_ConnectCC_pass3(EightConnectedComponent ecc){
		//manage EQ table
		//left to right, top to bottom, p(i,j) = EQAry[p(i,j)]
		for(int i=1; i< ecc.zeroFramedARY.length-1; i++){
			for(int j=1; j<=ecc.zeroFramedARY[i].length-1; j++){
				ecc.zeroFramedARY[i][j] = ecc.EQAry[ecc.zeroFramedARY[i][j]];
			}
		}
	}
	
	public void PrintImage(FileWriter outfile, EightConnectedComponent ecc) throws IOException{
		//System.out.println(ecc.numRows + " " + ecc.numCols + " " + ecc.newMin + " " + ecc.newMax);
		outfile.write(ecc.numRows + " " + ecc.numCols + " " + ecc.newMin + " " + ecc.newMax);
		outfile.write(System.lineSeparator());
		for (int i = 1; i <= ecc.numRows+1; i++){
			for (int j = 1; j <= ecc.numCols+1; j++){
				//System.out.print(ecc.zeroFramedARY[i][j] + " ");
				outfile.write(ecc.zeroFramedARY[i][j] + " ");
			}
			//System.out.println();
			outfile.write(System.lineSeparator());
		}
	}

	public void updateEQAry(int temp){
		for(int i=0; i<4; i++){
			if(neighborARY[i] !=0 && neighborARY[i]> temp){
				EQAry[neighborARY[i]] = temp;
			}
		}
	}
	
	public void manageEQAry(EightConnectedComponent ecc){
		for(int index =1; index <= ecc.newLabel; index++){
			if(index == ecc.EQAry[index]){
				ecc.trueLabel++;
				ecc.EQAry[index] = ecc.trueLabel;
			}
			else{
				ecc.EQAry[index] = ecc.EQAry[ecc.EQAry[index]];
			}
		}
	}
	
	public void construct_ECCProperty(EightConnectedComponent ecc){
		//creating all property for each index for propertyARY.
		//constructor
		ecc.propertyARY = new property[ecc.trueLabel+1];
		for(int i=0; i<ecc.trueLabel+1; i++){
			ecc.propertyARY[i]=new property();
		}
		
		for(int i=1; i<=ecc.numRows; i++){
			for(int j=1; j<=ecc.numCols; j++){
				if(ecc.zeroFramedARY[i][j]>0){
					ecc.propertyARY[ecc.zeroFramedARY[i][j]].numbpixels++;
					ecc.propertyARY[ecc.zeroFramedARY[i][j]].label = ecc.zeroFramedARY[i][j];
					if(ecc.propertyARY[ecc.zeroFramedARY[i][j]].numbpixels ==1){
						ecc.propertyARY[ecc.zeroFramedARY[i][j]].minRow =i;
						ecc.propertyARY[ecc.zeroFramedARY[i][j]].maxCol =j;
						ecc.propertyARY[ecc.zeroFramedARY[i][j]].maxRow =i;
						ecc.propertyARY[ecc.zeroFramedARY[i][j]].minCol =j;
					}
					if (i<ecc.propertyARY[ecc.zeroFramedARY[i][j]].minRow)
						ecc.propertyARY[ecc.zeroFramedARY[i][j]].minRow = i;
					if (i>ecc.propertyARY[ecc.zeroFramedARY[i][j]].maxRow)
						ecc.propertyARY[ecc.zeroFramedARY[i][j]].maxRow = i;
					if (j<ecc.propertyARY[ecc.zeroFramedARY[i][j]].minCol)
						ecc.propertyARY[ecc.zeroFramedARY[i][j]].minCol = j;
					if (j>ecc.propertyARY[ecc.zeroFramedARY[i][j]].maxCol)
						ecc.propertyARY[ecc.zeroFramedARY[i][j]].maxCol = j;
				}
				if (ecc.zeroFramedARY[i][j] < ecc.newMin) //keep tracking the newMin 
					ecc.newMin = ecc.zeroFramedARY[i][j];
				if (ecc.zeroFramedARY[i][j] > ecc.newMax) // keep tracking the newMax
					ecc.newMax = ecc.zeroFramedARY[i][j];
			}
		}		
	}
	
	public void printECCProperty(FileWriter outfile, EightConnectedComponent ecc) throws IOException{
		outfile.write(ecc.numRows +" "+ ecc.numCols+" "+ecc.minVal+ " "+ ecc.trueLabel+ "\n" );
		outfile.write("Total label: "+ecc.trueLabel+ "\n");
		for(int i=1; i<=ecc.trueLabel; i++){
			outfile.write("For Label "+ i+ "\n");
			outfile.write("Have "+ ecc.propertyARY[i].numbpixels + " pixels. \n");
			outfile.write("Upper left most pixel bounding box is ("+ ecc.propertyARY[i].minRow + ","+ ecc.propertyARY[i].minCol+ ")\n");
			outfile.write("Lower right most pixel bounding box is ("+ ecc.propertyARY[i].maxRow + ","+ ecc.propertyARY[i].maxCol+ ")\n");
			outfile.write(System.lineSeparator());
		}
	}
	
	public void prettyPrint(FileWriter outfile, EightConnectedComponent ecc) throws IOException{
		for(int i=1; i<= ecc.numRows; i++){
			for(int j=1; j<=ecc.numCols; j++){
				if(ecc.zeroFramedARY[i][j]>0){
					outfile.write(ecc.zeroFramedARY[i][j]+" ");
				}
				else{
					outfile.write("  ");
				}
			}
			outfile.write(System.lineSeparator());
		}
	}

	public void printEQAry(FileWriter outfile, EightConnectedComponent ecc) throws IOException{
		outfile.write("EQAry table: \nIndex: ");
		for(int m=0; m<=ecc.newLabel; m++){
			outfile.write(+m+ " ");
		}
		outfile.write(System.lineSeparator());
		outfile.write("Value: ");
		for(int i=0; i<=ecc.newLabel; i++){
			outfile.write(ecc.EQAry[i]+" ");
		}
		outfile.write(System.lineSeparator());
	}	
}
