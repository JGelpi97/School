/*
	Joey Gelpi
	CS1550 - Project 3
*/

import java.io.*;
import java.util.*;

public class random
{
	Random r = new Random();

	public random() {}
	
	public int evictPage(int numFrames)
	{
		int frameToEvict = r.nextInt(numFrames);

		return frameToEvict;
	}
}