package proj9_2;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class pro9_2 {
	public static void main(String[] args) throws IOException{	
		//step 0: - open input file and output files
		//	dynamically allocate Hough2DAry and initiallize to zero
		File input = new File(args[0]);
		Scanner inFile = new Scanner(input);
		File output1 = new File(args[1]);
		File output2 = new File(args[2]);
		FileWriter outFile1 = new FileWriter(output1);
		FileWriter outFile2 = new FileWriter(output2);
		int group, numPoints, row, col;
	    boolean flag = true;
	    numPoints = inFile.nextInt();
	    row = inFile.nextInt();
	    col = inFile.nextInt();
	    System.out.println("How many group you want? (For K) ");
	    Scanner inputK = new Scanner(System.in);
	    group = inputK.nextInt();
	    outFile1.write(group+"\n"+numPoints+"\n"+row+" "+col+"\n");
	    KIsoData Kcluster  = new KIsoData(group, numPoints, row, col);
	    Kcluster.loadPointSet(inFile);
	    Kcluster.assignLable();

	    while(flag == true){
	        Kcluster.mapPoint2Image();
	        Kcluster.PrettyPrint(outFile2);
	        flag = Kcluster.IsoClustering();
	    }
	    
	    //Kcluster.mapPoint2Image();
	    //Kcluster.PrettyPrint(output2);
	    Kcluster.printPointSet(outFile1);
	    
		inFile.close();
		outFile1.close();
		outFile2.close();
		
	}
}
