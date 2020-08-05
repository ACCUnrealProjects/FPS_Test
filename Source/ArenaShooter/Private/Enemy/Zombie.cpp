// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Enemy/Zombie.h"
#include "../Public/Component/HealthComponent.h"
#include "../Public/PickUps/PickUps.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_sight.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleSize(45.0f, 95.0f);

	MyMoveComp = FindComponentByClass<UCharacterMovementComponent>();

	MyHealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("MyHealthComponent"));
	MyHealthComp->bEditableWhenInherited = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	ZombieSkel = GetMesh();
	ZombieSkel->SetSkeletalMesh(MeshAsset.Object);
	ZombieSkel->SetOnlyOwnerSee(true);
	ZombieSkel->SetupAttachment(GetCapsuleComponent());
	ZombieSkel->bCastDynamicShadow = true;
	ZombieSkel->CastShadow = true;
	ZombieSkel->SetRelativeLocation(FVector(0.f, 0.0f, -96.0f));
	ZombieSkel->SetRelativeRotation(FRotator( 0.0f,-90.0f, 0.0f));
	ZombieSkel->bOnlyOwnerSee = false;

	bUseControllerRotationYaw = false;
	MyMoveComp->bUseControllerDesiredRotation = true;
	MyMoveComp->bOrientRotationToMovement = true;
	MyMoveComp->MaxWalkSpeed = 200.0f;

	SetUpMyStimulis();
}

void AZombie::SetUpMyStimulis()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("MyAIStim"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();

	MyHealthComp->SetMaxHealth(30);
	MyHealthComp->IHaveDied.AddUniqueDynamic(this, &AZombie::Death);
	MyHealthComp->IHaveBeenHit.AddUniqueDynamic(this, &AZombie::ImHit);
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZombie::MeleeAttack(AActor* target)
{	
	if (!CanPunch || !target) { return; }

	UGameplayStatics::ApplyDamage(target, PunchDamage, GetController(), this, UDamageType::StaticClass());
	CanPunch = false;
	FTimerHandle PunchAttackTimer;
	GetWorld()->GetTimerManager().SetTimer(PunchAttackTimer, this, &AZombie::PunchCooldownComplete, PunchCoolDown, false);

	if (!AttackAnimation) { return; }
	UAnimInstance* AnimInstance = ZombieSkel->GetAnimInstance();
	if (!AnimInstance) { return; }
	AnimInstance->Montage_Play(AttackAnimation);

}

void AZombie::SetDrop(TSubclassOf<class APickUps> MyDrop)
{
	Drop = MyDrop;
}

void AZombie::PunchCooldownComplete()
{
	CanPunch = true;
}

void AZombie::ImHit()
{
	//play hit sound
}

void AZombie::RagDoll()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;
}

void AZombie::Death()
{
	if (IsDying) { return; }
	IsDying = true;

	DetachFromControllerPendingDestroy();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	SetActorEnableCollision(true);

	MyHealthComp->SetComponentTickEnabled(false);

	MyMoveComp->StopMovementImmediately();
	MyMoveComp->DisableMovement();
	MyMoveComp->SetComponentTickEnabled(false);

	RagDoll();

	if (Drop)
	{
		GetWorld()->SpawnActor<APickUps>(Drop, GetActorLocation(), GetActorRotation());
	}

	FTimerHandle DeathTimer;
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &AZombie::DestroyMe, 2.0f, false);
}

void AZombie::DestroyMe()
{
	Destroy();
}