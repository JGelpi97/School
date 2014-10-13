using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NaT_Final_Project
{
    public class Room
    {
        //0 = no door, 1 = open door, 2 = locked door
        public int doorN;
        public int doorS;
        public int doorW;
        public int doorE;

        bool note;
        bool chest;

        string description;
        string noteText;

        public List<string> chestContents = new List<string>();

        public Room(int doorN, int doorS, int doorW, int doorE, bool chest, bool note)
        {
            this.doorN = doorN;
            this.doorS = doorS;
            this.doorW = doorW;
            this.doorE = doorE;
            this.chest = chest;
            this.note = note;
        }

        public void printDescription()
        {
            Console.WriteLine(description);
            if (chest)
            {
                Console.WriteLine("There is a chest in the room.");
            }            
        }

        public void setDescription(string s)
        {
            description = s;
        }

        public bool hasChest()
        {
            return chest;
        }
        public bool hasNote()
        {
            return note;
        }

        public void setNoteText(string s)
        {
            noteText = s;
        }

        public void printNoteText()
        {
            Console.WriteLine(noteText);
        }

        public void printChestContents(Player p)
        {
            if (chestContents.Count == 0)
            {
                Console.WriteLine("The chest is empty.");
            }
            else
            {
                Console.WriteLine("The chest contains:");
                foreach (string s in chestContents)
                {
                    Console.WriteLine(" " + s);
                    if (string.Compare(s, "key") == 0)      //Keys dont go into inventory
                    {
                        p.addKey();
                    }
                    else if (string.Compare(s, "gold key") == 0)
                    {
                        p.goldKey = true;
                    }
                    else
                    {
                        p.addToInventory(s);
                    }
                }
                Console.WriteLine("You take the contents. The chest is empty.");
                chestContents.Clear();
            }
        }
    }
}
