// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Enemy/Turret.h"
#include "../Public/Component/HealthComponent.h"
#include "../Public/Component/TurretAimingComponent.h"
#include "../Public/MeshComponets/BarrelMesh.h"
#include "../Public/MeshComponets/TurretMesh.h"
#include "../Public/Projectile/TracerRound.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyHealthComp= CreateDefaultSubobject<UHealthComponent>(TEXT("MyHealthComponent"));
	MyHealthComp->bEditableWhenInherited = true;

	MyAimingComp = CreateDefaultSubobject<UTurretAimingComponent>(TEXT("MyAimingComponent"));
	MyAimingComp->bEditableWhenInherited = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyBody"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyAsset(TEXT("StaticMesh'/Game/MyStuff/Meshes/Turret/mortar_body.mortar_body'"));
	Body->SetStaticMesh(BodyAsset.Object);
	SetRootComponent(Body);

	MyTurret = CreateDefaultSubobject<UTurretMesh>(TEXT("MyTurret"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretAsset(TEXT("StaticMesh'/Game/MyStuff/Meshes/Turret/mortar_dome.mortar_dome'"));
	MyTurret->SetStaticMesh(TurretAsset.Object);
	MyTurret->AttachTo(Body, "Turret", EAttachLocation::SnapToTarget);
	MyTurret->bEditableWhenInherited = true;

	MyBarrel = CreateDefaultSubobject<UBarrelMesh>(TEXT("MyBarrel"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelAsset(TEXT("StaticMesh'/Game/MyStuff/Meshes/Turret/mortar_barrel.mortar_barrel'"));
	MyBarrel->SetStaticMesh(BarrelAsset.Object);
	MyBarrel->AttachTo(MyTurret, "Barrel", EAttachLocation::SnapToTarget);
	MyBarrel->bEditableWhenInherited = true;

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(FName("Fire Effect"));
	//FireEffect->AttachToComponent(Muzzle, FAttachmentTransformRules::KeepRelativeTransform);
	//FireEffect->bAutoActivate = false;

	HitEffect = CreateDefaultSubobject<UParticleSystem>(FName("Hit Effect"));
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	MyAimingComp->SetUp(MyTurret, MyBarrel);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurret::Fire(AActor* Target)
{
	if (!MyAimingComp->CloseToHittingTarget(Target->GetActorLocation()) || !CanFire) { return; }

	FRotator FireDirRotator = MyBarrel->GetSocketRotation(FName("FireLocation"));
	FVector FirePoint = MyBarrel->GetSocketLocation(FName("FireLocation"));

	FHitResult ShotHit;
	FireDirRotator += FRotator(FMath::RandRange(-RecoilMinMax, RecoilMinMax), FMath::RandRange(-RecoilMinMax, RecoilMinMax), 0);
	FVector RayEnd = FirePoint + (FireDirRotator.Vector() * Range);

	FCollisionQueryParams ShotParams;
	ShotParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(ShotHit, FirePoint, RayEnd, ECollisionChannel::ECC_Camera, ShotParams))
	{
		UGameplayStatics::ApplyDamage(ShotHit.GetActor(), Damage, GetController(), this, UDamageType::StaticClass());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, ShotHit.ImpactPoint, ShotHit.ImpactNormal.Rotation());
	}

	if (ProjectileBlueprint)
	{
		GetWorld()->SpawnActor<ATracerRound>(ProjectileBlueprint, MyBarrel->GetSocketLocation(FName("FireLocation")), MyBarrel->GetSocketRotation(FName("FireLocation")));
	}
	

}

void ATurret::AimAt(AActor* Target)
{
	MyAimingComp->AimToTarget(Target->GetActorLocation());
}

void ATurret::SetCanFireTrue()
{
	CanFire = true;
}