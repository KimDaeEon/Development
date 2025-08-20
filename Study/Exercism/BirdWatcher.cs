using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Exercism
{
    class BirdCount
    {
        private int[] birdsPerDay;
        private int today;
        private static int[] lastWeeksCnt = new[] { 0, 2, 5, 3, 7, 8, 4 };

        public BirdCount(int[] birdsPerDay)
        {
            this.birdsPerDay = birdsPerDay;
            today = this.birdsPerDay.Length - 1;
        }

        public static int[] LastWeek() => lastWeeksCnt;

        public int Today() => birdsPerDay[today];

        public void IncrementTodaysCount() => birdsPerDay[today]++;

        public bool HasDayWithoutBirds()
        {
            for(int i =0; i<birdsPerDay.Length; i++)
            {
                if (birdsPerDay[i] == 0)
                {
                    return true;
                }
            }

            return false;
        }

        public int CountForFirstDays(int numberOfDays)
        {
            int count = 0;
            for (int i = 0; i < numberOfDays; ++i)
            {
                count += birdsPerDay[i];
            }

            return count;
        }

        public int BusyDays()
        {
            int busyDays = 0;
            for(int i = 0; i < birdsPerDay.Length; i++)
            {
                if (birdsPerDay[i] >= 5)
                {
                    busyDays++;
                }
            }

            return busyDays;
        }
    }
}
