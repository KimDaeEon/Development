using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace AccountServer.Utils
{
    public class EncryptionHelper
    {
        public static string EncryptionSHA256(string message)
        {
            // 입력받은 문자열을 바이트배열로 변환
            byte[] array = Encoding.Default.GetBytes(message);
            byte[] hashValue;
            string result = string.Empty;

            // 바이트배열을 암호화된 32byte 해쉬값으로 생성
            using (SHA256 hash256 = SHA256.Create())
            {
                hashValue = hash256.ComputeHash(array);
            }
            // 32byte 해쉬값을 16진수로변환하여 64자리로 만든다.
            for (int i = 0; i < hashValue.Length; i++)
            {
                result += hashValue[i].ToString("x2");
            }
            return result;
        }
    }
}
