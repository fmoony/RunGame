// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "Damagable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDamagable : public UInterface
{
	GENERATED_BODY()
};

class RUNGAME_API IDamagable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damageable")
	void OnTakeDamage(float Damage, FGameplayTag DamageType, AActor* DamageCauser);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damageable")
	void OnTakeHealing(float HealAmount, AActor* Healer);
	 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damageable")
	void OnDeath(AActor* DeathCauser);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damageable")
	float GetCurrentHP() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damageable")
	float GetMaxHP() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damageable")
	bool IsDead() const;
};
