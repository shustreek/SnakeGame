// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 10.f;
	LastMovementDirection = EMovementDirection::DOWN;
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
		FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform(NewLocation);
		ASnakeElementBase* NewElement = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewElement->SnakeOwner = this;
		const int32 ElemIndex = SnakeElements.Add(NewElement);
		if (ElemIndex == 0)
		{
			NewElement->SetFirstElementType();
		}
	}
}

void ASnakeBase::Move()
{
	FVector MovementVector(ForceInitToZero);
	switch (LastMovementDirection)
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
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* OtherActor)
{
	if (IsValid(OverlappedElement))
	{
		bool IsFirst = SnakeElements.Find(OverlappedElement) == 0;
		IInteractable* InteractableInterface = Cast<IInteractable>(OtherActor);
		if (InteractableInterface)
		{
			InteractableInterface->Interact(this, IsFirst);
		}
	}
}
