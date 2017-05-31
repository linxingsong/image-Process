import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class Main {

	public static void main(String[] args) throws FileNotFoundException {
		Scanner input = new Scanner(new File(args[0]));
		PrintWriter output1 = new PrintWriter(args[1]);
		PrintWriter output2 = new PrintWriter(args[2]);
		//Step 0 read the image header
		int row = input.nextInt();
		int col = input.nextInt();
		int min = input.nextInt();
		int max = input.nextInt();
		
		ThinningSkeleton thinning = new ThinningSkeleton(row, col, min, max);
		thinning.zeroFramed(thinning.firstAry);
		thinning.zeroFramed(thinning.secondAry);
		thinning.loadImage(input);		
		while(thinning.changeFlag){	
			// step 4
			if(thinning.cycleCount == 0 ||thinning.cycleCount == 3 || thinning.cycleCount == 5){
				output2.println("Thinning in " + thinning.cycleCount + " th Cycle");
				thinning.prettyPrint(output2);
			}
			// step 5
			thinning.changeFlag = false;
			thinning.cycleCount++;
			// step 6
			thinning.northThinning();
			thinning.copyAry();
			// step 7
			thinning.southThinning();
			thinning.copyAry();
			// step 8
			thinning.westThinning();
			thinning.copyAry();
			// step 9
			thinning.eastThinning();
			thinning.copyAry();
		}// step 10: repeat step 4 - 9 while  changeFile is true 
	
		// step 11: prettyPrint firstAry to args[2]
		output2.println("Final result");
		thinning.prettyPrint(output2);
		// step 12: write image header to args[1] and copy firstAry from [1][1] to args[1]
		thinning.createImage(output1);
		//step 13: close all files
		input.close();
		output1.close();
		output2.close();
	}
}

class ThinningSkeleton{
	
	private int numRows;
	private int numCols;
	private int minVal;
	private int maxVal;
	public boolean changeFlag; 
	public int cycleCount;
	public int[][] firstAry;
	public int[][] secondAry;
	
	public ThinningSkeleton(int row, int col, int min, int max){
		numRows = row;
		numCols = col;
		minVal = min;
		maxVal = max;
		changeFlag = true;
		// dynamiclly allocate first and second array
		firstAry = new int[row+2][col+2];
		secondAry = new int[row+2][col+2];	
	}
	
	public void zeroFramed(int[][] ary){
		for(int i = 0; i < ary.length; i++)
			for(int j = 0; j < ary.length; j++)
				ary[i][j] = 0;
	}
	
	public void loadImage(Scanner input){
		for(int i = 1; i <= numRows; i++)
			for(int j = 1; j <= numCols; j++){
				int pixel = input.nextInt();
				firstAry[i][j] = pixel;
				secondAry[i][j] = pixel;
			}
	}
	
	public void copyAry(){
		for(int i = 1; i <= numRows; i++)
			for(int j = 1; j <= numCols; j++)
				firstAry[i][j] = secondAry[i][j];
	}
	
	public int getMinNeighborLabel(int[] arr){
		int min = 99;
		for(int i =0; i < arr.length; i++)
				if(arr[i] != 0 && arr[i] < min)   
					min = arr[i];
		return min;
	}
	
