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
class URunGameTimerSubsystem;
class UHealthComponent;
class USkillComponent;
class URunGameInputComponent;
class URunGameInputContextComponent;
class URunGameControlPipelineComponent;
class URunGameMovementComponent;
class URunGameLocomotionComponent;
class URunGameEffectComponent;
class URunGameCameraComponent;
class URunGameCollisionAbilityComponent;
class UNiagaraSystem;
class UNiagaraComponent;

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
	TObjectPtr<URunGameInputComponent> RunGameInputComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URunGameInputContextComponent> InputContextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URunGameControlPipelineComponent> ControlPipelineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URunGameLocomotionComponent> LocomotionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URunGameEffectComponent> EffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URunGameCollisionAbilityComponent> CollisionAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URunGameCameraComponent> CameraComponent;

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

	/** RuntimeState 转发门面，状态校验由 RuntimeState 负责 / RuntimeState forwarding facade; RuntimeState owns validation */
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

	/** 覆写：桥接到 MovementComponent 的跳跃规则 / Override: bridge to MovementComponent jump rules */
	virtual bool CanJumpInternal_Implementation() const override;

	/** CMC 确认起跳后转发给 Locomotion / Forward confirmed CMC launch to Locomotion */
	virtual void OnJumped_Implementation() override;

	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	FORCEINLINE USkillComponent* GetSkillComponent() const { return SkillComponent; }

	/** 获取自定义运动组件 Get custom movement component */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	URunGameMovementComponent* GetRunGameMovementComponent() const;

	/** 获取跑酷运动规则组件 Get runner locomotion rules component */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	URunGameLocomotionComponent* GetRunGameLocomotionComponent() const;

private:
	UFUNCTION()
	void OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState);

	UFUNCTION()
	void OnRSCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

};
