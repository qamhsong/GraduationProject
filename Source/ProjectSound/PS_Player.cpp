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
#include "Sound/SoundEffectSource.h"
#include <../../Synthesis/Source/Synthesis/Classes/SourceEffects/SourceEffectStereoDelay.h>
#include <../../Synthesis/Source/Synthesis/Classes/SourceEffects/SourceEffectEQ.h>
#include <../../Synthesis/Source/Synthesis/Classes/SourceEffects/SourceEffectBitCrusher.h>
#include <../../Synthesis/Source/Synthesis/Classes/SourceEffects/SourceEffectFilter.h>
#include <../../Synthesis/Source/Synthesis/Classes/SourceEffects/SourceEffectPhaser.h>
#include <../../Synthesis/Source/Synthesis/Classes/SourceEffects/SourceEffectChorus.h>


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



// Called when the game starts or when spawned
void APS_Player::BeginPlay()
{
	Super::BeginPlay();

	_CreateSourceChain();
	_CreateAllPreset();

	// 시작 UI 호출
	GetUIManager()->Show<UUI_Start>();
	//GetUIManager()->ApplyInputMode(EUIInputMode::GameAndUI);

	// Player Controller 검증 및 Audio Device 정보 획득
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

	//FrequenciesToGet.Add(100.f);
	//FrequenciesToGet.Add(500.f);
	//FrequenciesToGet.Add(1000.f);
	//FrequenciesToGet.Add(3000.f);
	//FrequenciesToGet.Add(5000.f);


	for (int32 i = 1; i <= 12; i++)
	{
		FrequencyMultiplier.Add(FMath::Pow(2,((float)(i * 1.f)/ 12.f)));
	}

}

void APS_Player::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EffectPresets.Num() > 1)
	{
		for (auto presets : EffectPresets)
		{
			presets->ConditionalBeginDestroy();
			presets = nullptr;
		}
	}
	SourceChain->ConditionalBeginDestroy();
	AudioComponent->Sound = nullptr;
	AudioComponent->SourceEffectChain = nullptr;

}

// Called every frame
void APS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 비실시간 Fast Fourier Transform 데이터 획득 여부 확인 코드
	//if (AudioComponent->IsPlaying())
	//{
	//	if(FrequenciesToGet.Num() <= 0)
	//	{
	//		return;
	//	}
	//	if (AudioComponent->GetCookedFFTData(FrequenciesToGet, OutSoundWaveSpectralData))
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Debug %f"), OutSoundWaveSpectralData[0].FrequencyHz));
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NO COOKED FFTDATA")));
	//	}
	//}

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
	// 마이크 오디오 캡쳐 기능 활성화
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
	
	// 오디오 녹화 시작 기능
	UAudioSettings* AudioSettings = GetMutableDefault<UAudioSettings>();
	USoundSubmix* NewSubmix = Cast<USoundSubmix>(AudioSettings->MasterSubmix.TryLoad());
	UAudioMixerBlueprintLibrary::StartRecordingOutput(this, 5.f, NewSubmix);
	bIsRecording = true;
	
}

void APS_Player::DeActivateVoiceCapture()
{
	// 오디오 캡쳐 기능 비활성화
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
	bIsRecording = false;

	// Fast Fourier Transform setting
	soundtest->bEnableBakedFFTAnalysis = true;
	soundtest->FFTSize = ESoundWaveFFTSize::VeryLarge_2048;
	soundtest->FFTAnalysisFrameSize = 4096;

	//Adjust Pitch by converting Frequency data
	//soundtest->Pitch = ConvertDesiredFrequencyToPitch(FrequencyMultiplier[DesiredKeyNumber]);
	
	// 녹화하여 생성된 오디오 파일 검증 및 오디오 컴포넌트에 등록
	if (soundtest->IsValidLowLevelFast())
	{
		AudioComponent->SetSound(soundtest);
		UE_LOG(LogTemp, Warning, TEXT("Sound Set to AudioComponent"));
	}
}

void APS_Player::ActivateRealTimeVoiceCapture()
{
	AudioCapture->Activate();
	bIsAudioCaptureActivated = true;
}

void APS_Player::DeActivateRealTimeVoiceCapture()
{
	 AudioCapture->Deactivate();
	 bIsAudioCaptureActivated  = false;
}

