// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Public/Weapons/Base_Weapon.h"
#include "RayGun.generated.h"


UCLASS()
class ARENASHOOTER_API ARayGun : public ABase_Weapon
{
	GENERATED_BODY()
	
private:

protected:

	float Range = 10000.0f;

public:

	ARayGun();

	virtual void Fire(FVector FirePoint, FRotator FireDirRotator) override;

	virtual void OnAttach(AActor* MyOwner) override;

};