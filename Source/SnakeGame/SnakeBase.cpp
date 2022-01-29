// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"

#include <algorithm>

#include "Food.h"
#include "Bonus.h"
#include "SnakeElementBase.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 10.f;
	NextMovementDirection = EMovementDirection::DOWN;
	srand(time(NULL));
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(4);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
}

void ASnakeBase::AddSnakeElement(int ElementNum)
{
	for (int i = 0; i < ElementNum; i++)
	{
		FVector NewLocation;
		const int32 Num = SnakeElements.Num();
		if (Num == 0)
		{
			NewLocation = FVector(0, 0, 0);
		}
		else
		{
			NewLocation = SnakeElements[Num - 1]->GetActorLocation();
		}
		FTransform NewTransform(NewLocation);
		ASnakeElementBase* NewElement = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewElement->SnakeOwner = this;
		const int32 ElemIndex = SnakeElements.Add(NewElement);
		if (ElemIndex == 0)
		{
			NewElement->SetFirstElementType();
		}
	}
	SpawnFood();
}

void ASnakeBase::SpawnFood()
{
	const auto FoodActor = GetWorld()->SpawnActor<AFood>(FoodActorClass, FTransform());
	FoodActor->SetActorLocation(GenerateLocation());
}

void ASnakeBase::SpawnBonus()
{
	const auto BonusActor = GetWorld()->SpawnActor<ABonus>(BonusActorClass, FTransform());
	BonusActor->SetActorLocation(GenerateLocation());
}

void ASnakeBase::SetBonus(bool bIsNegativeBonus)
{
	if (bIsNegativeBonus)
	{
		MovementSpeed = std::min(Max_Speed, MovementSpeed + Delta_Speed);
		SetActorTickInterval(MovementSpeed);
	}
	else
	{
		MovementSpeed = std::max(Min_Speed, MovementSpeed - Delta_Speed);
		SetActorTickInterval(MovementSpeed);
	}
}

void ASnakeBase::Move()
{
	FVector MovementVector(ForceInitToZero);
	switch (NextMovementDirection)
	{
	case EMovementDirection::UP:
		MovementVector.X += ElementSize;
		break;
	case EMovementDirection::DOWN:
		MovementVector.X -= ElementSize;
		break;
	case EMovementDirection::LEFT:
		MovementVector.Y += ElementSize;
		break;
	case EMovementDirection::RIGHT:
		MovementVector.Y -= ElementSize;
		break;
	}

	SnakeElements[0]->ToggleCollision();
	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		const auto CurrentElement = SnakeElements[i];
		const auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
	}

	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();
	LastMovementDirection = NextMovementDirection;
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* OtherActor)
{
	if (IsValid(OverlappedElement))
	{
		const bool IsFirst = SnakeElements.Find(OverlappedElement) == 0;
		IInteractable* InteractableInterface = Cast<IInteractable>(OtherActor);
		if (InteractableInterface)
		{
			InteractableInterface->Interact(this, IsFirst);
		}
	}
}

void ASnakeBase::ChangeMovementDirection(const EMovementDirection& Direction)
{
	NextMovementDirection = Direction;
}

EMovementDirection ASnakeBase::GetMovementDirection()
{
	return LastMovementDirection;
}

int ASnakeBase::GetRandom()
{
	return (rand() % 17 - 8) * ElementSize;
}

FVector ASnakeBase::GenerateLocation()
{
	const int X = GetRandom();
	const int Y = GetRandom();
	for (const ASnakeElementBase* SnakeElement : SnakeElements)
	{
		const FVector Location = SnakeElement->GetActorLocation();
		if (Location.X == X && Location.Y == Y)
		{
			return GenerateLocation();
		}
	}
	return FVector(X, Y, 0);
}
