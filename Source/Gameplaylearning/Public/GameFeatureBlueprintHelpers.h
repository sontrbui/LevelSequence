// Asynchronous Blueprint-accessible wrappers around the Engine GameFeatures subsystem
// Provides asynchronous functions to manage GameFeature plugins at runtime.
// Supports loading, activation, deactivation (with cleanup), and URL lookup with Blueprint callbacks for success/failure scenarios.

#pragma once

#include "CoreMinimal.h"
#include "GameFeaturesSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameFeatureBlueprintHelpers.generated.h"

// Delegate for async operation results
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameFeatureOperationResult, bool, bSuccess);

/**
 * Synchronous Blueprint-callable helper to get GameFeature plugin URLs by name.
 */
UCLASS()
class GAMEPLAYLEARNING_API UGameFeatureHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Try to look up the plugin URL for a plugin registered with the GameFeatures system (by plugin name). */
	UFUNCTION(BlueprintCallable, Category = "Game Features")
	static bool GetPluginURLByName(const FString& PluginName, FString& OutPluginURL);
};

/**
 * Asynchronous Blueprint node to load and activate a GameFeature plugin by URL.
 * Broadcasts completion results (success or failure) when the operation completes.
 */
UCLASS()
class GAMEPLAYLEARNING_API UAsyncLoadAndActivateGameFeature : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Fired when the plugin load/activation completes with success or failure result. */
	UPROPERTY(BlueprintAssignable)
	FOnGameFeatureOperationResult OnCompleted;

	/** The URL that will be requested. */
	UPROPERTY(BlueprintReadOnly)
	FString PluginURL;
	
	UPROPERTY(BlueprintReadOnly)
	UGameFeaturesSubsystem* GameFeaturesSubsystem;

	/** Create and start an async request to load+activate the plugin by URL. */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Game Features")
	static UAsyncLoadAndActivateGameFeature* LoadAndActivateGameFeatureByURL(const FString& InPluginURL, UGameFeaturesSubsystem* GFSubsystem);

	// UBlueprintAsyncActionBase
	virtual void Activate() override;
	
};

/**
 * Asynchronous Blueprint node to deactivate and unload a GameFeature plugin by URL.
 * This properly triggers cleanup in Game Feature Actions (removes widgets, level instances, etc.).
 * Broadcasts completion results (success or failure) when the operation completes.
 */
UCLASS()
class GAMEPLAYLEARNING_API UAsyncDeactivateGameFeature : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Fired when the plugin deactivation and unload completes with success or failure result. */
	UPROPERTY(BlueprintAssignable)
	FOnGameFeatureOperationResult OnCompleted;

	/** The URL that will be deactivated and unloaded. */
	UPROPERTY(BlueprintReadOnly)
	FString PluginURL;
	
	UPROPERTY(BlueprintReadOnly)
	UGameFeaturesSubsystem* GameFeaturesSubsystem;

	/** Create and start an async request to deactivate and unload the plugin by URL. */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Game Features")
	static UAsyncDeactivateGameFeature* UnloadGameFeatureByURL(const FString& InPluginURL, UGameFeaturesSubsystem* GFSubsystem);

	// UBlueprintAsyncActionBase
	virtual void Activate() override;
	
};