	public boolean notCreateTwoORMoreCC(int i, int j){
		int[][] ary = new int [5][5];
		int [] EQAry =  new int[12];
		for(int n = 0; n < 12; n++) EQAry[n] = n;
		
		zeroFramed(ary);
		ary[1][1] = secondAry[i-1][j-1];
		ary[1][2] = secondAry[i-1][j];
		ary[1][3] = secondAry[i-1][j+1];
		ary[2][1] = secondAry[i][j-1];
		ary[2][2] = 0;
		ary[2][3] = secondAry[i][j+1];
		ary[3][1] = secondAry[i+1][j-1];
		ary[3][2] = secondAry[i+1][j];
		ary[3][3] = secondAry[i+1][j+1];
		int newLabel = 0;
		// pass -1 for connected component
		for(int r = 1; r < 4; r++)
			for(int c =1; c < 4; c++)
				if(ary[r][c] > 0){
					int[] neighbors = {ary[r-1][c-1],ary[r-1][c],ary[r-1][c+1],ary[r][c-1]};
					// case 1: all 4 neighbors are 0
					if(neighbors[0] == 0 && neighbors[1] == 0 
							&& neighbors[2] == 0 && neighbors[3] == 0){
						newLabel++;
						ary[r][c] = newLabel;
					
					}else{
						int minLabel = getMinNeighborLabel(neighbors);
						ary[r][c] = minLabel;  
						for(int n =0; n < 4; n++)
							if(neighbors[n] != 0 && neighbors[n] > minLabel)
								EQAry[neighbors[n]] = minLabel;
							
					}		
				
				}// end of pass 1
		
		// pass 2
		for(int r=3;r>0;r--)
			for(int c=3;c>0;c--)
				if(ary[r][c]>0){
					int[] neighbors = {ary[r][c], ary[r][c+1],
							ary[r+1][c-1], ary[r+1][c], ary[r+1][c+1]};
					int minLabel = getMinNeighborLabel(neighbors);
					ary[r][c] = minLabel;
					for(int n =0; n < 5; n++)
						if(neighbors[n] != 0 && neighbors[n] > minLabel)
							EQAry[neighbors[n]] = minLabel;
					
				}// end of pass 2	
		// manage EQAry
		int trueLabel = 0;
		int index = 1;
		while(index <= newLabel){
			if(EQAry[index] == index)
				trueLabel++;
			index++;
		}
	
		return trueLabel == 1;
	}
	
	
	public void doThinning(int i, int j){
		int countNotZeros = 0;
		for(int r = i - 1; r <= i + 1; r++)
			for(int c = j - 1; c <= j + 1; c++)
				if(secondAry[r][c] == 1) countNotZeros++;
		
		if(countNotZeros > 4  && notCreateTwoORMoreCC(i,j)){ //notCreateTwoORMoreCC(i,j)
			secondAry[i][j] = 0;
			changeFlag = true;
		}
	}
	
	public void northThinning(){
		boolean first = true;
		for(int i = 1; i <= numRows; i++)
			for(int j =1; j <= numCols; j++){
				if(first && firstAry[i][j] == 1){
					first = false;
					continue;
				}
				if(firstAry[i][j] == 1 && firstAry[i-1][j] == 0)
					doThinning(i, j);
			}
	}
	
	public void southThinning(){
		for(int i = 1; i <= numRows; i++)
			for(int j =1; j <= numCols; j++){
				if(firstAry[i][j] == 1 && firstAry[i+1][j] == 0)
					doThinning(i, j);
			}
	}
	
	public void westThinning(){
		for(int i = 1; i <= numRows; i++)
			for(int j =1; j <= numCols; j++)
				if(firstAry[i][j] == 1 && firstAry[i][j-1] == 0)
					doThinning(i, j);
	}
	
	public void eastThinning(){
		for(int i = 1; i <= numRows; i++)
			for(int j =1; j <= numCols; j++)
				if(firstAry[i][j] == 1 && firstAry[i][j+1] == 0)
					doThinning(i, j);
	}
	
	public void prettyPrint(PrintWriter output){
		for(int i = 1; i <= numRows; i++){
			for(int j = 1; j <= numCols; j++)
				if(firstAry[i][j] == 0) output.printf("%2s", " ");
				else output.printf("%2s",firstAry[i][j]);	
			
			output.println();
		}
	}
	
	public void createImage(PrintWriter output) {
		output.println(numRows + " " + numCols + " " + minVal+ " " + maxVal);
		for(int i = 1; i <= numRows; i++){
			for(int j = 1; j <= numCols; j++)
				output.printf("%2s",firstAry[i][j]);

			output.println();
		}
	}	
}