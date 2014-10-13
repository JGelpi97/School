using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 * Joey Gelpi - 2014
 * Text based adventure game for Narrative and Technology final project
 */

namespace NaT_Final_Project
{
    class Program
    {
        static bool finalRoom = false;
        static bool gameOver = false;
        static Player player;
        static Room[,] map = new Room[4, 4];
        static int stage = -1;
        static void Main(string[] args)
        {
            setup();

            while (!gameOver)
            {
                Console.Write(">");
                string input = Console.ReadLine();
                parseInput(input);
            }
            
        }

        static public void setup()
        {
            //map[y, x];
            player = new Player();
            map[2, 1] = new Room(1, 1, 0, 1, false, true);     //Start
            map[2, 1].setDescription("This is where you woke up. The room is empty. There is a note on the wall.\nThere are doors to the north, south, and east.");
            map[2, 1].setNoteText(" You know what you did.");

            map[1, 1] = new Room(2, 1, 1, 1, true, false);
            map[1, 1].setDescription("A room with doors in all 4 directions.");
            map[1, 1].chestContents.Add("key");
            map[1, 1].chestContents.Add("apple");

            map[0, 1] = new Room(0, 2, 3, 0, false, true);
            map[0, 1].setDescription("A brightly lit room. There is the door to the south that you came from.\nThere is a huge door to the west locked with a golden padlock.\nThere is a note on the wall.");
            map[0, 1].setNoteText(" So close, yet so far.");

            map[1, 0] = new Room(0, 0, 0, 1, false, true);
            map[1, 0].setDescription("The room is pitch black, you can't see a thing. There might have been a breath\non your neck.");
            map[1, 0].setNoteText(" Shhhhhhh, this isn't here.");

            map[3, 1] = new Room(1, 0, 2, 1, true, false);
            map[3, 1].setDescription("The walls of this room are painted blue and there is a breeze from somewhere\nyou can't find. There are doors north, west, and east.");
            map[3, 1].chestContents.Add("key");
            map[3, 1].chestContents.Add("cake");

            map[3, 0] = new Room(1, 0, 0, 2, true, false);
            map[3, 0].setDescription("A room with doors to the north and east.");
            map[3, 0].chestContents.Add("key");

            map[2, 0] = new Room(0, 1, 0, 0, false, true);
            map[2, 0].setDescription("There is only the door to the south you came through. There is a note on \nthe wall.");
            map[2, 0].setNoteText(" You know what you did last summer... I sure as hell don't.");

            map[3, 2] = new Room(1, 0, 1, 1, true, false);
            map[3, 2].setDescription("It is dark, very dark, there is light from under the doors to the north,\neast, and west. There is a strange humming sound from the east door.");

            map[3, 3] = new Room(2, 0, 1, 0, false, true);
            map[3, 3].setDescription("The room is dark, an almost burned out candle sits in the corner.\nYou can make out doors to the north and west. You can see the faint outline of\na note on the wall in the darkness. There is a knock on the north door.");
            map[3, 3].setNoteText(" You can't get out of this one this time.");

            map[2, 3] = new Room(2, 2, 0, 0, true, true);
            map[2, 3].setDescription("A brightly lit room with doors to the north and south. The wall \nhas a note on it. You hear a knock on the north door. Your head is pounding.");
            map[2, 3].setNoteText(" Are you getting closer? Maybe you're getting farther... You'll never get \n her back.");

            map[1, 3] = new Room(0, 2, 0, 0, true, true);
            map[1, 3].setDescription("There is nothing in here that could have been making noise. There is only\nthe south door you came through. The wall has a lengthy note on it.");
            map[1, 3].chestContents.Add("flame thrower");
            map[1, 3].chestContents.Add("pie");
            map[1, 3].setNoteText(" You probably think you're making some progress here. \n Actually, maybe you are, but then again maybe you aren't.\n You probably have no idea who I am, but I bet you would like to find out. \n Keep searching..");

            map[2, 2] = new Room(1, 1, 1, 0, false, false);
            map[2, 2].setDescription("A room with doors north, west, and south. There is an old couch in here.");

            map[1, 2] = new Room(1, 1, 1, 0, true, true);
            map[1, 2].setDescription("A room with doors north, west, and south. There is a note on the wall");
            map[1, 2].chestContents.Add("gold key");
            map[1, 2].setNoteText(" Congratulations.");

            map[0, 2] = new Room(0, 1, 0, 1, false, false);
            map[0, 2].setDescription("A room with doors to the south and east.");

            map[0, 3] = new Room(0, 0, 1, 0, true, false);
            map[0, 3].setDescription("A room with only the door to the west that you came from.");
            map[0, 3].chestContents.Add("key");

            map[0, 0] = new Room(0, 0, 0, 0, false, false);
            Console.WriteLine("<Insert title here>");
            Console.WriteLine("Joey Gelpi - 2013\n");
            Console.WriteLine("Welcome, the basic controls are n, e, s, and w for movement. 'i' for inventory.");
            Console.WriteLine("Some other commands you might want to try are 'open chest', 'read note', or \n'unlock door'.\n\n");
            Console.WriteLine("----------------------------------------------------------\n\n");
            Console.WriteLine("You are in a dimly lit room with 2 flickering candles on the old stone walls.\n" +
                  "There are no windows, and there is a heavy feel to the air. The last thing you\n" +
                  "remember is your wife screaming and then everything going black.\n");            
            Console.WriteLine("There is a note on the wall.\nThe room is empty. There are doors to the north, south, and east.");
        }

