// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class ProtobufLib : ModuleRules
{
    // �Ʒ�ó�� �����صΰ� Unreal Project Generate �ϸ� ���ͷ� Include�� Lib ���� ������� �߰��ȴ�.
    public ProtobufLib(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        // include <> �������� �߰��ϱ� ���� ��, ��ġ std�� include <>�� �߰��ϵ��� ���� ���� �ڵ��.
        PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "Include")); // ModleDirectory �� ���⼭ ProtobufLib ������ ���Ѵ�.

        // include "" �������� �߰��ϱ� ���� ��
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Lib", "Win64", "libprotobuf.lib")); // ���� ������ �̿��� ���� ������ ���� ���� ����

        PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI=1"); // ��Ÿ�� Ÿ�� ����(RTTI)�� ������� �ʵ��� ����, RTTI�� ������� �ʴ� ��� ������ ����ų �� ����
    }
}
