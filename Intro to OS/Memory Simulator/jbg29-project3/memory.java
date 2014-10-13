/*
	Joey Gelpi
	CS1550 - Project 3
*/

import java.io.*;
import java.util.*;

public class memory
{	
	private int numFrames;
	private int[] frameSpace;	//An array of page numbers, -1 indicates no page is currently in the frame(basically simulates ram frames)
	private int freeFrames;		//The number of free frames we have, this way we only have to search for an empty frame to use if there is one
	private char evictionAlgo;
	private pageTable pt;
	private int diskWrites = 0;	//Increase by 1 if we evict a dirty page
	private int pageFaults = 0;
	private int accesses = 0;
	
	clock clockAlgo;
	random randomAlgo = new random();
	NRU nruAlgo;
	OPT optAlgo;

	public memory(int nf, char eg, pageTable _pt, int refreshTime)
	{		
		evictionAlgo = eg;
		pt = _pt;
		numFrames = nf;
		freeFrames = numFrames;
		frameSpace = new int[numFrames];
		for (int i = 0; i < numFrames; i++)
		{
			frameSpace[i] = -1;
		}
		
		clockAlgo = new clock(nf);
		nruAlgo = new NRU(refreshTime);
	}
	
	public memory(int nf, pageTable _pt, OPT opt)
	{		
		evictionAlgo = 'O';
		pt = _pt;
		numFrames = nf;
		freeFrames = numFrames;
		frameSpace = new int[numFrames];
		for (int i = 0; i < numFrames; i++)
		{
			frameSpace[i] = -1;
		}
		
		optAlgo = opt;
	}	
	
