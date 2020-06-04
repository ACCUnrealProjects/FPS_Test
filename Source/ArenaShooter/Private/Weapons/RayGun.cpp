// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Weapons/RayGun.h"
#include "DrawDebugHelpers.h"

ARayGun::ARayGun()
{
	myWeaponType = GunType::RayGun;

	ClipSize = 1;
	MaxAmmo = 30;
	CurrentTotalAmmo = MaxAmmo - ClipSize;
	CurrentClipAmmo = ClipSize;

	FireRate = 1.0f;
	TotalReloadTime = 1.0f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(TEXT("SkeletalMesh'/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	GunMesh->SetSkeletalMesh(MeshAsset.Object);
	Muzzle->SetRelativeLocation(FVector(0.0f, 55.0f, 10.0f));
}

void ARayGun::Fire(FVector FirePoint, FRotator FireDirRotator)
{
	ABase_Weapon::Fire(FirePoint, FireDirRotator);

	FHitResult ShotHit;
	FireDirRotator += FRotator(FMath::RandRange(-RecoilCounter, RecoilCounter), FMath::RandRange(-RecoilCounter, RecoilCounter), 0);
	FVector RayEnd = FirePoint + (FireDirRotator.Vector() * Range);

	FCollisionQueryParams ShotParams;
	ShotParams.AddIgnoredActor(this);
	ShotParams.AddIgnoredActor(GunOwner);

	DrawDebugLine(GetWorld(), FirePoint, RayEnd, FColor(255, 0, 0), true, 0, 0, 1);
	if (GetWorld()->LineTraceSingleByChannel(ShotHit, FirePoint, RayEnd, ECollisionChannel::ECC_Camera, ShotParams))
	{
		DrawDebugLine(GetWorld(), FirePoint, ShotHit.ImpactPoint, FColor(0, 255, 0), true, 0, 0, 10);
	}
	CurrentClipAmmo--;
}

void ARayGun::OnAttach(AActor* MyOwner)
{
	ABase_Weapon::OnAttach(MyOwner);
	AddActorLocalRotation(FRotator(0.0f, -90.0f, 0.0f));
}