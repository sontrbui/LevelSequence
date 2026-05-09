// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSGameInstanceSubsystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFPSGameInstanceEvent, const FString&, InPluginURL, bool, bSuccess);

UCLASS()
class GAMEPLAYLEARNING_API UTPSGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FFPSGameInstanceEvent OnPluginLoadingCompleted;
	
	UFUNCTION(BlueprintCallable, Category = "Game Instance Subsystem")
	void LoadAndActivateGameFeaturePlugin(const FString& PluginName);
	
	UFUNCTION(BlueprintCallable, Category = "Game Instance Subsystem")
	void UnloadGameFeaturePlugin(const FString& PluginName);
	
	virtual void Deinitialize() override;
	
protected:
	FString PluginURL;
};
