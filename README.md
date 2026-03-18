# ChatX

**Unreal Engine 5.5** 기반의 멀티플레이어 실시간 채팅 + 숫자야구 게임 프로젝트입니다.  
플레이어들이 채팅창을 통해 소통하면서, 동시에 숫자야구 게임을 함께 즐길 수 있습니다.

---

## 🎮 프로젝트 개요

ChatX는 Unreal Engine의 네트워크 복제(Replication) 및 RPC(Remote Procedure Call) 시스템을 학습하고 실습하기 위해 제작된 프로젝트입니다.  
멀티플레이어 환경에서 실시간 채팅과 게임 로직이 어떻게 동작하는지 직접 구현한 것이 핵심입니다.

---

## ✨ 주요 기능

### 💬 실시간 채팅
- 모든 플레이어가 같은 채팅창을 통해 실시간으로 메시지를 주고받을 수 있습니다.
- 플레이어 접속 시 입장 메시지가 전체에게 브로드캐스트됩니다.
- 채팅 메시지에 플레이어 이름과 현재 추측 횟수가 함께 표시됩니다 (예: `Player1(1/3): 123`).

### 🔢 숫자야구 게임
- 서버가 자동으로 **1~9 사이의 중복 없는 3자리 숫자**를 생성합니다.
- 플레이어가 채팅창에 3자리 숫자를 입력하면 자동으로 게임 입력으로 인식합니다.
- 결과는 **Strike / Ball / Out** 형식으로 전체 채팅에 출력됩니다.
  - 예: `Player1(1/3): 123 -> 1S2B`
- 3 Strike 달성 시 해당 플레이어의 승리를 전체에 알리고 게임이 초기화됩니다.
- 모든 플레이어가 최대 추측 횟수(기본 3회)를 소진하면 무승부 처리됩니다.

### 🌐 네트워크 기능
- Server RPC / Client RPC / Multicast RPC를 활용한 메시지 동기화
- `NotificationText` 복제를 통한 알림 UI 실시간 업데이트
- `PlayerState`를 통한 플레이어 이름, 현재 추측 횟수, 최대 추측 횟수 복제

---

## 🏗️ 프로젝트 구조

```
Source/ChatX/
├── ChatX.h / .cpp              # 유틸리티 함수 라이브러리 (디버그 출력, 네트 모드/롤 확인)
├── Game/
│   ├── CXGameModeBase          # 게임 핵심 로직 (숫자 생성, 판정, 승패 처리)
│   └── CXGameStateBase         # 멀티캐스트 RPC를 통한 입장 메시지 브로드캐스트
├── Player/
│   ├── CXPawn                  # 플레이어 폰 (네트 모드/롤 로그 출력)
│   ├── CXPlayerController      # 채팅 입력 처리 및 Server/Client RPC 호출
│   └── CXPlayerState           # 플레이어 이름, 추측 횟수 복제
└── UI/
    └── CXChatInput             # 채팅 입력 UMG 위젯 (Enter 키 입력 처리)
```

---

## 🔧 개발 환경

| 항목 | 내용 |
|------|------|
| 엔진 | Unreal Engine 5.5 |
| 언어 | C++ |
| UI | UMG (Unreal Motion Graphics) |
| 네트워크 | Unreal 내장 네트워크 (Listen Server 기반) |
| 빌드 설정 | BuildSettingsVersion.V5 |

**모듈 의존성:**
- `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`
- `UMG`, `Slate`, `SlateCore`

---

## 🚀 실행 방법

### 요구 사항
- Unreal Engine 5.5 이상 설치
- Visual Studio 2022 (Windows) 또는 Xcode (Mac)

### 빌드 및 실행

1. 레포지토리를 클론합니다.
   ```bash
   git clone https://github.com/saltlake00/ChatX.git
   ```

2. `ChatX.uproject` 파일을 우클릭 → **Generate Visual Studio project files** 선택

3. 생성된 `.sln` 파일을 Visual Studio에서 열고 빌드합니다.

4. Unreal Editor에서 프로젝트를 열어 실행합니다.

### 멀티플레이어 테스트

Unreal Editor 상단 플레이 버튼 옆 설정에서:
- **Number of Players** 를 2 이상으로 설정
- **Net Mode** 를 `Play As Listen Server` 로 설정 후 실행

---

## 🎯 게임 플레이 방법

