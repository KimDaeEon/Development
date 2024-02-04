// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class ProtobufLib : ModuleRules
{
    // 아래처럼 세팅해두고 Unreal Project Generate 하면 필터로 Include랑 Lib 폴더 내용들이 추가된다.
    public ProtobufLib(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        // include <> 형식으로 추가하기 위한 것, 마치 std를 include <>로 추가하듯이 쓰기 위한 코드다.
        PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "Include")); // ModleDirectory 는 여기서 ProtobufLib 폴더를 말한다.

        // include "" 형식으로 추가하기 위한 것
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Lib", "Win64", "libprotobuf.lib")); // 추후 윈도우 이외의 버전 구분을 위해 폴더 구분

        PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI=1"); // 런타임 타입 정보(RTTI)를 사용하지 않도록 설정, RTTI를 사용하지 않는 경우 성능을 향상시킬 수 있음
    }
}
