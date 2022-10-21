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
            Console.WriteLine("[1] Eager loading");
            Console.WriteLine("[2] Explicit Loading");
            Console.WriteLine("[3] Select Loading");


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
                        DbCommands.EagerLoading();
                        break;

                    case "2":
                        DbCommands.ExplicitLoading();
                        break;
                    
                    case "3":
                        DbCommands.SeletcLoading();
                        break;
                }
            }
        }
    }
}
