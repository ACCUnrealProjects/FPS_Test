// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponTypeEnum.h"
#include "WeaponControllerComponet.generated.h"

class ABase_Weapon;
class USkeletalMeshComponent;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARENASHOOTER_API UWeaponControllerComponet : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponControllerComponet();

private:
	bool IsFiring = false;

	// Info for where to attach new guns to
	USkeletalMeshComponent* WeaponAttachSkel = nullptr;
	FString PointInSkel;

	FActorSpawnParameters SpawnParams;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<ABase_Weapon*> MyEquipedGuns;

	UPROPERTY(BlueprintReadOnly, Category = "Weapons")
	ABase_Weapon* CurrentWeapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TMap<GunType, UAnimMontage*> FireAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TMap<GunType, UAnimMontage*> ReloadAnimations;


public:	
	//Adding New Gun
	void AddGun(TSubclassOf<ABase_Weapon> NewWeapon);
	void SetAttachSkel(USkeletalMeshComponent* AttachWeaponTo, FString PointToAttachTo);
	bool DoIAlreadyHaveGun(GunType NewGunType);

	//Get current gun
	ABase_Weapon* GetCurrentGun();

	//Weapon Controls
	void FireCurrentWeapon(FVector FirePoint, FRotator FireDirRotator);
	void Reload();
	void StopFire();
	void ChangeGun(int8 WeaponNum);
};