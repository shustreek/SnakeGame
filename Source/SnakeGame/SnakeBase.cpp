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
	LastMovementDirection = EMovementDirection::UP;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	AddSnakeElement(4);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move(DeltaTime);
}

void ASnakeBase::AddSnakeElement(int ElementNum)
{
	for (int i = 0; i < ElementNum; i++)
	{
		FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform(/*GetActorLocation() - */NewLocation);
		ASnakeElementBase* NewElement = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewElement->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		SnakeElements.Add(NewElement);
	}
}

void ASnakeBase::Move(float DeltaTime)
{
	float MovementSpeedDelta = MovementSpeed * DeltaTime;
	FVector MovementVector(0, 0, 0);
	switch (LastMovementDirection)
	{
	case EMovementDirection::UP:
		MovementVector.X += MovementSpeedDelta;
		break;
	case EMovementDirection::DOWN:
		MovementVector.X -= MovementSpeedDelta;
		break;
	case EMovementDirection::LEFT:
		MovementVector.Y += MovementSpeedDelta;
		break;
	case EMovementDirection::RIGHT:
		MovementVector.Y -= MovementSpeedDelta;
		break;
	}
	AddActorWorldOffset(MovementVector);
}
