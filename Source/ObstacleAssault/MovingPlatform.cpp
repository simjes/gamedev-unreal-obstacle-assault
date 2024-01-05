// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "GameFramework/Character.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
}

void AMovingPlatform::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
 	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
 {
	if (!EnableKnockback)
	{
		return;
	}
 	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    const FVector Knockback = -HitNormal * FVector(5000, 5000, 1000);
	ACharacter* PlayerCharacter = Cast<ACharacter>(Other);
	
	if (PlayerCharacter)
	{
		PlayerCharacter->LaunchCharacter(Knockback, true, false);
	}
 }

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	if (ShouldPlatformReturn())
	{
		const FVector MoveDirection = PlatformVelocity.GetSafeNormal();
		StartLocation = StartLocation + MoveDirection * MoveDistance;
		SetActorLocation(StartLocation);
		PlatformVelocity = -PlatformVelocity;
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation = CurrentLocation + PlatformVelocity * DeltaTime;
	SetActorLocation(CurrentLocation);
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	// UE_LOG(LogTemp, Display, TEXT("%s Rotating..."), *GetName());
	AddActorLocalRotation(RotationVelocity * DeltaTime);
}

bool AMovingPlatform::ShouldPlatformReturn() const
{
	// const double Overshoot = TravelDistance - MoveDistance;
	// const FString Name = GetName();
	// UE_LOG(LogTemp, Display, TEXT("%s overshoot: %f"),  *Name, Overshoot);
	return  GetTravelDistance() > MoveDistance;
}

float AMovingPlatform::GetTravelDistance() const
{
	return FVector::Dist(StartLocation, GetActorLocation());
}

