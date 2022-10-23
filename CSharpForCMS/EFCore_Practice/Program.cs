using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Storage;
using System;

namespace EFCore_Practice
{
    class Program
    {

        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            DbCommands.InitializeDB(resetDb: false);

            Console.WriteLine("명령어를 입력하세요.");
            Console.WriteLine("[0] DB Reset");
            
            while (true)
            {
                Console.Write(">> ");
                string command = Console.ReadLine();

                switch (command)
                {
                    case "0":
                        DbCommands.InitializeDB(resetDb:true);
                        break;

                    case "1":
                        DbCommands.SoftDeleteTest();
                        break;

                    case "2":
                        break;
                    
                    case "3":
                        break;
                }
            }
        }
    }
}
