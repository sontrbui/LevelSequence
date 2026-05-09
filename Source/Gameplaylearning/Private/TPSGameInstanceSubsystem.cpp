// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstanceSubsystem.h"

#include "GameFeaturePluginOperationResult.h"
#include "GameFeaturesSubsystem.h"

void UTPSGameInstanceSubsystem::LoadAndActivateGameFeaturePlugin(const FString& PluginName)
{
	if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
	{
		UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateWeakLambda(this,[this, PluginName](const UE::GameFeatures::FResult& Result)
		{
			const bool bSuccess = Result.HasValue();
			if (!bSuccess)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to load and activate plugin %s: %s"), *PluginName, *Result.GetError());
			}
			else
			{
				OnPluginLoadingCompleted.Broadcast(PluginURL, bSuccess);
			}
		}));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Plugin %s was not found to load"), *PluginName);
	}
}

void UTPSGameInstanceSubsystem::UnloadGameFeaturePlugin(const FString& PluginName)
{
	if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
	{
		UGameFeaturesSubsystem::Get().UnloadGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateWeakLambda(this,[this, PluginName](const UE::GameFeatures::FResult& Result)
		{
			const bool bSuccess = Result.HasValue();
			if (!bSuccess)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to unload plugin %s: %s"), *PluginName, *Result.GetError());
			}
			else
			{
				OnPluginLoadingCompleted.Broadcast(PluginURL, bSuccess);
			}
		}));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Plugin %s was not found to unload"), *PluginName);
	}
}

void UTPSGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
	if (!PluginURL.IsEmpty())
	{
		UGameFeaturesSubsystem::Get().UnloadGameFeaturePlugin(PluginURL, true);
	}	
}
