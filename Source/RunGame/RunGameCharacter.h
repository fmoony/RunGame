// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "RunGameType.h"
#include "Interfaces/Damagable.h"
#include "RunGameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UCurveFloat;
class URunGameTimerSubsystem;
class UHealthComponent;
class UAnimMontage;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDiedSignature, FGameplayTag, DamageType, ARunGameCharacter*, DeadCharacter);

UCLASS(abstract)
class ARunGameCharacter : public ACharacter, public IDamagable
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SlideAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Constructor */
	ARunGameCharacter();

	/** Broadcast when character dies. Listeners (Controller, GameMode, etc.) react accordingly */
	UPROPERTY(BlueprintAssignable, Category = "RunGame|Death")
	FOnCharacterDiedSignature OnCharacterDied;

	/** Stops movement, spawns death camera, plays death animation, then self-destructs */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Death")
	void Die(FGameplayTag DamageType, float DestroyDelay = 3.0f);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

	// Called when HealthComponent reaches 0 HP
	UFUNCTION()
	void OnHealthDepleted(FGameplayTag DamageType, AActor* DeathCauser);

	/** Map damage type to corresponding death montage */
	UPROPERTY(EditAnywhere, Category = "RunGame|Death")
	TMap<FGameplayTag, UAnimMontage*> DeathMontages;

	UFUNCTION()
	void OnDeathMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	// Hit reaction

	/** Called when HealthComponent takes damage without dying — plays hit reaction animation */
	UFUNCTION()
	void OnHitReaction(float Damage, FGameplayTag DamageType, AActor* DamageCauser);

	/** Map damage type to corresponding hit reaction montage */
	UPROPERTY(EditAnywhere, Category = "RunGame|Hit")
	TMap<FGameplayTag, UAnimMontage*> HitReactionMontages;

	/** Duration of the material dissolve effect in seconds */
	UPROPERTY(EditAnywhere, Category = "RunGame|Death")
	float DissolveDuration = 2.0f;

	/** Name of the scalar material parameter to animate during dissolve */
	UPROPERTY(EditAnywhere, Category = "RunGame|Death")
	FName DissolveParameterName = "DissolveAmount";

	/** Creates dynamic material instances and starts the dissolve timer */
	void StartDissolve();

	/** Called each tick during dissolve to update material parameter */
	void TickDissolve();

	FTimerHandle DissolveTimerHandle;
	float DissolveElapsed = 0.0f;
	TArray<UMaterialInstanceDynamic*> DissolveMaterials;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** Initiates slide movement -- crouches, removes ground friction, plays slide montage */
	void StartSlide();

	/** Ends slide movement -- uncrouches and restores default movement parameters */
	void EndSlide();

	/** Callback when slide montage finishes blending out -- triggers EndSlide cleanup */
	UFUNCTION()
	void OnSlideBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Movement")
	UAnimMontage* SlideMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RunGame|Movement")
	bool bIsSliding;

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Movement")
	float RootMotionScale = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Movement")
	float MontagePlayRate = 0.8f;

	/** Float curve that maps elapsed game time to max walk speed */
	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Movement")
	TObjectPtr<UCurveFloat> MaxSpeedCurve;

	float DefaultGroundFriction;

	float BaseMaxWalkSpeed;

	UPROPERTY()
	TObjectPtr<URunGameTimerSubsystem> TimerSubsystem;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool IsSliding() const { return bIsSliding; }

	/** Reacts to game state changes -- self-destructs when returning to MainMenu */
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

	bool bTurn;
	bool InTurnBox;

	FRotator DesireRotation;

	// ~begin IDamagable interface

	virtual void OnTakeDamage_Implementation(float Damage, FGameplayTag DamageType, AActor* DamageCauser) override;
	virtual void OnTakeHealing_Implementation(float HealAmount, AActor* Healer) override;
	virtual void OnDeath_Implementation(AActor* DeathCauser) override;
	virtual float GetCurrentHP_Implementation() const override;
	virtual float GetMaxHP_Implementation() const override;
	virtual bool IsDead_Implementation() const override;

	// ~end IDamagable interface

	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }
};
