## CSE40500 Graduation Project (2023 Spring)

# 실제 소리를 증강하는 사운드 시스템 구현

## 프로젝트 개요

사운드 이펙트는 영화, TV 프로그램, 비디오 게임, 애니메이션 등에서 사용되는 소리
효과이다. 사운드 이펙트는 영화의 분위기를 조성하고, 캐릭터와 스토리를 이해하는데
도움을 준다. 또한, 사운드는 사실감 있는 몰입형 환경을 만드는데 있어서 중요한 역할을
차지한다. 게임을 예로 들면, 레벨의 배경 사운드부터 캐릭터의 음성 대화, VFX(Visual 
Effect)에 알맞는 사운드 이펙트 등 단조로운 시각적 효과를 풍성하게 해주는 효과를
갖는다. 하지만, 사운드 이펙트를 생성하여 적용하기 위해서는 대부분 외부 미들웨어(MiddleWare)를
사용하여야하며 사운드 비전문가가 접근하기 어려운 영역이다. 외부 DAW(Digital Audio 
Workstation)와 같은 미들웨어(Middleware)에서 사운드를 제작하여 사운드 파일을
불러오는 경우가 대다수이며 이에 대한 전문 지식 및 사용법에 대해 알고 있지 않는
이상 사운드를 제작 및 변조하기 어렵다. 따라서, 본 문서는 언리얼
엔진(Unreal Engine 4)를 활용하여 소리를 직접 실시간, 비실시간 변조가
가능한 사운드 제작 시스템을 구현하고자 한다. 

## 프로젝트 목표

최근 AI 기술 및 게임 엔진의 발달로 프로그래머가 아니더라도 비주얼 스크립팅(Visual Scripting)등으로 게임을 제작할 수 있는 시대가 도래하고 있다.
게임 업계에서 보편적으로 사용되는 WWise와 같은 외부 사운드 미들웨어(MiddleWare)는 인디 게임 개발자 또는 개발 입문자들이 사용하기에는 비용적으로 부담되고 진입 장벽이 높다. 따라서 본 프로젝트는 UI 툴을 통해 원하는 사운드 이펙트를 추가하여 게임 개발에 필요한 사운드를 제작할 수 있는 저작 도구를 제공하고자 한다.

## 설치 및 사용 방법

### 시스템 요구 사항 및 세팅

