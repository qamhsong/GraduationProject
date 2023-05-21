// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SourceEffects/SourceEffectStereoDelay.h"
#include "PS_Player.generated.h"

class UAudioCaptureComponent;
class USoundSubmix;
class USoundWaveProcedural;
class APS_PlayerController;
class USoundEffectSourcePresetChain;
class USourceEffectEQPreset;
class USourceEffectBitCrusherPreset;
class USourceEffectFilterPreset;
class USourceEffectPhaserPreset;


UENUM()
enum class EEffectPreset : uint8
{
	EFilterLow = 0,
	EFilterHigh,
	EEQ,
	EBitCrusher,
	EStereoDelay,
	EPhaser
};

UENUM()
enum class EEffectPhaserLFOType : uint8
{
	Sine = 0,
	Upsaw,
	DownSaw,
	Square,
	Triangle,
	Exponential,
	RandomSampleHold,
};


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

#pragma region SourceChain & Effects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USoundEffectSourcePresetChain*  SourceChain;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USourceEffectStereoDelayPreset* DelayPreset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USourceEffectEQPreset* EQPreset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USourceEffectBitCrusherPreset* BitCrusherPreset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USourceEffectFilterPreset* LowFilterPreset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USourceEffectFilterPreset* HighFilterPreset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USourceEffectPhaserPreset* PhaserPreset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundEffectSourcePreset*> EffectPresets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundEffectSourcePreset*> AppliedEffects;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEffectPreset PresetToApply;

#pragma endregion


#pragma region Effect Settings
	UFUNCTION()
	void _EQBandSettings(const float& _frequency, const float& _bandwidth, const float& _gaindb);

	UFUNCTION(BlueprintCallable)
	void LowPassFilterSettings(const float& _cutofffrequency, const float& _qfilter);

	UFUNCTION()
	void HighPassFilterSettings(const float& _cutofffrequency, const float& _qfilter);

	UFUNCTION()
	void BitcrusherSettings(const float& _samplerate, const float& _bitdepth);

	UFUNCTION()
	void PhaserSettings_float(const float& _wetlevel, const float& _frequency, const float& _feedback);

	// Low Frequency Oscillator 효과 종류
	UFUNCTION()
	void PhaserSettings_LFO(EEffectPhaserLFOType _lfotype);

#pragma endregion


	//UPROPERTY()
	//float VoiceCaptureVolume;

	//UPROPERTY()
	//bool PlayVoiceCaptureFlag;

	//UPROPERTY()
	//FTimerHandle VoiceCaptureTickTimer;

	//UPROPERTY()
	//FTimerHandle PlayVoiceCaptureTimer;

	//UPROPERTY()
	//USoundWaveProcedural* VoiceCaptureSoundWaveProcedural;

	//UPROPERTY()
	//TArray<uint8> VoiceCaptureBuffer;

	//TSharedPtr<class IVoiceCapture> VoiceCapture;

	//float VoiceCaptureTime = 0;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
	
	UPROPERTY()
	bool bIsPlaying = false;

	UPROPERTY()
	bool bIsRecording = false;

	UPROPERTY()
	bool bIsAudioCaptureActivated = false;


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

	UFUNCTION()
	void ActivateRealTimeVoiceCapture();

	UFUNCTION()
	void DeActivateRealTimeVoiceCapture();

	UFUNCTION(BlueprintCallable)
	void PlayAudio();

	UFUNCTION()
	bool _CreateAllPreset();

	UFUNCTION()
	bool _CreateSourceChain();

	UFUNCTION(BlueprintCallable)
	void RegisterSourceChainEffect(EEffectPreset effectPreset);

	UFUNCTION()
	void RemoveSourceChainEffect(EEffectPreset effectPreset);

	// convert Frequency value to pitch value.
	// Frequency & Pitch has an exponential relationship
	UFUNCTION(BlueprintCallable)
	float ConvertDesiredFrequencyToPitch(const float _Frequency);

	UFUNCTION()
	void SetPitchMultiplier(float value);

	UFUNCTION()
	void SetVolumeMultiplier(float value);


	UFUNCTION()
	bool CheckSoundAppliedToAudioComponent();

	UFUNCTION()
	bool GetAudioCaptureState();

	UFUNCTION()
	bool GetRecordingState();
};
