// Fill out your copyright notice in the Description page of Project Settings.


#include "HCTButtonBase.h"
#include "Components/BoxComponent.h"


// Sets default values
AHCTButtonBase::AHCTButtonBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Firstly, create root scene component so that we can still change the box collision relative position
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot); // or RootComponent = SceneRoot;
	
	// Create, parent, and configure box collision component
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AHCTButtonBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind overlap events
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AHCTButtonBase::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AHCTButtonBase::OnOverlapEnd);
}

// Called every frame
void AHCTButtonBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AHCTButtonBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		BuocChanVaoGianHang.Broadcast(OtherActor);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AHCTButtonBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		BuocChanRaKhoiGianHang.Broadcast(OtherActor);
	}
}
