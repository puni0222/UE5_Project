// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
	}

}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacter)
	{
		GroudSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);
		IsFalling = PlayerCharacterMovement->IsFalling();
		CharacterState = PlayerCharacter->GetCharacterState();
		ActionState = PlayerCharacter->GetActionState();
		DeathPose = PlayerCharacter->GetDeathPose();
	}
	
}