        static public void parseInput(string input)
        {
            input = input.ToLower();
            switch (input)
            {
                case "n":
                    {
                        if (currentRoom().doorN == 1)
                        {
                            player.moveNorth();
                            currentRoom().printDescription();
                        }
                        else if (currentRoom().doorN == 2)
                        {
                            Console.WriteLine("The door is locked.");
                        }
                        else
                        {
                            Console.WriteLine("You can't go that way.");
                        }
                    }
                    break;
                case "s":
                    {
                        if (currentRoom().doorS == 1)
                        {
                            player.moveSouth();
                            currentRoom().printDescription();
                        }
                        else if (currentRoom().doorS == 2)
                        {
                            Console.WriteLine("The door is locked.");
                        }
                        else
                        {
                            Console.WriteLine("You can't go that way.");
                        }
                    }
                    break;
                case "w":
                    {
                        if (currentRoom().doorW == 1)
                        {
                            player.moveWest();
                            if (player.xPos() == 0 && player.yPos() == 0)
                            {
                                finalRoom = true;
                                Console.WriteLine("You enter through the giant door. It slams shut behind and is now locked.\nIt is pitch black, you cannot see a thing. Your feet hit something.\nYou pick it up and it feels like a flashlight.");

                            }
                            else
                            {
                                currentRoom().printDescription();
                            }
                        }
                        else if (currentRoom().doorW == 2)
                        {
                            Console.WriteLine("The door is locked.");
                        }
                        else if (currentRoom().doorW == 3)
                        {
                            Console.WriteLine("It looks like you need a golden key to open this door.");
                        }
                        else
                        {
                            Console.WriteLine("You can't go that way.");
                        }
                    }
                    break;
                case "e":
                    {
                        if (finalRoom)
                        {
                            Console.WriteLine("The door wont budge now.");
                        }
                        else
                        {
                            if (currentRoom().doorE == 1)
                            {
                                player.moveEast();
                                currentRoom().printDescription();
                            }
                            else if (currentRoom().doorE == 2)
                            {
                                Console.WriteLine("The door is locked.");
                            }
                            else
                            {
                                Console.WriteLine("You can't go that way.");
                            }
                        }
                    }
                    break;
                case "i":
                    {
                        player.printInventory();
                    }
                    break;
                case "look":
                    {
                        currentRoom().printDescription();
                    }
                    break;
                case "open chest":
                case "search chest":
                    {
                        if (currentRoom().hasChest())
                        {
                            currentRoom().printChestContents(player);
                        }
                    }
                    break;
                case "take note":
                    {
                        if (currentRoom().hasNote())
                        {
                            Console.WriteLine("It's attatched to the wall and you can't get it off.");
                        }
                        else
                        {
                            Console.WriteLine("There is no note in here.");
                        }
                    }
                    break;
                case "read note":
                    {
                        if (currentRoom().hasNote())
                        {
                            Console.WriteLine("The note says:");
                            currentRoom().printNoteText();
                        }
                        else
                        {
                            Console.WriteLine("There is no note in here.");
                        }
                    }
                    break;
                case "unlock door":
                    {
                        if (player.numberOfKeys() > 0)
                        {
                            if (currentRoom().doorN == 2)
                            {
                                Console.WriteLine("You unlock the door.");
                                currentRoom().doorN = 1;
                                roomN().doorS = 1;
                                player.removeKey();
                            }
                            else if (currentRoom().doorS == 2)
                            {
                                Console.WriteLine("You unlock the door.");
                                currentRoom().doorS = 1;
                                roomS().doorN = 1;
                                player.removeKey();
                            }
                            else if (currentRoom().doorE == 2)
                            {
                                Console.WriteLine("You unlock the door.");
                                currentRoom().doorE = 1;
                                roomE().doorW = 1;
                                player.removeKey();
                            }
                            else if (currentRoom().doorW == 2)
                            {
                                Console.WriteLine("You unlock the door.");
                                currentRoom().doorW = 1;
                                roomW().doorE = 1;
                                player.removeKey();
                            }
                            else
                            {
                                Console.WriteLine("There is no door to unlock.");
                            }
                        }
                        else if (player.hasGoldKey() && currentRoom().doorW == 3)   //At final door with gold key, game over bitches
                        {
                            Console.WriteLine("You unlock the giant door with the golden key.");
                            currentRoom().doorW = 1;
                            player.goldKey = false;
                            stage = 0;
                        }
                        else
                        {
                            Console.WriteLine("You have no keys!");
                        }
                    }
                    break;
                case "turn on flashlight":                
                    {
                        if (!finalRoom)
                        {
                            Console.WriteLine("You don't have a flashlight.");
                        }
                        else
                        {
                            if (stage == 0)
                            {
                                Console.WriteLine("You turn on the flashlight. There is a lightswitch on the wall next to you. The flashlight dies. You drop it.");
                                stage = 1;
                            }
                            else
                            {
                                Console.WriteLine("The flashlight is dead.");
                            }
                        }
                    }
                    break;
                case "turn on lights":
                case "hit lightswitch":
                case "flip lightswitch":
                    {
                        if (!finalRoom)
                        {
                            Console.WriteLine("There is no lightswitch to use.");
                        }
                        else
                        {
                            if (stage == 1)
                            {
                                Console.WriteLine("You hit the switch and the lights come on. The room is bigger than the others.\nThere is a woman standing on the other side of the room. You think about saying something.");
                                stage = 2;
                            }
                            else if (stage < 1)
                            {
                                Console.WriteLine("You can't see anything.");
                            }
                            else if (stage > 1)
                            {
                                Console.WriteLine("The lights are already on.");
                            }
                        }
                    }
                    break;
                case "talk":
                case "yell":
                case "talk to her":
                case "talk to the woman":
                case "talk to woman":
                case "speak":                
                case "speak to the woman":
                case "speak to woman":
                case "say something":
                case "say something to the woman":                    
                case "say something to woman":
                case "say something to her":
                    {
                        if (!finalRoom)
                        {
                            Console.WriteLine("There is nobody in the room.");
                        }
                        else
                        {
                            if (stage < 2)
                            {
                                Console.WriteLine("You can't see anything.");
                            }
                            else if (stage == 2)                            
                            {
                                Console.WriteLine("The woman turns around, and it turns out to be your wife. She is okay after all.\nYou ask her what she is doing and where you are. She tells you that you \nknow what you did.\nShe pulls out a sword and runs towards you.");
                                stage = 3;
                                if (!player.inventory.Contains("flame thrower"))    //Game over man
                                {
                                    Console.WriteLine("You have nothing to defend yourself with. Your wife kills you.\nBefore you go she says:");
                                    Console.Read();
                                    Console.Clear();
                                    Console.SetCursorPosition(22, 10);
                                    Console.ForegroundColor = ConsoleColor.Red;
                                    Console.Write("You should have put down the seat");
                                    Console.Read();
                                    Console.Read();
                                    gameOver = true;
                                }
                            }
                        }
                    }
                    break;
                case "throw cake":
                    {
                        if (player.inventory.Contains("cake"))
                        {
                            if (finalRoom)
                            {
                                if (stage == 3)
                                {
                                    Console.WriteLine("You throw the cake at your wife. She eats it and keeps running.");
                                    player.inventory.Remove("cake");
                                }
                                else
                                {
                                    Console.WriteLine("You throw the cake.");
                                    player.inventory.Remove("cake");
                                }
                            }
                            else
                            {
                                Console.WriteLine("You throw the cake on the ground, that was productive.\nHappy birthday ground.");
                                player.inventory.Remove("cake");
                            }
                        }
                        else
                        {
                            Console.WriteLine("You don't have a cake.");
                        }
                    }
                    break;
                case "throw pie":
                    {
                        if (player.inventory.Contains("pie"))
                        {
                            if (finalRoom)
                            {
                                if (stage == 3)
                                {
                                    Console.WriteLine("You throw the pie at your wife. She eats it and keeps running.");
                                    player.inventory.Remove("pie");
                                }
                                else
                                {
                                    Console.WriteLine("You throw the cake.");
                                    player.inventory.Remove("cake");
                                }
                            }
                            else
                            {
                                Console.WriteLine("You throw the pie on the ground, that was productive.");
                                player.inventory.Remove("pie");
                            }
                        }
                        else
                        {
                            Console.WriteLine("You don't have a pie.");
                        }
                    }
                    break;
                case "throw apple":
                    {
                        if (player.inventory.Contains("apple"))
                        {
                            if (finalRoom)
                            {
                                if (stage == 3)
                                {
                                    Console.WriteLine("You throw the apple at your wife. She eats it and keeps running.");
                                    player.inventory.Remove("apple");
                                }
                                else
                                {
                                    Console.WriteLine("You throw the cake.");
                                    player.inventory.Remove("cake");
                                }
                            }
                            else
                            {
                                Console.WriteLine("You throw the apple on the ground, that was productive.");
                                player.inventory.Remove("apple");
                            }
                        }
                        else
                        {
                            Console.WriteLine("You don't have an apple.");
                        }
                    }
                    break;
                case "eat apple":
                    if (player.inventory.Contains("apple"))
                    {
                        Console.WriteLine("You eat the apple.");
                        player.inventory.Remove("apple");
                    }
                    else
                    {
                        Console.WriteLine("You don't have an apple.");
                    }
                    break;
                case "eat cake":
                    if (player.inventory.Contains("cake"))
                    {
                        Console.WriteLine("You eat the cake. Mmmm cake.");
                        player.inventory.Remove("cake");
                    }
                    else
                    {
                        Console.WriteLine("You don't have a cake.");
                    }
                    break;
                case "eat pie":
                    if (player.inventory.Contains("pie"))
                    {
                        Console.WriteLine("You eat the pie.");
                        player.inventory.Remove("pie");
                    }
                    else
                    {
                        Console.WriteLine("You don't have a pie.");
                    }
                    break;
                case "use flame thrower":
                case "burn wife":
                case "kill wife":
                case "kill wife with flame thrower":
                case "kill her":
                case "burn her":
                    {
                        if (player.inventory.Contains("flame thrower"))
                        {
                            if (finalRoom && stage == 3)
                            {
                                Console.WriteLine("You set your wife on fire. Thats pretty messed up man.\nBut hey, you're still alive.\nDid she put you down here? Why did she do it?\nMaybe there will be a sequel, or maybe she has to kill you to find out...");
                                Console.Read();
                                gameOver = true;
                            }
                            else
                            {
                                Console.WriteLine("There is nothing to use the flame thrower on.");
                            }
                        }
                        else
                        {
                            Console.WriteLine("You don't have a flame thrower.");
                        }
                    }
                    break;
                case "quit":
                    {
                        gameOver = true;
                    }
                    break;
                case "":
                    break;
                default:
                    Console.WriteLine("I do not understand that.");
                    break;
            }
            Console.WriteLine();
        }

        static public Room currentRoom()
        {
            return map[player.yPos(), player.xPos()];
        }

        //The room in relation to the player's current room
        static public Room roomN()
        {
            return map[player.yPos() - 1, player.xPos()];
        }
        static public Room roomS()
        {
            return map[player.yPos() + 1, player.xPos()];
        }
        static public Room roomE()
        {
            return map[player.yPos(), player.xPos() + 1];
        }
        static public Room roomW()
        {
            return map[player.yPos(), player.xPos() - 1];
        }
    }
}