1. Unreal Engine 4.27.2 다운로드  https://www.unrealengine.com/en-US/download
2. 프로젝트 압축 파일 다운로드 및 압축 해제
3. ProjectSound.uproject 파일 우 클릭 후 Generate Visual Studio project files 클릭
![image](https://github.com/qamhsong/GraduationProject/assets/114596581/829ceaf3-fae8-4eae-ba83-6f70e4cc751e)

4. Epic Games Launcher-> 라이브러리-> 내 프로젝트에서 실행 또는 다운로드 파일의 ProjectSound.uproject 더블 클릭으로 프로젝트 실행
5. 사운드 입 출력 장비 준비(마이크 & 스피커)
6. UE4 Editor 실행 이후 월드 세팅-> GameMode -> 게임 모드 오버라이드에서 BP_TestGameMode 체크
![2023-06-11 20 36 59](https://github.com/qamhsong/GraduationProject/assets/114596581/d2a5d912-a772-420b-a2c0-58c15e85643a)

### 프로젝트 시나리오

1. 프로젝트 실행 이후 UE4 Editor 상단 툴바에서 플레이 버튼 클릭
![image](https://github.com/qamhsong/GraduationProject/assets/114596581/6eb102ef-06b7-4294-b849-0076ca16b17f)
2. 언리얼 뷰포트에 게임 화면 확인 및 START 버튼 클릭
![image](https://github.com/qamhsong/GraduationProject/assets/114596581/5035e69d-3f1d-4a34-9a24-7944922e2127)
3. F11 클릭시 전체 화면으로 전환 
4. Start 버튼 클릭 이후 UI_Audio_Settings 화면 확인
![image](https://github.com/qamhsong/GraduationProject/assets/114596581/b6a5e57a-c01a-4ce3-ba75-be8da464e832)

5. Audio Capture 영역에 있는 OFF 버튼을 클릭하여 원본 사운드 녹음을 진행한다.
6. 녹음이 완료되고 활성화되는 PLAY 버튼을 통해 원본 사운드 확인이 가능하다.
7. 원하는 사운드 이펙트의 파라미터를 +- 버튼, 슬라이더, 편집 가능한 텍스트 등으로 조절한 뒤 APPLY 버튼을 클릭하여 적용할 수 있다.
8. 다른 사운드 이펙트를 5 번과 같은 방법으로 추가하거나 이미 적용된 이펙트를 REMOVE 버튼으로 제거할 수 있다.
9. APPLY를 통해 원하는 이펙트를 적용한 뒤 다시 PLAY 버튼을 클릭하여 변조된 사운드를 확인할 수 있다.
10. 원하는 사운드가 만들어지면 Content 폴더에 있는 test.uasset 오디오 wav파일을 자신의 프로젝트에 이주(Migrate)하여 사용할 수 있다.


## Audio Settings

### Audio Capture

Audio Capture Plugin을 통해 동작. 마이크 입력 관련 선택을 하는 UI 영역
* RT(RealTime) :  실시간 오디오 변조를 선택하는 버튼. 자신이 이미 원하는 이펙트 세팅이 있다면 에디터 내에서 이펙트 등록을 통해 실시간으로 결과를 확인할 수 있다. 
* OFF : 비실시간 오디오 변조를 선택하는 버튼. 클릭 시 오디오 녹음이 시작되며 텍스트가 ON으로 변경된다. ON 상태에서 다시 클릭 시 녹음 종료
* PLAY : OFF 버튼을 통해 오디오 비실시간 녹음을 완료했을 때 활성화 되는 버튼. 등록한 오디오 이펙트가 적용되어 오디오가 재생된다.

### Low Pass Filter

Audio Source Effect Preset 중 하나로 지정된 주파수 이하의 주파수 신호만 통과시키는 필터 효과 제공
* CutFrequency : 지정한 주파수 영역 이하의 주파수 신호만 통과
* QFilter(Q-Factor) : Resonant Filter에서 사용되는 값, CutFrequency의 레벨을 직접 지정. QFilter를 높게 설정하면 CutoffFrequency 부근이 증폭되는 결과 획득

### High Pass Filter

Audio Source Effect Preset 중 하나로 지정된 주파수 이상의 주파수 신호만 통과시키는 필터 효과 제공
* CutFrequency : 지정된 주파수 영역 이상의 주파수 신호만 통과
* QFilter(Q-Factor) : Resonant Filter에서 사용되는 값, CutFrequency의 레벨을 직접 지정. QFilter를 높게 설정하면 CutoffFrequency 부근이 증폭되는 결과 획득

### Phaser

Audio Source Effect Preset 중 하나로 신호의 위상차를 이용하여 소리의 위치를 변화시키는 효과 제공
* WetLevel : Phaser 효과의 강도를 조절. 높게 설정시 효과가 강해지고 낮게 설정하면 효과가 약해짐
* Frequency : Phaser 효과가 발생하는 주파수를 조절.
* Feedback : Phaser 효과가 발생하는 횟수를 조절. 높게 설정시 여러 번 발생하고 낮게 설정 시 빈도가 적게 발생
* LFO(Low Frequency Oscillator) Type : Phaser 효과가 발생하는 방식을 결정.

### Chorus

Audio Source Effect Preset 중 하나로 소리를 복제하여 약간의 위상차를 주어 멀리서 들리는 듯한 효과 제공
* Depth : 복제된 소리의 비율을 조절
* Frequency : 복제된 소리의 주파수를 조절
* Feedback : 복제된 소리를 다시 원래 신호에 되돌려주는 비율을 조절
* WetLevel : 복제된 소리의 볼륨의 조절
* DryLevel : 원본 소리의 볼륨을 조절
* Spread : 복제된 소리의 위치를 조절하여 높게 설정시 복제된 소리가 넓게 퍼져서 들리는 효과 제공

### BitCrusher 

Audio Source Effect Preset 중 하나로 디지털 오디오 신호의 비트 깊이와 샘플 속도를 조절하여 신호의 왜곡 효과 제공
Bit Depth와 Sample Rate가 낮을수록 오디오 신호의 품질이 저하되고 왜곡이 증가
* BitDepth :  오디오 신호의 비트 깊이를 조절
* SampleRate : 줄이고 싶은 Sample Rate을 조절



## 프로젝트 시연 영상

https://drive.google.com/file/d/1B-FX9Y2kpxbedrSVoZiE8qL9-igEHEIe/view?usp=sharing







