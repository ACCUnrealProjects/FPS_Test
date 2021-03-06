// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UCLASS()
class ARENASHOOTER_API AZombie : public ACharacter
{
	GENERATED_BODY()

private:

	//Punch info
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float PunchCoolDown = 1.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	int32 PunchDamage = 10.0f;

	bool CanPunch = true;
	bool IsDying = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthComponent", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* MyHealthComp = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackAnimation;

	class USkeletalMeshComponent* ZombieSkel;
	class UCharacterMovementComponent* MyMoveComp = nullptr;
	class UAIPerceptionStimuliSourceComponent* Stimulus = nullptr;

	class UWidgetComponent* MyHealthBar = nullptr;

	TSubclassOf<class APickUps> Drop = nullptr;

	void SetUpMyStimulis();
	void PunchCooldownComplete();
	void DisableHealthBar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void RagDoll();

	void DestroyMe();

public:	

	// Sets default values for this character's properties
	AZombie();

	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	void MeleeAttack(AActor* Target);

	void SetDrop(TSubclassOf<class APickUps> MyDrop);

	UFUNCTION()
	void ImHit();

	UFUNCTION()
	void Death();

};
