// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PS_Player.generated.h"

class UAudioCaptureComponent;
class USoundSubmix;

UCLASS()
class PROJECTSOUND_API APS_Player : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APS_Player();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioCaptureComponent* AudioCapture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComponent;

	FAudioDevice* AudioDevice;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AudioCaptureVolumeMultiplier = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OutputAudioName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OutputAudioPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PitchCoefficient = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SampleRate = 44100;

	TArray<float> FrequenciesToGet;

	TArray<FSoundWaveSpectralData> OutSoundWaveSpectralData;

	bool bIsPlaying;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// If pressed 1, Activate voice capture and begin recording
	UFUNCTION(BlueprintCallable)
	void ActivateVoiceCapture();

	// If pressed 2, Deactivate voice capture and finish recording
	// when finished, save audio file as SoundWave
	UFUNCTION(BlueprintCallable)
	void DeActivateVoiceCapture();

	UFUNCTION(BlueprintCallable)
	void PlayAudio();

};
