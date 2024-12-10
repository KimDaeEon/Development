using System;
using System.Collections;
using System.Net;
using System.Text;
using Newtonsoft.Json;
using UnityEngine;
using UnityEngine.Networking;

public class CertificateWhore : CertificateHandler
{
    protected override bool ValidateCertificate(byte[] certificateData)
    {
        return true;
    }
}

public class WebManager
{
    // TODO: 아래 부분도 Config로 빼서 웹 서버용 URL 넣을 수 있도록 해야할 듯
    public string BaseUrl { get; set; }
    public string ip = "127.0.0.1";
    public int port = 8888;
    
    public void Init()
    {
        IPAddress ipv4 = Utils.GetIpv4Address(ip);
        if (ipv4 == null)
        {
            Debug.LogError("WebServer IPv4 Failed");
            return;
        }
	
        BaseUrl = $"http://{ipv4.ToString()}:{port}";
        Debug.Log($"WebServer BaseUrl : {BaseUrl}");
    }
    
    public void SendPostRequest<T>(string url, object obj, Action<T> res)
    {
        Managers.Instance.StartCoroutine(CoSendWebRequest(url, UnityWebRequest.kHttpVerbPOST, obj, res));
    }

    public void SendGetRequest<T>(string url, object obj, Action<T> res)
    {
        Managers.Instance.StartCoroutine(CoSendWebRequest(url, UnityWebRequest.kHttpVerbGET, obj, res));
    }

    IEnumerator CoSendWebRequest<T>(string url, string method, object obj, Action<T> res)
    {
        if (string.IsNullOrEmpty(BaseUrl))
            Init();

        // ipAddress/test/hello
        string sendUrl = $"{BaseUrl}/{url}";

        byte[] jsonBytes = null;
        if (obj != null)
        {
            string jsonStr = JsonConvert.SerializeObject(obj);
            jsonBytes = Encoding.UTF8.GetBytes(jsonStr);
        }

        using (var uwr = new UnityWebRequest(sendUrl, method))
        {
            uwr.uploadHandler = new UploadHandlerRaw(jsonBytes); // 요청 데이터 서버로 전송하는 역할
            uwr.downloadHandler = new DownloadHandlerBuffer(); // 서버로 부터 응답 데이터를 수신하는 역할
            uwr.certificateHandler = new CertificateWhore(); // SSL 인증 관련
            uwr.SetRequestHeader("Content-Type", "application/json"); // Json 포맷으로 웹 요청을 보낸다고 지정

            yield return uwr.SendWebRequest();

            if (uwr.result == UnityWebRequest.Result.ConnectionError)
            {
                Debug.Log($"CoSendWebRequest Failed : {uwr.error}");
            }
            else
            {
                Debug.Log($"CoSendWebRequest Failed : {uwr.error}");
                T resObj = JsonUtility.FromJson<T>(uwr.downloadHandler.text);
                res.Invoke(resObj);
            }
        }
    }
}
