// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SourceEffects/SourceEffectStereoDelay.h"
#include <../../Synthesis/Source/Synthesis/Classes/SourceEffects/SourceEffectStereoDelay.h>
#include "PS_Player.generated.h"

class UAudioCaptureComponent;
class USoundSubmix;
class USoundWaveProcedural;
class APS_PlayerController;
class USoundEffectSourcePresetChain;

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

	UPROPERTY()
	APS_PlayerController* pc;

	FAudioDevice* AudioDevice;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USoundEffectSourcePresetChain*  SourceChain;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USourceEffectStereoDelayPreset* DelayPreset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSourceEffectStereoDelaySettings StereoDelaySettings;


	UPROPERTY()
	float VoiceCaptureVolume;
	UPROPERTY()
	bool PlayVoiceCaptureFlag;
	UPROPERTY()
	FTimerHandle VoiceCaptureTickTimer;
	UPROPERTY()
	FTimerHandle PlayVoiceCaptureTimer;
	UPROPERTY()
	USoundWaveProcedural* VoiceCaptureSoundWaveProcedural;
	UPROPERTY()
	TArray<uint8> VoiceCaptureBuffer;

	TSharedPtr<class IVoiceCapture> VoiceCapture;

	float VoiceCaptureTime = 0;

	UFUNCTION(BlueprintCallable)
void VoiceCaptureTick();
	UFUNCTION(BlueprintCallable)
	void PlayVoiceCapture();



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
	float PitchCoefficient = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SampleRate = 48000;

	// Frequency array for FFT to analyze
	TArray<float> FrequenciesToGet;
	
	// Container for storing post FFT data
	TArray<FSoundWaveSpectralData> OutSoundWaveSpectralData;

	// preset frequency product value of piano key
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<float> FrequencyMultiplier;

	//key value to select
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DesiredKeyNumber = 5;
	
	bool bIsPlaying;

	UPROPERTY()
	bool bIsAudioCaptureActivated;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StereoDelayTime = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStereoDelaySourceEffect DelayMode = EStereoDelaySourceEffect::PingPong;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayRatio = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFilter = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStereoDelayFiltertype FilterType = EStereoDelayFiltertype::Lowpass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CutoffFilterFrequency = 1000.f;

	// The higher the Q the more resonant the filter, and the narrower the range of frequencies that are allowed to pass.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FilterQualityFactor = 3.0f;



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

	// convert Frequency value to pitch value.
	// Frequency & Pitch has an exponential relationship
	UFUNCTION(BlueprintCallable)
	float ConvertDesiredFrequencyToPitch(const float _Frequency);

	UFUNCTION()
	void SetPitchMultiplier(float value);

	UFUNCTION()
	void SetVolumeMultiplier(float value);

	UFUNCTION()
	bool _CheckSoundAppliedToAudioComponent();

	UFUNCTION()
	bool GetAudioCaptureState();
};
