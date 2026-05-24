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
class USkillComponent;
class UAnimMontage;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDiedSignature, FGameplayTag, DamageType, ARunGameCharacter*, DeadCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterStateChangedSignature, ERunGameCharacterState, OldState, ERunGameCharacterState, NewState);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkillComponent> SkillComponent;

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

	/** Stops movement, spawns death camera, plays death animation, dissolves, then self-destructs */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Death")
	void Die(FGameplayTag DamageType, float DestroyDelay = 3.0f, AActor* DeathCauser = nullptr);

	/** Broadcast when character state changes. Follows same pattern as GameState::OnGameStateChanged */
	UPROPERTY(BlueprintAssignable, Category = "RunGame|State")
	FOnCharacterStateChangedSignature OnCharacterStateChanged;

	/** Set the character's core state. Validates transition, guards same-state, saves old, modifies, broadcasts. Pure — no side effects */
	UFUNCTION(BlueprintCallable, Category = "RunGame|State")
	void SetCharacterState(ERunGameCharacterState NewState);

	/** Returns the current character state (from PlayerRuntimeState) */
	UFUNCTION(BlueprintPure, Category = "RunGame|State")
	ERunGameCharacterState GetCharacterState() const;

	/** Returns true if the transition from current state to NewState is allowed */
	UFUNCTION(BlueprintPure, Category = "RunGame|State")
	bool IsCharacterStateTransitionAllowed(ERunGameCharacterState NewState) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

	/** Called when character lands on ground. Handles Airborne→Idle transition and input buffer consumption */
	virtual void Landed(const FHitResult& Hit) override;

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

	/** Initiates slide movement -- requests Sliding state. If rejected (e.g. Airborne), input is buffered */
	void StartSlide();

	/** Callback when slide montage finishes blending out -- requests Idle state transition */
	UFUNCTION()
	void OnSlideBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Movement")
	UAnimMontage* SlideMontage;


	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Movement")
	float RootMotionScale = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Movement")
	float MontagePlayRate = 0.8f;

	/** Float curve that maps elapsed game time to max walk speed */
	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Movement")
	TObjectPtr<UCurveFloat> MaxSpeedCurve;

	/** Active speed modifiers from skills, props, etc. Tag → Multiplier */
	TMap<FGameplayTag, float> SpeedModifiers;

	/** Cached product — maintained via multiply/divide on Add/Remove. 1.0 = no modifiers active */
	float CachedCompositeSpeedMultiplier = 1.0f;

	/** Current smoothed speed — interpolated toward target in Tick. Initialized from MaxWalkSpeed in BeginPlay */
	float SmoothedMaxWalkSpeed = 1200.0f;

	/** Duration in seconds to reach target speed via smooth interpolation */
	UPROPERTY(EditAnywhere, Category = "RunGame|Movement")
	float SpeedTransitionDuration = 0.5f;

	float DefaultGroundFriction;

	float BaseMaxWalkSpeed;

	UPROPERTY()
	TObjectPtr<URunGameTimerSubsystem> TimerSubsystem;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure, Category = "RunGame|State")
	bool IsSliding() const;

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

	FORCEINLINE USkillComponent* GetSkillComponent() const { return SkillComponent; }

	/** Add or update a speed modifier. Overwrites if tag already exists. Multiplier is the factor applied */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Movement")
	void AddSpeedModifier(FGameplayTag ModifierTag, float Multiplier);

	/** Remove a speed modifier by tag. No-op if not found */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Movement")
	void RemoveSpeedModifier(FGameplayTag ModifierTag);

	float GetCompositeSpeedMultiplier() const;

	/** Activate a skill by tag — called from dynamically-bound Enhanced Input */
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void ActivateSkillByTag(FGameplayTag SkillTag);

	/** 获取转弯/转角盒状态（调试用） Get turn/box flags (for debug) */
	FORCEINLINE bool IsInTurn() const { return bTurn; }
	FORCEINLINE bool IsInTurnBox() const { return bInTurnBox; }

private:
	/** Buffered input — when a state transition is rejected, store it here. MAX = no buffered input */
	ERunGameCharacterState PendingInputState = ERunGameCharacterState::MAX;

	/** Reacts to own state changes — slide setup/cleanup, turn flag management, etc. */
	UFUNCTION()
	void OnCharacterStateChangedCallback(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	// ---- GameState 响应 ----

	UFUNCTION()
	void OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState);

	/** 响应 RS 状态变化——广播自身委托让蓝图可监听 Forward RS state to own delegate for BP listeners */
	UFUNCTION()
	void OnRSCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	bool bTurn = false;
	bool bInTurnBox = false;

};
