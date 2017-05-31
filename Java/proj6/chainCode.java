package proj6;

import java.io.FileWriter;
import java.io.IOException;

public class chainCode{
	public point neighborCoord[];
	public point startP;
	public point currentP;
	public point nextP;
	public int lastQ, nextQ;
	public int nextDirTable[]={6,6,0,0,2,2,4,4};
	public int nextDir;
	public int PChain;

	public chainCode(image img, FileWriter outfile1, FileWriter outfile2)throws IOException{
		startP= new point();
		currentP = new point();
		nextP = new point();
		neighborCoord = new point[8];
		gotFirstNeededPixel:
		for(int i=0; i<img.numRows+2; i++){
			for(int j=0; j<img.numCols+2; j++){
				//declare first position, start point.
				if(img.zeroFramedAry[i][j]>0){
					outfile1.write("Start Row；"+i+" Start Col: "+j+" Label Value: "+img.zeroFramedAry[i][j]);
	                outfile2.write(i+" "+j+" "+img.zeroFramedAry[i][j]+" ");
					startP.row = i;
					startP.col = j;
					currentP.row = i;
					currentP.col = j;
					lastQ = 4;
					//outfile1.write(" lastQ: "+lastQ+" \n");
	                //outfile2.write(lastQ+" \n");
	                break gotFirstNeededPixel;
				}
			}
	    }

	    doChainCoding(img, outfile1, outfile2);
	    PrettyPrint(img);
	}
	
	void doChainCoding(image img, FileWriter outfile1, FileWriter outfile2) throws IOException{
		//step 8: repeat while condition false, not equal
		int count = 0;
		while(!equal(nextP, startP)){
	        //step 4: nextQ
			nextQ = (lastQ+1)%8;
	        //outfile1<<"nextQ: "<<nextQ<<" ";
	        //outfile2<<nextQ<<" ";
			//step 5: Pchain
	        PChain = findNextP(currentP, nextQ, img);
	        //step 6: output Pchain
	        outfile1.write("PChain: "+PChain+" ");
	        if(count%20 ==0){
	        	outfile2.write("\n");
	        }
	        outfile2.write(PChain+" ");
	       
	        //step 7: lastQ
	        lastQ = nextDirTable[PChain];
	        //outfile1<<"lastQ: "<<lastQ<<" ";
	        //outfile2<<lastQ<<" ";
	        currentP.row = nextP.row;
	        currentP.col = nextP.col;
	        count++;
	        //outfile1.write("currentP: "+currentP.row+" "+currentP.col+" ");
	        //outfile2.write(currentP.row+" "+currentP.col+" \n");
	    }
	}
	
	boolean equal(point a, point b){ // function check is two point are the same{
		if(a.row == b.row && a.col == b.col){
			return true;
		}
		else
			return false;
	}
	
	int findNextP(point currentP, int nextQ, image image){ // the function of find next point of chain code
		//step 5.1: load NiighborsCoord(currentP)
		loadNeighborsCoord(currentP);
		int chainDir;	
		//step 5.2: chainDir
		while(image.zeroFramedAry[neighborCoord[nextQ].row][neighborCoord[nextQ].col]!= 1){
			nextQ = (nextQ+1)%8;
		}
		chainDir = nextQ;
		//step 5.3: nextP
		nextP.row = neighborCoord[chainDir].row;
		nextP.col = neighborCoord[chainDir].col;
		//step 5.4: return.
		return chainDir;
	}
	
	void loadNeighborsCoord(point curr){ // load neighbor{
		neighborCoord[0]= new point();
		neighborCoord[1]= new point();
		neighborCoord[2]= new point();
		neighborCoord[3]= new point();
		neighborCoord[4]= new point();
		neighborCoord[5]= new point();
		neighborCoord[6]= new point();
		neighborCoord[7]= new point();
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
					System.out.print(image.zeroFramedAry[i][j] + " ");
					}
				else{
	                System.out.print("  ");
				}
			}
			System.out.println();
		}
	}
}