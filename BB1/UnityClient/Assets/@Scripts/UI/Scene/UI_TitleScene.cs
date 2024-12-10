using Google.Protobuf.Protocol;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using static CustomEnum;
using Object = UnityEngine.Object;

// TODO: �Ʒ��� UI�� ��Ʈ��ũ, ���� �ڵ�� ���� �ִµ� Unity ���� ���� �� �� ã�ƺ��� UI�� �и��ϴ� ������� ���� ��ġ�ϱ�
public class UI_TitleScene : UI_Scene
{
	private enum GameObjects
	{
		StartButton,
	}

	private enum Texts
	{
		StatusText,
	}

	private enum TitleSceneState
	{
		None,
		AssetLoading,
		AssetLoaded,
		ConnectingToServer,
		ConnectedToServer,
		FailedToConnectToServer,
	}

	TitleSceneState _state = TitleSceneState.None;
	TitleSceneState State
	{
		get { return _state; }
		set
		{
			_state = value;
			switch (value)
			{
				case TitleSceneState.None:
					break;
				case TitleSceneState.AssetLoading:
					GetText((int)Texts.StatusText).text = $"TODO �ε���";
					break;
				case TitleSceneState.AssetLoaded:
					GetText((int)Texts.StatusText).text = "TODO �ε� �Ϸ�";
					break;
				case TitleSceneState.ConnectingToServer:
					GetText((int)Texts.StatusText).text = "TODO ���� ������";
					break;
				case TitleSceneState.ConnectedToServer:
					GetText((int)Texts.StatusText).text = "TODO ���� ���� ����";
					break;
				case TitleSceneState.FailedToConnectToServer:
					GetText((int)Texts.StatusText).text = "TODO ���� ���� ����";
					break;
			}
		}
	}

	protected override void Awake()
	{
		base.Awake();

		BindObjects(typeof(GameObjects));
		BindTexts(typeof(Texts));

		GetObject((int)GameObjects.StartButton).BindEvent((evt) =>
		{
			Debug.Log("OnClick");
			Managers.Scene.LoadScene(EScene.GameScene);
		});

		GetObject((int)GameObjects.StartButton).gameObject.SetActive(false);
	}

	protected override void Start()
	{
		base.Start(); 

		// Load ����
		State = TitleSceneState.AssetLoading;

		Managers.Resource.LoadAllDataAsync<Object>("Preload", (key, count, totalCount) =>
		{
            GetText((int)Texts.StatusText).text = $"TODO �ε��� : {key} {count}/{totalCount}";

			if (count == totalCount)
			{
				OnAssetLoaded();
			}
		});
	}

	private void OnAssetLoaded()
	{
		State = TitleSceneState.AssetLoaded;
		Managers.Data.Init();

		Debug.Log("Connecting To Server");
		State = TitleSceneState.ConnectingToServer;

		// TODO: �Ʒ� ���뵵 config ���Ϸ� ������
		IPAddress ipAddr = IPAddress.Parse("127.0.0.1");
		IPEndPoint endPoint = new IPEndPoint(ipAddr, 8888);
		Managers.Network.GameServer.Connect(endPoint, OnConnectionSuccess, OnConnectionFailed);
	}

	private void OnConnectionSuccess()
	{
		Debug.Log("Connected To Server");
		State = TitleSceneState.ConnectedToServer;

		GetObject((int)GameObjects.StartButton).gameObject.SetActive(true);

		StartCoroutine(CoSendTestPackets());
	}

	private void OnConnectionFailed()
	{
		Debug.Log("Failed To Connect To Server");
		State = TitleSceneState.FailedToConnectToServer;
	}

	IEnumerator CoSendTestPackets()
	{
		while (true)
		{
			yield return new WaitForSeconds(1);

			C_Test pkt = new C_Test();
			pkt.V = 1;
			Managers.Network.Send(pkt);
		}
	}
}
