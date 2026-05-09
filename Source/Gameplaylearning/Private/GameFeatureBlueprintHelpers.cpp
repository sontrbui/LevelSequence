// Implementation of asynchronous GameFeature library for runtime plugin management
// Provides async nodes for Blueprint usage with proper result callbacks

#include "GameFeatureBlueprintHelpers.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeaturePluginOperationResult.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameFeatureLibrary, Log, All);

//////////////////////////////////////////////////////////////////////
// UGameFeatureHelpers - Synchronous Helper Functions
//////////////////////////////////////////////////////////////////////

bool UGameFeatureHelpers::GetPluginURLByName(const FString& PluginName, FString& OutPluginURL)
{
	if (PluginName.IsEmpty())
	{
		UE_LOG(LogGameFeatureLibrary, Warning, TEXT("GetPluginURLByName called with empty PluginName"));
		return false;
	}

	// Use the engine's GameFeatures subsystem helper to map a short name to a URL
	if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, /*out*/ OutPluginURL))
	{
		return true;
	}
	else
	{
		UE_LOG(LogGameFeatureLibrary, Warning, TEXT("Unable to find GameFeature plugin URL for name '%s'"), *PluginName);
		OutPluginURL.Reset();
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
// UAsyncLoadAndActivateGameFeature - Asynchronous Load/Activate
//////////////////////////////////////////////////////////////////////

UAsyncLoadAndActivateGameFeature* UAsyncLoadAndActivateGameFeature::LoadAndActivateGameFeatureByURL(const FString& InPluginURL, UGameFeaturesSubsystem* GFSubsystem)
{
	UAsyncLoadAndActivateGameFeature* Node = NewObject<UAsyncLoadAndActivateGameFeature>();
	Node->PluginURL = InPluginURL;
	Node->GameFeaturesSubsystem = GFSubsystem;
	// Returned node will have Activate called by Blueprint VM
	return Node;
}

void UAsyncLoadAndActivateGameFeature::Activate()
{
	if (PluginURL.IsEmpty())
	{
		UE_LOG(LogGameFeatureLibrary, Warning, TEXT("LoadAndActivateGameFeatureByURL called with empty PluginURL"));
		OnCompleted.Broadcast(false);
		return;
	}

	UE_LOG(LogGameFeatureLibrary, Log, TEXT("Async: Requesting LoadAndActivateGameFeature for URL: %s"), *PluginURL);

	GameFeaturesSubsystem->LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateWeakLambda(this, [this](const UE::GameFeatures::FResult& Result)	
	{
		const bool bSuccess = Result.HasValue();
		UE_LOG(LogGameFeatureLibrary, Log, TEXT("Async: LoadAndActivateGameFeature completed for: %s (Success=%d)"), *PluginURL, (int)bSuccess);
		OnCompleted.Broadcast(bSuccess);
	}));
}

//////////////////////////////////////////////////////////////////////
// UAsyncDeactivateGameFeature - Asynchronous Deactivate & Unload
//////////////////////////////////////////////////////////////////////

UAsyncDeactivateGameFeature* UAsyncDeactivateGameFeature::UnloadGameFeatureByURL(const FString& InPluginURL, UGameFeaturesSubsystem* GFSubsystem)
{
	UAsyncDeactivateGameFeature* Node = NewObject<UAsyncDeactivateGameFeature>();
	Node->PluginURL = InPluginURL;
	Node->GameFeaturesSubsystem = GFSubsystem;
	// Returned node will have Activate called by Blueprint VM
	return Node;
}

void UAsyncDeactivateGameFeature::Activate()
{
	if (PluginURL.IsEmpty())
	{
		UE_LOG(LogGameFeatureLibrary, Warning, TEXT("DeactivateGameFeatureByURL called with empty PluginURL"));
		OnCompleted.Broadcast(false);
		return;
	}

	UE_LOG(LogGameFeatureLibrary, Log, TEXT("Async: Requesting DeactivateGameFeature for URL: %s"), *PluginURL);

	// Use UnloadGameFeaturePlugin to properly trigger cleanup in Game Feature Actions
	GameFeaturesSubsystem->UnloadGameFeaturePlugin(PluginURL, FGameFeaturePluginChangeStateComplete::CreateWeakLambda(this, [this](const UE::GameFeatures::FResult& Result)	
	{
		const bool bSuccess = Result.HasValue();
		UE_LOG(LogGameFeatureLibrary, Log, TEXT("Async: DeactivateGameFeature completed for: %s (Success=%d)"), *PluginURL, (int)bSuccess);
		OnCompleted.Broadcast(bSuccess);
	}), true);
}

