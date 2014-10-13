/*
	Joey Gelpi
	CS1550 - Project 3
*/

import java.io.*;
import java.util.*;

public class OPT
{
	HashMap<Integer, ArrayList<Integer>> map = new HashMap<Integer, ArrayList<Integer>>();
	int timeStep = 1;

	public OPT() {}
		
	public void createTable(int pageNum)
	{
		if (map.containsKey(pageNum))		//if it already has the page number, add the timeStep to the list
		{
			ArrayList<Integer> timeStepsList = map.get(pageNum);
			timeStepsList.add(timeStep);
		}
		else								//The map does not contain the page number
		{
			ArrayList<Integer> timeStepsList = new ArrayList<Integer>();
			timeStepsList.add(timeStep);
			map.put(pageNum, timeStepsList);
		}
		timeStep++;
	}
	
	
	/*
		This method goes through every page in memory, it looks through the list of when each page will be referenced next,
		and it finds the farthest one away, and returns the frame that that page exists in.
	*/
	public int evictPage(int currentTime, memory mem)
	{
		int frameToEvict = -1;
		
		int[] memArray = mem.memArray();
		
		int farthestAway = currentTime;		
		for (int i = 0; i < memArray.length; i++)					//Go through all pages in memory
		{
			int currPage = memArray[i];
			ArrayList<Integer> timeStepsList = map.get(currPage);
			//System.out.println(currPage + " = " + timeStepsList);
			
			for (int j = 0; j < timeStepsList.size(); j++)
			{
				if (timeStepsList.get(j) > currentTime)
				{
					if (timeStepsList.get(j) > farthestAway)
					{
						farthestAway = timeStepsList.get(j);			//Now we have the farthest away time
						//System.out.println("farthest away = " + farthestAway);
						frameToEvict = i;								//We want to evict the farthest away page
						break;											//Dont keep looking in one pages access times, move on to next page
					}
					break;
				}
			}
		}
		
		/*
		System.out.println("Frame to evict = " + frameToEvict);
		System.out.println("current time = " + currentTime);
		System.out.println("farthest away = " + farthestAway);
		for (int i = 0; i < memArray.length; i++)	
		{
			System.out.println(memArray[i] + "=" + map.get(memArray[i]));
		}
		if (currentTime > 10)
			System.exit(0);
		*/
		
		if (frameToEvict == -1)										//All the pages will not be referenced again
		{
			frameToEvict = 0;										//So just evict page 0, 
		}
		
		return frameToEvict;
		
	}
}