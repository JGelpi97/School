/*
	Joey Gelpi
	CS1550 - Project 3
*/

import java.io.*;
import java.util.*;

public class pageTable
{
	private PTE[] table = new PTE[(int)(Math.pow(2, 20))];

	public pageTable()
	{
	
	}
	
	//Put this page into a frame
	public void updateEntry(int pageNum, int frameNum)
	{
		if (table[pageNum] == null)		//If there is no info about this page
		{
			table[pageNum] = new PTE(frameNum);
		}
		else											//The page mapping already exists, update it
		{
			table[pageNum].frameNum = frameNum;
		}
	}
	
	public int pageFrameNumber(int pageNum)
	{
		return table[pageNum].frameNum;
	}
	
	public boolean isDirty(int pageNum)
	{
		return table[pageNum].dirty;
	}
	
	public void clean(int pageNum)
	{
		table[pageNum].dirty = false;
	}
	
	public void makeDirty(int pageNum)
	{
		table[pageNum].dirty = true;
	}
	
	public boolean isRef(int pageNum)
	{
		return table[pageNum].ref;
	}
	
	public void setRef(int pageNum, boolean val)
	{
		table[pageNum].ref = val;
	}
	
	//Does this page have an entry yet?
	public boolean entryExists(int pageNum)
	{
		if (table[pageNum] == null)
			return false;
		return true;
	}
	
	private class PTE
	{
		public int frameNum = -1;
		public boolean dirty = false;
		public boolean ref = true;
		
		public PTE(int fn)
		{
			frameNum = fn;
		}
	}
}