void APS_Player::PlayAudio()
{
	if (CheckSoundAppliedToAudioComponent())
	{
		AudioComponent->Play();
	}
}

bool APS_Player::_CreateAllPreset()
{
	EQPreset = NewObject<USourceEffectEQPreset>(USourceEffectEQPreset::StaticClass());
	
	DelayPreset = NewObject<USourceEffectStereoDelayPreset>(USourceEffectStereoDelayPreset::StaticClass());

	BitCrusherPreset = NewObject<USourceEffectBitCrusherPreset>(USourceEffectBitCrusherPreset::StaticClass());
	
	LowFilterPreset = NewObject<USourceEffectFilterPreset>(USourceEffectFilterPreset::StaticClass());


	HighFilterPreset = NewObject<USourceEffectFilterPreset>(USourceEffectFilterPreset::StaticClass());

	PhaserPreset = NewObject<USourceEffectPhaserPreset>(USourceEffectPhaserPreset::StaticClass());

	ChorusPreset = NewObject<USourceEffectChorusPreset>(USourceEffectChorusPreset::StaticClass());

	//EffectPresets.Emplace(EQPreset);
	//EffectPresets.Emplace(DelayPreset);
	EffectPresets.Emplace(BitCrusherPreset);
	EffectPresets.Emplace(LowFilterPreset);
	EffectPresets.Emplace(HighFilterPreset);
	EffectPresets.Emplace(PhaserPreset);
	EffectPresets.Emplace(ChorusPreset);
		
	return true;
}

void APS_Player::_EQBandSettings(const float& _frequency, const float& _bandwidth, const float& _gaindb)
{
	
}

void APS_Player::LowPassFilterSettings(const float& _cutofffrequency, const float& _qfilter)
{
	LowFilterPreset->Settings.FilterCircuit = ESourceEffectFilterCircuit::StateVariable;
	LowFilterPreset->Settings.FilterType = ESourceEffectFilterType::LowPass;
	LowFilterPreset->Settings.CutoffFrequency = _cutofffrequency;
	LowFilterPreset->Settings.FilterQ = _qfilter;
}

void APS_Player::HighPassFilterSettings(const float& _cutofffrequency, const float& _qfilter)
{
	LowFilterPreset->Settings.FilterCircuit = ESourceEffectFilterCircuit::StateVariable;
	HighFilterPreset->Settings.FilterType = ESourceEffectFilterType::HighPass;
	HighFilterPreset->Settings.CutoffFrequency = _cutofffrequency;
	HighFilterPreset->Settings.FilterQ = _qfilter;
}

void APS_Player::BitcrusherSettings(const float& _samplerate, const float& _bitdepth)
{
	BitCrusherPreset->SetBits(_bitdepth);
	BitCrusherPreset->SetSampleRate(_samplerate);

	//BitCrusherPreset->SetSettings(bitcrushSettings);
}

void APS_Player::PhaserSettings_float(const float& _wetlevel, const float& _frequency, const float& _feedback)
{
	PhaserPreset->Settings.WetLevel = _wetlevel;
	PhaserPreset->Settings.Frequency = _frequency;
	PhaserPreset->Settings.Feedback = _feedback;
}

void APS_Player::PhaserSettings_LFO(EEffectPhaserLFOType _lfotype)
{
	switch (_lfotype)
	{
	 case EEffectPhaserLFOType::Sine:
		PhaserPreset->Settings.LFOType = EPhaserLFOType::Sine;
		break;
	case EEffectPhaserLFOType::Upsaw:
		PhaserPreset->Settings.LFOType = EPhaserLFOType::UpSaw;
		break;
	case EEffectPhaserLFOType::DownSaw:
		PhaserPreset->Settings.LFOType = EPhaserLFOType::DownSaw;
		break;
	case EEffectPhaserLFOType::Square:
		PhaserPreset->Settings.LFOType = EPhaserLFOType::Square;
		break;
	case EEffectPhaserLFOType::Triangle:
		PhaserPreset->Settings.LFOType = EPhaserLFOType::Triangle;
		break;
	case EEffectPhaserLFOType::Exponential:
		PhaserPreset->Settings.LFOType = EPhaserLFOType::Exponential;
		break;
	case EEffectPhaserLFOType::RandomSampleHold:
		PhaserPreset->Settings.LFOType = EPhaserLFOType::RandomSampleHold;
		break;
	default:
		PhaserPreset->Settings.LFOType = EPhaserLFOType::Sine;
		break;
	}
}

