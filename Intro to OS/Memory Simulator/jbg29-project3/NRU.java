/*
	Joey Gelpi
	CS1550 - Project 3
*/

import java.io.*;
import java.util.*;

public class NRU
{
	private int refreshTime;
	private int numberOfReferences = 0;
	
	public NRU(int refTime)
	{
		refreshTime = refTime;
	}
	
	/*
		If we've hit our refresh interval, flip the bits
	*/
	public void flipBits(memory mem, pageTable pt)
	{
		numberOfReferences++;
		if (numberOfReferences >= refreshTime)
		{
			numberOfReferences = 0;
			for (int i = 0; i < mem.numberOfFrames(); i++)
			{
				if (mem.memArray()[i] != -1)					//There has to actually be a page
					pt.setRef(mem.memArray()[i], false);
			}
		}
	}
	
	public int evictPage(memory mem, pageTable pt)
	{
		int frameToEvict = -1;
		int[] classOfEntry = new int[mem.numberOfFrames()];
		for (int i = 0; i < mem.numberOfFrames(); i++)
		{
			classOfEntry[i] = -1;
		}
		int[] memArray = mem.memArray();									//So i dont have to keep getting the array
		for (int i = 0; i < mem.numberOfFrames(); i++)
		{
			if (!pt.isRef(memArray[i]) && !pt.isDirty(memArray[i]))			//Evict this page, it is class 0, non ref and clean
			{
				frameToEvict = i;
				break;
			}
			else if (!pt.isRef(memArray[i]) && pt.isDirty(memArray[i]))		//Class 1, non referenced and dirty
			{
				classOfEntry[i] = 1;							
			}
			else if (pt.isRef(memArray[i]) && !pt.isDirty(memArray[i]))		//Class 2, referenced and clean
			{
				classOfEntry[i] = 2;							
			}
			else if (pt.isRef(memArray[i]) && pt.isDirty(memArray[i]))		//Class 3, ref and dirty
			{
				classOfEntry[i] = 3;
			}
		}
		
		if (frameToEvict == -1)		//We didnt find an unreferenced clean page
		{
			int smallestClass = 4;			

			for (int i = 0; i < mem.numberOfFrames(); i++)
			{
				if (classOfEntry[i] == 1)
				{
					frameToEvict = i;
					break;
				}
				else if (classOfEntry[i] < smallestClass)
				{
					smallestClass = classOfEntry[i];
					frameToEvict = i;
				}
			}
		}
		return frameToEvict;
	}
}