using System;
using System.Collections.Generic;
using System.IO;

namespace TextConverter
{
    class Program
    {
        static void Main(string[] args)
        {
            /* Variables */
            string fullpath = string.Empty;
            int height = -1;
            int width = -1;
            int depth = -1;
            bool[][][] input;
            
            WriteLine("Please enter the fullpath of the file you wish to read:", ConsoleColor.White);
            fullpath = Console.ReadLine();
            Console.WriteLine();

            /* Read file */
            List<string> read;
            try
            {
                StreamReader reader = new StreamReader(fullpath);
                read = new List<string>();
                while (!reader.EndOfStream)
                {
                    read.Add(reader.ReadLine());
                }
                reader.Close();
            }
            catch (Exception e)
            {
                WriteLine(e.ToString(), ConsoleColor.Red);
                Console.ReadLine();
                return;
            }

            /* Process list */
            int.TryParse(read[0], out height);
            int.TryParse(read[1], out width);
            int.TryParse(read[2], out depth);

            int count = 2;
            input = new bool[depth][][];
            for (int i = 0; i < depth; i++)
            {
                input[i] = new bool[width][];
                for (int j = 0; j < width; j++)
                {
                    input[i][j] = new bool[height];
                    count++;
                    for (int k = 0; k < height; k++)
                    {
                        if (read[count][k] == 'X' || read[count][k] != 'x')
                        {
                            input[i][j][k] = true;
                        }
                        else
                        {
                            input[i][j][k] = false;
                        }
                    }
                }
            }

            /* Write file */
            try
            {
                StreamWriter writer = new StreamWriter("OUTPUT" + fullpath);
                for (int i = 0; i < depth; i++)
                {
                    writer.WriteLine("Z = {0}", i);
                    writer.WriteLine("--------------------------------------------------------");
                    for (int j = 0; j < width; j++)
                    {
                        writer.Write("Y = {0}:\t", j);
                        string output = string.Empty;
                        for (int k = 0; k < height; k++)
                        {
                            if (input[i][j][k])
                            {
                                output += "00";
                            }
                            else
                            {
                                output += "01";
                            }
                        }
                        string hex = Convert.ToString(Convert.ToInt32(output, 2), 16);
                        int length = hex.Length;
                        for (int k = 0; k < (32 /4) - length; k++)
                        {
                            hex = '0' + hex;
                        }
                        writer.WriteLine("{2}{0}\tBinary: {1}", hex, output, "0x");
                    }
                    writer.WriteLine();
                }
                writer.WriteLine();
                writer.WriteLine();
                writer.Flush();
                writer.Close();
            }
            catch (Exception e)
            {
                WriteLine(e.ToString(), ConsoleColor.Red);
                Console.ReadLine();
                return;
            }

            WriteLine("Success!", ConsoleColor.Green);
            Console.ReadLine();
        }

        public static void WriteLine(string line, ConsoleColor colour)
        {
            ConsoleColor previous = Console.ForegroundColor;
            Console.ForegroundColor = colour;
            Console.WriteLine(line);
            Console.ForegroundColor = previous;
        }
    }
}
