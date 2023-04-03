// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Player.h"
#include "PS_GameInstance.h"
#include "AudioCaptureComponent.h"
#include "Sound/SoundSubmix.h"
#include "AudioMixerBlueprintLibrary.h"

// Sets default values
APS_Player::APS_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	AudioCapture = CreateAbstractDefaultSubobject<UAudioCaptureComponent>(TEXT("AudioCapture"));
	AudioCapture->SetupAttachment(RootComponent);
	AudioCapture->SetVolumeMultiplier(AudioCaptureVolumeMultiplier);
}

// Called when the game starts or when spawned
void APS_Player::BeginPlay()
{
	Super::BeginPlay();

	AudioDevice = FAudioDeviceManager::Get()->GetActiveAudioDevice().GetAudioDevice();

	if(AudioDevice == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Cannot Detect Audio Device."));
}

// Called every frame
void APS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ActivateVC", IE_Pressed, this, &APS_Player::ActivateVoiceCapture);
	PlayerInputComponent->BindAction("DeActivateVC", IE_Pressed, this, &APS_Player::DeActivateVoiceCapture);

}

void APS_Player::ActivateVoiceCapture()
{
	AudioCapture->Activate();
	if (!AudioDevice)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Detect Audio Device."));
		return;
	}
		
	UAudioSettings* AudioSettings = GetMutableDefault<UAudioSettings>();
	USoundSubmix* NewSubmix = Cast<USoundSubmix>(AudioSettings->MasterSubmix.TryLoad());
	UAudioMixerBlueprintLibrary::StartRecordingOutput(this, 5.f, NewSubmix);
	
}

void APS_Player::DeActivateVoiceCapture()
{
	AudioCapture->Deactivate(); 

	if (!AudioDevice)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Detect Audio Device."));
		return;
	}
		

	UAudioSettings* AudioSettings = GetMutableDefault<UAudioSettings>();
	USoundSubmix* NewSubmix = Cast<USoundSubmix>(AudioSettings->MasterSubmix.TryLoad());
	USoundWave* ExistingSoundWaveToOverWrite = nullptr;
	UAudioMixerBlueprintLibrary::StopRecordingOutput(this, EAudioRecordingExportType::SoundWave, FString("soundtest"), FString(""), NewSubmix, ExistingSoundWaveToOverWrite);
}

