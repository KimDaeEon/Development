// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h" // TCP ������ ���� �������� �����, ���� ���� �����ϰ� �ϵ��� ������
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h" // �÷��� �������� ���� �ý����� ���� �������̽� ����

void UMyGameInstance::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address ip;
	FIPv4Address::Parse(IpAddress, ip);

	TSharedRef<FInternetAddr> internetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	internetAddr->SetIp(ip.Value);
	internetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting to %s:%d"), *IpAddress, Port));

	bool connected = Socket->Connect(*internetAddr);

	if (connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Connection Success"));

		// Session �߰�

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to connect to server"));
	}
}

void UMyGameInstance::DisconnectFromGameServer()
{
}
