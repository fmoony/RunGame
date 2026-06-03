// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "RunGameType.h"
#include "Interfaces/Damagable.h"
#include "Character/RunGameInputBufferComponent.h"
#include "RunGameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class URunGameTimerSubsystem;
class UHealthComponent;
class USkillComponent;
class URunGameMovementComponent;
class URunGameEffectComponent;
class URunGameCollisionAbilityComponent;
class UNiagaraSystem;
class UNiagaraComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URunGameInputBufferComponent> InputBuffer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URunGameEffectComponent> EffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URunGameCollisionAbilityComponent> CollisionAbility;

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

	/** Constructor — 使用 FObjectInitializer 强制替换默认 CMC */
	ARunGameCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Broadcast when character dies */
	UPROPERTY(BlueprintAssignable, Category = "RunGame|Death")
	FOnCharacterDiedSignature OnCharacterDied;

	/** 生成死亡摄像机 → 设置 Dead 状态 → 广播。动画组件和溶解链独立响应 Spawn death camera → set Dead → broadcast. Animation/dissolve react independently */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Death")
	void Die(FGameplayTag DamageType, float DestroyDelay = 3.0f, AActor* DeathCauser = nullptr);

	UPROPERTY(BlueprintAssignable, Category = "RunGame|State")
	FOnCharacterStateChangedSignature OnCharacterStateChanged;

	UFUNCTION(BlueprintCallable, Category = "RunGame|State")
	void SetCharacterState(ERunGameCharacterState NewState);

	UFUNCTION(BlueprintPure, Category = "RunGame|State")
	ERunGameCharacterState GetCharacterState() const;

	UFUNCTION(BlueprintPure, Category = "RunGame|State")
	bool IsCharacterStateTransitionAllowed(ERunGameCharacterState NewState) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION()
	void OnHealthDepleted(FGameplayTag DamageType, AActor* DeathCauser);

	/** 桥接 HealthComponent 伤害 → RuntimeState Bridge damage → RuntimeState */
	UFUNCTION()
	void OnHealthDamageTaken(float Damage, FGameplayTag DamageType, AActor* DamageCauser);

protected:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** 空中跳跃是否可用——进入 CoyoteTime 或跳离地面时设 true，消耗后设 false，落地重置 */
	bool bAirJumpAvailable = false;

	/** 起跳前状态——OnJumped 用它区分「地面跳」和「土狼时间跳」 Pre-jump state — OnJumped uses it to distinguish ground jump vs coyote-time jump */
	ERunGameCharacterState PreJumpState = ERunGameCharacterState::Idle;

	void StartSlide();

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure, Category = "RunGame|State")
	bool IsSliding() const;

	// ~begin IDamagable interface
	virtual void OnTakeDamage_Implementation(float Damage, FGameplayTag DamageType, AActor* DamageCauser) override;
	virtual void OnTakeHealing_Implementation(float HealAmount, AActor* Healer) override;
	virtual void OnDeath_Implementation(AActor* DeathCauser) override;
	virtual float GetCurrentHP_Implementation() const override;
	virtual float GetMaxHP_Implementation() const override;
	virtual bool IsDead_Implementation() const override;
	// ~end IDamagable interface

	/** 覆写：CoyoteTime 或 Airborne + bAirJumpAvailable 时允许跳跃 Override: allow jump during CoyoteTime or Airborne with double jump available */
	virtual bool CanJumpInternal_Implementation() const override;

	/** 跳跃物理生效后回调 —— 在此统筹二段跳扣除和 CoyoteTime→Airborne 状态切换
	 *  Called after jump physics is applied — handles air jump consumption and CoyoteTime→Airborne transition */
	virtual void OnJumped_Implementation() override;

	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	FORCEINLINE USkillComponent* GetSkillComponent() const { return SkillComponent; }

	/** 获取自定义运动组件 Get custom movement component */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	URunGameMovementComponent* GetRunGameMovementComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Skills")
	void ActivateSkillByTag(FGameplayTag SkillTag);

	/** 输入缓冲消费回调 Input buffer consumption callback */
	void OnBufferedInputReady(ERunGameInputCommand Command);

private:
	UFUNCTION()
	void OnCharacterStateChangedCallback(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	UFUNCTION()
	void OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState);

	UFUNCTION()
	void OnRSCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);
};
