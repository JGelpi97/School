/*
	Joey Gelpi
	CS1550 - Project 3
*/

import java.io.*;
import java.util.*;

/*
	When need to evict, go around entire clock until we find ref=0
	If not referenced(ref=0), evict it, move pointer to next page, stop
	If refereced, move on but make it unreferenced
*/
public class clock
{
	int index;
	int[] space;
	int size;
	
	public clock(int s) 
	{
		index = 0;
		size = s;
		space = new int[size];
		for (int i = 0; i < size; i++)
		{
			space[i] = -1;
		}
	}
	
	public void addPage(int pageNum, int frameNum)
	{
		space[frameNum] = pageNum;
	}
	
	public int evictPage(pageTable pt)
	{
		boolean foundPage = false;
		int frameToEvict = -1;
		while (!foundPage)
		{
			if (pt.isRef(space[index]))			//The page has been referenced
			{
				pt.setRef(space[index], false);	//Unref the page, we dont really have to do this, it will end up being re-referenced if we bring it back into memory
			}
			else								//The page hasnt been referenced, evict it
			{
				foundPage = true;
				frameToEvict = index;
			}
			index++;
			if (index == size)
			{
				index = 0;
			}
		}
		return frameToEvict;
	}
}
