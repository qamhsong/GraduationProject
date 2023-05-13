// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Player.h"
#include "PS_GameInstance.h"
#include "AudioCaptureComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundSubmix.h"
#include "AudioMixerBlueprintLibrary.h"
#include "UIManager.h"
#include "UI_Start.h"
#include "PS_PlayerController.h"
//#include "AudioAnalyzerManager.h"
#include "Sound/SoundEffectSource.h"
#include <../../Synthesis/Source/Synthesis/Classes/SourceEffects/SourceEffectStereoDelay.h>


// Sets default values
APS_Player::APS_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	AudioCapture = CreateDefaultSubobject<UAudioCaptureComponent>(TEXT("AudioCapture"));
	AudioCapture->SetupAttachment(RootComponent);
	AudioCapture->SetVolumeMultiplier(AudioCaptureVolumeMultiplier);
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = true;
	AudioComponent->bAlwaysPlay = true;


}

void APS_Player::VoiceCaptureTick()
{

}

void APS_Player::PlayVoiceCapture()
{

}

// Called when the game starts or when spawned
void APS_Player::BeginPlay()
{
	Super::BeginPlay();


	


	GetUIManager()->Show<UUI_Start>();
	//GetUIManager()->ApplyInputMode(EUIInputMode::GameAndUI);

	UPS_GameInstance* gInst = GInst();
	if (gInst == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO GAME INSTANCE"));
	}
	UWorld* world = gInst->GetWorld();
	if (world == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No WORLD"));
	}
	pc = world->GetFirstPlayerController<APS_PlayerController>();

	AudioDevice = FAudioDeviceManager::Get()->GetActiveAudioDevice().GetAudioDevice();

	if(AudioDevice == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Cannot Detect Audio Device."));

	FrequenciesToGet.Add(100.f);
	FrequenciesToGet.Add(500.f);
	FrequenciesToGet.Add(1000.f);
	//FrequenciesToGet.Add(3000.f);
	FrequenciesToGet.Add(5000.f);


	for (int32 i = 1; i <= 12; i++)
	{
		FrequencyMultiplier.Add(FMath::Pow(2,((float)(i * 1.f)/ 12.f)));
	}

}

// Called every frame
void APS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AudioComponent->IsPlaying())
	{
		if(FrequenciesToGet.Num() <= 0)
		{
			return;
		}
		if (AudioComponent->GetCookedFFTData(FrequenciesToGet, OutSoundWaveSpectralData))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Debug %f"), OutSoundWaveSpectralData[0].FrequencyHz));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NO COOKED FFTDATA")));
		}
	}

}

// Called to bind functionality to input
void APS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ActivateVC", IE_Pressed, this, &APS_Player::ActivateVoiceCapture);
	PlayerInputComponent->BindAction("DeActivateVC", IE_Pressed, this, &APS_Player::DeActivateVoiceCapture);
	PlayerInputComponent->BindAction("PlayAudio", IE_Pressed, this, &APS_Player::PlayAudio);

}

void APS_Player::ActivateVoiceCapture()
{
	AudioCapture->Activate();
	
	if (!AudioDevice)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Detect Audio Device."));
		return;
	}
	if (bIsAudioCaptureActivated == false)
	{
		bIsAudioCaptureActivated = true;
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
	if (bIsAudioCaptureActivated == true)
	{
		bIsAudioCaptureActivated = false ;
	}
	// Create Submix & Recording Settings
	UAudioSettings* AudioSettings = GetMutableDefault<UAudioSettings>();
	USoundSubmix* NewSubmix = Cast<USoundSubmix>(AudioSettings->MasterSubmix.TryLoad());
	USoundWave* ExistingSoundWaveToOverWrite = nullptr;
	USoundWave* soundtest = UAudioMixerBlueprintLibrary::StopRecordingOutput(this, EAudioRecordingExportType::SoundWave, FString("soundtest"), FString(""), NewSubmix, ExistingSoundWaveToOverWrite);

	//Fast Fourier Transform setting
	soundtest->bEnableBakedFFTAnalysis = true;
	soundtest->FFTSize = ESoundWaveFFTSize::VeryLarge_2048;
	soundtest->FFTAnalysisFrameSize = 4096;

	//Adjust Pitch by converting Frequency data
	//soundtest->Pitch = ConvertDesiredFrequencyToPitch(FrequencyMultiplier[DesiredKeyNumber]);
	
	if (soundtest->IsValidLowLevelFast())
	{
		AudioComponent->SetSound(soundtest);
		UE_LOG(LogTemp, Warning, TEXT("Sound Set to AudioComponent"));
	}

	SourceChain = NewObject<USoundEffectSourcePresetChain>(USoundEffectSourcePresetChain::StaticClass());

	StereoDelaySettings.DelayMode = DelayMode;
	StereoDelaySettings.DelayTimeMsec = StereoDelayTime;
	StereoDelaySettings.bFilterEnabled = bEnableFilter;
	StereoDelaySettings.FilterType = FilterType;
	StereoDelaySettings.FilterFrequency = CutoffFilterFrequency;
	StereoDelaySettings.FilterQ = FilterQualityFactor;

	DelayPreset = NewObject<USourceEffectStereoDelayPreset>(USourceEffectStereoDelayPreset::StaticClass());


	DelayPreset->SetSettings(StereoDelaySettings);
	FSourceEffectChainEntry ChainEntry;
	ChainEntry.bBypass = true;
	ChainEntry.Preset = DelayPreset;
	

	SourceChain->Chain.Emplace(ChainEntry);
	if(SourceChain->Chain.Num() > 0)
		AudioComponent->SetSourceEffectChain(SourceChain);
	else
		UE_LOG(LogTemp,Warning, TEXT("Failed To Add Chain Entry"));

//	PlayAudio();

}

void APS_Player::PlayAudio()
{
	if (_CheckSoundAppliedToAudioComponent())
	{
		AudioComponent->Play();
	}
}

float APS_Player::ConvertDesiredFrequencyToPitch(const float _Frequency)
{
	return FMath::Log2(_Frequency);
}

void APS_Player::SetPitchMultiplier(float value)
{
	AudioComponent->SetPitchMultiplier(value);
}

void APS_Player::SetVolumeMultiplier(float value)
{
	AudioComponent->SetVolumeMultiplier(value);
}

bool APS_Player::_CheckSoundAppliedToAudioComponent()
{
	if (AudioComponent == nullptr)
	{
		 return false;
	}
	if (AudioComponent->Sound == nullptr)
	{
		return false;
	}
	return true;
}


bool APS_Player::GetAudioCaptureState()
{
	return bIsAudioCaptureActivated;
}
