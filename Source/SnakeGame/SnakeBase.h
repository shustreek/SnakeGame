// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeBase.generated.h"

class ABonus;
class AFood;
class ASnakeElementBase;

UENUM()
enum class EMovementDirection { UP, DOWN, LEFT, RIGHT };

UCLASS()
class SNAKEGAME_API ASnakeBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASnakeBase();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeElementBase> SnakeElementClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFood> FoodActorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABonus> BonusActorClass;

	UPROPERTY(EditDefaultsOnly)
	float ElementSize;

	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed;

	UPROPERTY()
	TArray<ASnakeElementBase*> SnakeElements;

	UPROPERTY(EditDefaultsOnly)
	float Delta_Speed = 0.1;
	UPROPERTY(EditDefaultsOnly)
	float Max_Speed = 10.5;
	UPROPERTY(EditDefaultsOnly)
	float Min_Speed = 9.5;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	EMovementDirection LastMovementDirection;

	EMovementDirection NextMovementDirection;

	void SpawnFood();

	FVector GenerateLocation();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AddSnakeElement(int ElementNum = 1);

	UFUNCTION(BlueprintCallable)
	void Move();

	UFUNCTION()
	void SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* OtherActor);

	UFUNCTION()
	void ChangeMovementDirection(const EMovementDirection& Direction);

	UFUNCTION()
	EMovementDirection GetMovementDirection();

	UFUNCTION()
	int GetRandom();

	UFUNCTION(BlueprintCallable)
	void SpawnBonus();

	void SetBonus(bool bIsNegativeBonus);
};