	/**
		Gets a page number(that is not already in a frame), and adds it to memory.
		We know the page does not already exist in memory because we are checking against the page table first,
		 if the page doesnt exist currently in memory(a frame), then we send it to be added.
	*/
	public void addPage(int pageNumber, char type)
	{		
		accesses++;
		if (accesses % 10000 == 0)
			//System.out.println(accesses);
		
		if (evictionAlgo == 'N')
			nruAlgo.flipBits(this, pt);								//Increase the reference count of the NRU algo, flip if ready
		
		//If the page is not in a frame or its entry doesnt exist
		if (!pt.entryExists(pageNumber) || pt.pageFrameNumber(pageNumber) == -1)
		{		
			pageFaults++;
			
			if (freeFrames != 0)								//If there is a space
			{
				freeFrames--;									//1 less free frame
				
				for (int i = 0; i < numFrames; i++)
				{
					if (frameSpace[i] == -1)					//Found empty frame
					{
						frameSpace[i] = pageNumber;				//Memory contains this page number now
						pt.updateEntry(pageNumber, i);			//Update pageTable, now pageTable knows that this page in memory at frame i
						if (type == 'W')						//If we are Writing, make that page dirty, otherwise it is clean by default on a Read
						{
							pt.makeDirty(pageNumber);
						}
						
						if (evictionAlgo == 'C')				//If the clock algo, update the clock
						{
							clockAlgo.addPage(pageNumber, i);
						}
						
						pt.setRef(pageNumber, true);
						
						break;
					}
				}
			}
			else												//There isnt a space, Evict
			{				
				/*
					Split the algorithms up completely so it is simpler to understand,
					there will be redundant code but it doesnt all get run, so it doesnt make much a difference
				*/
				if (evictionAlgo == 'R')		//Random
				{
					int frameToEvict = randomAlgo.evictPage(numFrames);		//Get a frame to evict			
					
					int pageToEvict = frameSpace[frameToEvict];
					
					if (pt.isDirty(pageToEvict))							//If page we are evicting is dirty
					{
						diskWrites++;										//We write that page to disk
						pt.clean(pageToEvict);								//We make it clean
					}
					
					pt.updateEntry(pageToEvict, -1);					//Tell page table the page we are evicting is no longer in a frame
											
					frameSpace[frameToEvict] = pageNumber;				//Add page to frame in memory
					pt.updateEntry(pageNumber, frameToEvict);			//Tell page table it now exists in memory
					if (type == 'W')									//If we are Writing, make that page dirty, otherwise it is clean by default on a Read
					{
						pt.makeDirty(pageNumber);
					}
					
					pt.setRef(pageNumber, true);						//We referenced the page
				}
				else if (evictionAlgo == 'C')	//Clock
				{
					int frameToEvict = clockAlgo.evictPage(pt);
					
					int pageToEvict = frameSpace[frameToEvict];
					
					
					if (pt.isDirty(pageToEvict))							//If page we are evicting is dirty
					{
						diskWrites++;										//We write that page to disk
						pt.clean(pageToEvict);								//We make it clean
					}
					
					pt.updateEntry(pageToEvict, -1);					//Tell page table the page we are evicting is no longer in a frame

					frameSpace[frameToEvict] = pageNumber;				//Add page to frame in memory
					pt.updateEntry(pageNumber, frameToEvict);			//Tell page table it now exists in memory
					if (type == 'W')									//If we are Writing, make that page dirty, otherwise it is clean by default on a Read
					{
						pt.makeDirty(pageNumber);
					}

					clockAlgo.addPage(pageNumber, frameToEvict);		//Update the clock
					
					pt.setRef(pageNumber, true);						//We referenced the page
				}
				else if (evictionAlgo == 'N')
				{
					int frameToEvict = nruAlgo.evictPage(this, pt);
					
					int pageToEvict = frameSpace[frameToEvict];
					
					
					if (pt.isDirty(pageToEvict))							//If page we are evicting is dirty
					{
						diskWrites++;										//We write that page to disk
						pt.clean(pageToEvict);								//We make it clean
					}
					
					pt.updateEntry(pageToEvict, -1);					//Tell page table the page we are evicting is no longer in a frame

					frameSpace[frameToEvict] = pageNumber;				//Add page to frame in memory
					pt.updateEntry(pageNumber, frameToEvict);			//Tell page table it now exists in memory
					if (type == 'W')									//If we are Writing, make that page dirty, otherwise it is clean by default on a Read
					{
						pt.makeDirty(pageNumber);
					}
					
					pt.setRef(pageNumber, true);						//We referenced the page
				}									
				else if (evictionAlgo == 'O')
				{
					int frameToEvict = optAlgo.evictPage(accesses, this);
					
					int pageToEvict = frameSpace[frameToEvict];
					
					
					if (pt.isDirty(pageToEvict))							//If page we are evicting is dirty
					{
						diskWrites++;										//We write that page to disk
						pt.clean(pageToEvict);							//We make it clean
					}
					
					pt.updateEntry(pageToEvict, -1);					//Tell page table the page we are evicting is no longer in a frame

					frameSpace[frameToEvict] = pageNumber;				//Add page to frame in memory
					pt.updateEntry(pageNumber, frameToEvict);			//Tell page table it now exists in memory
					if (type == 'W')									//If we are Writing, make that page dirty, otherwise it is clean by default on a Read
					{
						pt.makeDirty(pageNumber);
					}
					
					pt.setRef(pageNumber, true);						//We referenced the page
					
				}
			}
		}
		else 															//Page already exists in a frame, it might need to be made dirty
		{
			if (type == 'W')											//If we are Writing, make that page dirty, otherwise it is clean by default on a Read
			{
				pt.makeDirty(pageNumber);
			}
			pt.setRef(pageNumber, true);
		} 
	}
	
	public int[] memArray()
	{
		return frameSpace;
	}
	
	public int numberOfFrames()
	{
		return numFrames;
	}
	
	public void printStats()
	{
		System.out.println("Number of frames: " + numFrames);
		System.out.println("Total memory accesses: " + accesses);
		System.out.println("Total page faults: " + pageFaults);
		System.out.println("Total writes to disk: " + diskWrites);
	}
		
}