void APS_Player::ChorusSettings(const float& _depth, const float& _frequency, const float& _feedback, const float& _wetlevel, const float& _drylevel, const float& _spread)
{
	ChorusPreset->SetDepth(_depth);
	ChorusPreset->SetFrequency(_frequency);
	ChorusPreset->SetFeedback(_feedback);
	ChorusPreset->SetWet(_wetlevel);
	ChorusPreset->SetDry(_drylevel);
	ChorusPreset->SetSpread(_spread);

}

bool APS_Player::_CreateSourceChain()
{
	SourceChain = NewObject<USoundEffectSourcePresetChain>(USoundEffectSourcePresetChain::StaticClass());

	if (SourceChain == nullptr)
	{
		return false;   
	}
	return true;
}

void APS_Player::RegisterSourceChainEffect(EEffectPreset effectPreset)
{
	FSourceEffectChainEntry chainEntry;
	chainEntry.bBypass = false;
	switch (effectPreset)
	{
	case EEffectPreset::EFilterLow:
		chainEntry.Preset = LowFilterPreset;
		break;
	case EEffectPreset::EFilterHigh:
		chainEntry.Preset = HighFilterPreset;
		break; 
	case EEffectPreset::EEQ:
		chainEntry.Preset = EQPreset;
		break;
	case EEffectPreset::EBitCrusher:
		chainEntry.Preset = BitCrusherPreset;
		break;
	case EEffectPreset::EStereoDelay:
		chainEntry.Preset = DelayPreset;
		break;
	case EEffectPreset::EPhaser:
		chainEntry.Preset = PhaserPreset;
		break;
	case EEffectPreset::EChorus:
		chainEntry.Preset = ChorusPreset;
		break;
	default:
		chainEntry.Preset = nullptr;
		break;
	}
	
	if(chainEntry.Preset == nullptr)
		return;
	if(SourceChain == nullptr || AudioComponent == nullptr)
		return;


	SourceChain->Chain.Add(chainEntry);
	if (SourceChain->Chain.Num() > 0)
	{
		AudioComponent->SetSourceEffectChain(SourceChain);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Depth %f"), ChorusPreset->Settings.Depth));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Frequency %f"), ChorusPreset->Settings.Frequency));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Feedback %f"), ChorusPreset->Settings.Feedback));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("WetLevel %f"), ChorusPreset->Settings.WetLevel));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DryLevel %f"), ChorusPreset->Settings.DryLevel));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Spread %f"), ChorusPreset->Settings.Spread));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed To Add Chain Entry"));
	}
}

void APS_Player::RemoveSourceChainEffect(EEffectPreset effectPreset)
{
	if (SourceChain == nullptr || SourceChain->Chain.Num() < 1)
	{
		return;
	}
	USoundEffectSourcePreset* presetToRemove;
	switch (effectPreset)
	{
	case EEffectPreset::EFilterLow:
		presetToRemove = LowFilterPreset;
		break;
	case EEffectPreset::EFilterHigh:
		presetToRemove = HighFilterPreset;
		break;
	case EEffectPreset::EBitCrusher:
		presetToRemove = BitCrusherPreset;
		break;
	case EEffectPreset::EEQ:
		presetToRemove = EQPreset;
		break;
	case EEffectPreset::EStereoDelay:
		presetToRemove = DelayPreset;
		break;
	case EEffectPreset::EPhaser:
		presetToRemove = PhaserPreset;
		break;
	case EEffectPreset::EChorus:
		presetToRemove = ChorusPreset;
		break;
	default:
		presetToRemove = nullptr;
		break;
	}

	if (presetToRemove == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed To Remove Preset."));
		return;
	}

	for (int32  i= 0; i < SourceChain->Chain.Num(); i++)
	{
		if (SourceChain->Chain[i].Preset == presetToRemove)
		{
			if (SourceChain->Chain.IsValidIndex(i))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Remove Index : %d"), i));
				SourceChain->Chain.RemoveAt(i);
				UE_LOG(LogTemp, Warning, TEXT("REMOVED"));
			}
		}
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

bool APS_Player::CheckSoundAppliedToAudioComponent()
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

bool APS_Player::GetRecordingState()
{
	return bIsRecording;
}