1. 게임 시작 시 서버가 자동으로 3자리 비밀 숫자를 생성합니다.
2. 채팅창에 메시지를 입력하고 **Enter** 를 누릅니다.
3. 입력한 메시지의 **마지막 3자리가 유효한 숫자** (1~9, 중복 없음)이면 자동으로 숫자야구 입력으로 처리됩니다.
4. 그 외 메시지는 일반 채팅으로 전송됩니다.
5. 3 Strike를 달성하면 승리! 게임이 자동으로 리셋됩니다.

---

## 📖 학습 포인트

- Unreal Engine의 **Server RPC / Client RPC / NetMulticast RPC** 구현
- **`GetLifetimeReplicatedProps`** 를 통한 변수 복제
- **UMG 위젯**과 `PlayerController`의 연동
- `GameMode` / `GameState` / `PlayerState` / `PlayerController` 역할 분리
- Listen Server 환경에서의 네트워크 권한(Authority) 처리

---

## 구현 과정

### 1. 프로젝트 설정
`ChatX.Build.cs`에서 UMG, Slate 모듈을 추가해 채팅 위젯을 C++에서 사용할 수 있도록 구성했습니다.

### 2. 유틸리티 라이브러리 (`ChatXFunctionLibrary`)
네트 모드에 따라 디버그 메시지 출력 방식을 자동 분기하는 정적 유틸리티 클래스를 구현했습니다. Client / Listen Server에서는 화면 출력, Dedicated Server에서는 로그로 출력됩니다.

### 3. PlayerState — 플레이어 정보 복제
플레이어 이름(`PlayerNameString`), 현재 추측 횟수(`CurrentGuessCount`), 최대 추측 횟수(`MaxGuessCount`)를 `DOREPLIFETIME`으로 전체 클라이언트에 복제했습니다. 채팅 메시지에 `Player1(1/3)` 형태로 함께 표시됩니다.

### 4. PlayerController — 채팅 입출력
- `BeginPlay`에서 로컬 컨트롤러에만 채팅 위젯을 생성해 뷰포트에 추가합니다.
- 메시지 입력 시 PlayerState에서 플레이어 정보를 조합한 뒤 **Server RPC**로 서버에 전달합니다.
- 서버는 처리 결과를 **Client RPC**로 각 클라이언트에 전달해 화면에 출력합니다.
- `NotificationText`를 `Replicated`로 선언해 승리/무승부 알림을 UI에 자동 반영합니다.

### 5. GameModeBase — 핵심 게임 로직
서버 전용 클래스로 모든 게임 로직을 담당합니다.

- **`OnPostLogin`** — 플레이어 접속 시 이름 부여, GameState를 통해 입장 메시지 브로드캐스트
- **`GenerateSecretNumber`** — 1~9 배열에서 3개를 무작위 추출해 중복 없는 비밀 숫자 생성
- **`IsGuessNumberString`** — 길이 3, 숫자만 포함, 0 없음, 중복 없음 조건으로 유효성 검사
- **`JudgeResult`** — 자리·숫자 비교로 Strike / Ball / OUT 판정
- **`PrintChatMessageString`** — 입력 끝 3자리를 추출해 숫자야구 입력 여부를 판단하고 전체에 결과 전파
- **`JudgeGame`** / **`ResetGame`** — 승패 판정 후 비밀 숫자와 추측 횟수 초기화

### 6. GameStateBase — 입장 메시지 브로드캐스트
GameMode는 서버에만 존재하므로, 전체 클라이언트에 메시지를 보내기 위해 GameState의 **NetMulticast RPC**를 사용했습니다. `HasAuthority()` 체크로 클라이언트에서만 UI에 출력합니다.

### 7. 채팅 위젯 (`CXChatInput`)
`BindWidget` 매크로로 UMG의 `EditableTextBox`와 C++ 변수를 연결했습니다. `OnTextCommitted` 이벤트에서 Enter 입력을 감지해 PlayerController로 메시지를 전달하고 입력창을 초기화합니다. `NativeDestruct`에서 `IsAlreadyBound` 체크 후 이벤트를 해제해 메모리 안전성을 확보했습니다.

---
## 📄 라이선스

이 프로젝트는 학습 목적으로 제작되었습니다.  
일부 파일에 Epic Games의 Copyright 헤더가 포함되어 있으며, Unreal Engine 라이선스 정책을 따릅니다.
