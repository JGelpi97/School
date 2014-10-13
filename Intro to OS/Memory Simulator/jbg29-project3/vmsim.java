/*
	Joey Gelpi
	CS1550 - Project 3
*/

import java.io.*;
import java.util.*;

public class vmsim 
{
	public static void main(String[] args) throws IOException
	{
		char algo = 'D';
		int refreshTime = -1;
		int numFrames = 0;
		
		OPT optAlgo = new OPT();
		
		for (int i = 0; i < args.length; i++)
		{
			if (args[i].compareTo("-n") == 0)
			{
				numFrames = Integer.parseInt(args[i + 1]);
				if (numFrames <= 0)										//frames must be positive
					numFrames = -1;
			}
			else if (args[i].compareTo("-a") == 0)
			{
				String alg = args[i + 1];
				if (alg.compareTo("opt") == 0)
					algo = 'O';
				else if (alg.compareTo("clock") == 0)
					algo = 'C';
				else if (alg.compareTo("nru") == 0)
					algo = 'N';
				else if (alg.compareTo("rand") == 0)
					algo = 'R';
			}
			else if (args[i].compareTo("-r") == 0)
			{
				refreshTime = Integer.parseInt(args[i + 1]);
			}
		}
		
		if (algo == 'D' || numFrames == -1 || (algo == 'N' && refreshTime == -1))		//One of the fields was not completed at all or correctly
		{
			System.out.println("Usage: vmsim –n <numframes> -a <opt|clock|nru|rand> [-r <NRUrefresh>] <tracefile>");
			System.exit(0);
		}
		
		pageTable pt = new pageTable();
		
		memory mem;
		
		if (algo == 'O')
		{
			//Pre read in for opt and build the table
			File file = new File(args[args.length - 1]);
			BufferedReader br = new BufferedReader(new FileReader(file));
			String line;
			while ((line = br.readLine()) != null)
			{
				int pageNum = Integer.decode("0x" + line.substring(0, 5));		//Get address as Integer
				optAlgo.createTable(pageNum);
			}
			br.close();
			mem = new memory(numFrames, pt, optAlgo);
		}
		else
		{
			mem = new memory(numFrames, algo, pt, refreshTime);
		}		
		
		File file = new File(args[args.length - 1]);
		BufferedReader br = new BufferedReader(new FileReader(file));					
		
		String line;
		while ((line = br.readLine()) != null)
		{
			int pageNum = Integer.decode("0x" + line.substring(0, 5));		//Get address as Integer
			String type = line.substring(9, 10);						
			mem.addPage(pageNum, type.charAt(0));
		}
				
		mem.printStats();
		br.close();
	}
	
	
}