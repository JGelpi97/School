using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NaT_Final_Project
{
    public class Player
    {
        int numKeys = 0;
        public List<string> inventory = new List<string>();
        int xLoc = 1;
        int yLoc = 2;
        public bool goldKey = false;

        public Player()
        {
            
        }

        public void printInventory()
        {
            if (inventory.Count != 0 || numKeys > 0 || goldKey)            
            {
                Console.WriteLine("Inventory:");
                if (numKeys == 1)
                {
                    Console.WriteLine(" 1 key");
                }
                else if (numKeys > 1)
                {
                    Console.WriteLine(" " + numKeys + " keys");
                }
                if (goldKey)
                {
                    Console.WriteLine(" 1 gold key");
                }
                foreach (string s in inventory)
                {
                    Console.WriteLine(" " + s);
                }
            }
            else
            {
                Console.WriteLine("You have no inventory.");
            }
        }

        public void moveNorth()
        {
            yLoc--;
        }
        public void moveSouth()
        {
            yLoc++;
        }
        public void moveWest()
        {
            xLoc--;
        }
        public void moveEast()
        {
            xLoc++;
        }
        public int xPos()
        {
            return xLoc;
        }
        public int yPos()
        {
            return yLoc;
        }
        public void addToInventory(string s)
        {
            inventory.Add(s);
        }
        public void addKey()
        {
            numKeys++;
        }
        public void removeKey()
        {
            numKeys--;
        }
        public int numberOfKeys()
        {
            return numKeys;
        }
        public bool hasGoldKey()
        {
            return goldKey;
        }

    }
}
