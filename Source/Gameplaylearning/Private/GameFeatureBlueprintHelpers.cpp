// Implementation of asynchronous GameFeature library for runtime plugin management
// Provides async nodes for Blueprint usage with proper result callbacks

#include "GameFeatureBlueprintHelpers.h"
#include "GameFeaturesSubsystem.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameFeatureLibrary, Log, All);

namespace
{
	// Detection helper to check if FResult has HasError method across engine versions
	template <typename T>
	static auto Has_HasError(int) -> decltype(std::declval<T>().HasError(), std::true_type());

	template <typename T>
	static auto Has_HasError(...) -> std::false_type;

	static bool TryResultSuccess(const UE::GameFeatures::FResult& Result)
	{
		using R = UE::GameFeatures::FResult;

		if constexpr (decltype(Has_HasError<R>(0))::value)
		{
			return !Result.HasError();
		}
		else
		{
			// Unknown API - conservatively assume success
			return true;
		}
	}
}

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

UAsyncLoadAndActivateGameFeature* UAsyncLoadAndActivateGameFeature::LoadAndActivateGameFeatureByURL(const FString& InPluginURL)
{
	UAsyncLoadAndActivateGameFeature* Node = NewObject<UAsyncLoadAndActivateGameFeature>();
	Node->PluginURL = InPluginURL;
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

	UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::HandleResult));
}

void UAsyncLoadAndActivateGameFeature::HandleResult(const UE::GameFeatures::FResult& Result)
{
	const bool bSuccess = TryResultSuccess(Result);
	UE_LOG(LogGameFeatureLibrary, Log, TEXT("Async: LoadAndActivateGameFeature completed for: %s (Success=%d)"), *PluginURL, (int)bSuccess);
	OnCompleted.Broadcast(bSuccess);

	// Let Blueprint VM clean up the node when appropriate.
}

//////////////////////////////////////////////////////////////////////
// UAsyncDeactivateGameFeature - Asynchronous Deactivate & Unload
//////////////////////////////////////////////////////////////////////

UAsyncDeactivateGameFeature* UAsyncDeactivateGameFeature::DeactivateAndUnloadGameFeatureByURL(const FString& InPluginURL)
{
	UAsyncDeactivateGameFeature* Node = NewObject<UAsyncDeactivateGameFeature>();
	Node->PluginURL = InPluginURL;
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
	UGameFeaturesSubsystem::Get().UnloadGameFeaturePlugin(PluginURL, FGameFeaturePluginChangeStateComplete::CreateUObject(this, &ThisClass::HandleResult));
}

void UAsyncDeactivateGameFeature::HandleResult(const UE::GameFeatures::FResult& Result)
{
	const bool bSuccess = TryResultSuccess(Result);
	UE_LOG(LogGameFeatureLibrary, Log, TEXT("Async: DeactivateGameFeature completed for: %s (Success=%d)"), *PluginURL, (int)bSuccess);
	OnCompleted.Broadcast(bSuccess);

	// Let Blueprint VM clean up the node when appropriate.
}
