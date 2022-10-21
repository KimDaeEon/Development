using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace BlazorApp.Data
{
    public class Food
    {
        public string Name { get; set; }
        public int Price { get; set; }
    }

    public interface IFoodService
    {
        IEnumerable<Food> GetFoods();
    }

    public class FoodService : IFoodService
    {
        public IEnumerable<Food> GetFoods()
        {
            List<Food> foods = new List<Food>()
            {
                new Food () {Name ="BibimBap", Price = 7000},
                new Food () {Name ="KimBap", Price = 3000},
                new Food () {Name ="DonKatsu", Price = 8000},
            };

            return foods;
        }
    }

    public class FastFoodService : IFoodService
    {
        public IEnumerable<Food> GetFoods()
        {
            List<Food> foods = new List<Food>()
            {
                new Food () {Name ="Hamburger", Price = 5000},
                new Food () {Name ="Fries", Price = 2000},
            };

            return foods;
        }
    }

    // 아래처럼 생성자에 IFoddService 넣어두면 StartUp 에서 알아서 채워준다.
    public class PaymentService
    {
        IFoodService _service;

        public PaymentService(IFoodService service)
        {
            _service = service;
        }
    }

    public class SingletonService : IDisposable
    {
        public Guid ID { get; set; }

        public SingletonService()
        {
            ID = Guid.NewGuid();
        }

        public void Dispose()
        {
            Console.WriteLine("SingletonService Disposed");
        }
    }

    public class TransientService : IDisposable
    {
        public Guid ID { get; set; }

        public TransientService()
        {
            ID = Guid.NewGuid();
        }

        public void Dispose()
        {
            Console.WriteLine("TransientService Disposed");
        }
    }

    public class ScopedService : IDisposable
    {
        public Guid ID { get; set; }

        public ScopedService()
        {
            ID = Guid.NewGuid();
        }

        public void Dispose()
        {
            Console.WriteLine("ScopedService Disposed");
        }
    }
}
