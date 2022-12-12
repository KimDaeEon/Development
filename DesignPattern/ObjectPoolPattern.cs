using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ObjectPoolPattern
{
    public class PooledObject
    {
        public PooledObject(object data)
        {
            _data = data;
        }
        public bool IsUsed { get; set; } = false;
        public bool IsDead { get; set; } = false; // 임시용
        public PooledObject Next { get; set; } = null;

        object _data;

    }

    public class Pool
    {
        private PooledObject _first = null;

        private List<PooledObject> objects;

        public Pool()
        {
            objects = new List<PooledObject> { 
                new PooledObject(new object()), 
                new PooledObject(new object()), 
                new PooledObject(new object()), 
                new PooledObject(new object()), 
            };

            for (int i = 0; i < objects.Count - 1; i++)
            {
                objects[i].Next = objects[i + 1];
            }

            objects[objects.Count - 1] = null;
            _first = objects[0];
        }

        public PooledObject Create()
        {
            if(_first == null)
            {
                return null;
            }

            // 이렇게 함으로써 pool 에서 사용가능한 요소를 바로 가져올 수 있다.
            PooledObject newObject = _first;
            _first = _first.Next;

            return newObject;
        }

        public void Update()
        {
            for(int i = 0; i < objects.Count; i++)
            {
                if(objects[i].IsUsed && objects[i].IsDead)
                {
                    // 다 쓴 요소를 반환, pool 의 맨 앞으로 배치
                    objects[i].Next = _first;
                    _first = objects[i];
                }
            }
        }
    }
}
