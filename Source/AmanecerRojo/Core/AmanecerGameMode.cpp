#include "AmanecerGameMode.h"
#include "AmanecerGameInstance.h"
#include "Dialogue/DialogueSubsystem.h"
#include "Kismet/GameplayStatics.h"

AAmanecerGameMode::AAmanecerGameMode()
{
    // GameInstance y PlayerController se asignan en Project Settings
}

void AAmanecerGameMode::BeginPlay()
{
    Super::BeginPlay();
    InicializarSistemas();
}

void AAmanecerGameMode::InicializarSistemas()
{
    auto* GI = UAmanecerGameInstance::Get(this);
    if (GI)
    {
        // Intentar cargar partida guardada; si no existe, empezamos desde cero
        if (!GI->Cargar())
            UE_LOG(LogTemp, Log, TEXT("GameMode: Sin partida guardada — inicio nuevo"));
    }

    auto* Dialogue = GetGameInstance()->GetSubsystem<UDialogueSubsystem>();
    if (Dialogue && !CapituloInicial.IsEmpty())
    {
        Dialogue->CargarCapitulo(CapituloInicial);
        UE_LOG(LogTemp, Log, TEXT("GameMode: Capítulo '%s' cargado"), *CapituloInicial);
    }
}
