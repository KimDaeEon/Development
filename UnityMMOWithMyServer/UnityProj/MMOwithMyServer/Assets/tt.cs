using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class tt : MonoBehaviour
{
    [DllImport("MyGameServerDLL.dll")]
    public static extern void test();
    
    [DllImport("MyGameServerDLL.dll")]
    public static extern int test2(int a, int b);

    [DllImport("MyGameServerDLL.dll",CallingConvention = CallingConvention.Cdecl)]
    public static extern myclass createMyClass(int a, int b);



    // Start is called before the first frame update
    void Start()
    {
        auto a = createMyClass(10, 20);
        Debug.Log(test2(20, 5));
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
