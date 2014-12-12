package edu.hust.cse.diacritics.restoratin.test;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;

import edu.hust.cse.diacritics.restoration.RestoreDiacritics;
import junit.framework.TestCase;

public class Test extends TestCase{
	public void testRestoration() {
		String inputDir = "";
	    long start = System.currentTimeMillis();
    	System.out.println("Loading DB ...");
    	RestoreDiacritics.init();
    	System.out.println("Processing ...");
    	try {
    		File[] files = (new File(inputDir)).listFiles();

    		for (File file : files) {
	    		System.out.println(file.getPath());
    			if (!file.isDirectory()
                        && !file.isHidden()
                        && file.exists()
                        && file.canRead()
                        && (file.getPath().endsWith("txt"))) {
    				int count = 0;
    				int realcount = 0;
    				System.out.println("OK");
    				String pathFile = inputDir + file.getName().substring(0, file.getName().length()-3)+"out";	    				
    				FileWriter fw = new FileWriter(pathFile);
	    			BufferedReader reader = new BufferedReader(new FileReader(file));
			    	String word = null;
			    	String line = "";
			    	String separedP = ".?!";
			    	while ((word = reader.readLine())!= null) {
	//		    		System.out.println(word);
			    		if (word.trim() != "")
			    			count++;				    		
			    		line = line + " " + word.trim();
			    		
			    		if (separedP.contains(word.trim())) {
			    			String result = RestoreDiacritics.restore(line);
			    			String[] resultarr = result.trim().split("\\s");
			    			String[] initwords = line.trim().split("\\s");
			    			realcount = realcount + resultarr.length;
			    			int index = resultarr.length;
			    			if (index>initwords.length)
			    				index = initwords.length;
			    			for (int c=0; c<index; c++) 
			    				fw.write(initwords[c] + "\t" + resultarr[c] + "\n");

			    			line = "";
			    		}		    		
			    	}	
			    	fw.close();
    			}
    		}
    	}
    	catch (Exception ex) {
    		ex.printStackTrace();
    	}
    	long elapsedTimeMillis = System.currentTimeMillis()-start;
    	
    	float elapsedTimeMin = elapsedTimeMillis/(60*1000F);
    	System.out.print("Time : ");
    	System.out.println(elapsedTimeMin);
    	System.out.println("Done");
	}
}
