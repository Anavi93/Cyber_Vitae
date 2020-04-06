// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "CVCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ACVWeapon;
class UCVHealthComponent;
class ACVInteractiveActor;
class UCVInventoryComponent;
class ACVBaseEffect;

/*
UENUM(BlueprintType)
enum class EWeaponTypeEnum :uint8 {

	WE_Riffle			UMETA(DisplayName="Riffle"),
	WE_Sniper			UMETA(DisplayName="Sniper"),
	WE_GrenadeLauncher	UMETA(DisplayName="GrenadeLauncher"),
	WE_Pistol			UMETA(DisplayName="Pistol")
};
*/

USTRUCT(BlueprintType)
struct FInventoryItem: public FTableRowBase
{
	GENERATED_BODY()

public:

	FInventoryItem()
	{
		Name = FText::FromString("No name");
		Action = FText::FromString("No action");
		Description = FText::FromString("Please enter description for this item.");
		
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ACVInteractiveActor> ItemPickUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ACVBaseEffect> ItemEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeUsed;

	bool operator==(const FInventoryItem Item) const
	{
		if(ItemID==Item.ItemID)
		{
			return true;
		}

		return false;
	}
};

UCLASS()
class CYBER_VITAE_API ACVCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACVCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void BeginCrouch();
	void EndCrouch();

	void BeginZoom();
	void EndZoom();

	void NextWeapon();
	void PreviousWeapon();

	void Interact();

	void Reload();

	void StartFire();
	void StopFire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCVHealthComponent* HealthComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* ZoomedSpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* ZoomedCameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	ACVWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCVInventoryComponent* InventoryComp;

	void SpawnWeapon();
	void SpawnWeapons();

	int32 CurrentWeaponPlace;

	int32 WeaponStackSize;
	
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TArray<TSubclassOf<ACVWeapon>> EquippedWeaponClasses;

	TArray<ACVWeapon*> EquippedWeapons;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	UFUNCTION()
		void OnHealthChanged(UCVHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
			class AController* InstigatedBy, AActor* DamageCauser);

	//Pawn died previously
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool bDied;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bWantsToZoom;

	UPROPERTY(VisibleAnywhere, Category = "Player")
		float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
		float ZoomInterpSpeed;

	void CheckForInteractables();

	void SetZoom();

	UPROPERTY(BlueprintReadOnly, Category = "Interactive")
	ACVInteractiveActor* CurrentInteractive;

	UPROPERTY(BlueprintReadWrite, Category = "Effect")
		TSubclassOf<class ACVBaseEffect> CurrentEffectClass;

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
		ACVBaseEffect* CurrentEffect;

	UFUNCTION(BlueprintCallable)
	void UseEffect();

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Called to fill weapon magazine
	void FindAndReload(TSubclassOf<ACVWeapon> WeaponType);

	void DestroyEffect();

};
