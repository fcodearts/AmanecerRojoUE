#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AmanecerSaveGame.generated.h"

UCLASS()
class AMANECEROJO_API UAmanecerSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    // Stats
    UPROPERTY() int32 FuerzaRojo = 10;
    UPROPERTY() int32 TecnicaDorado = 10;
    UPROPERTY() int32 Prestigio = 0;
    UPROPERTY() int32 ConflictoInterno = 0;
    UPROPERTY() int32 HpActual = 100;
    UPROPERTY() int32 HpMaximo = 100;
    UPROPERTY() int32 ActoActual = 0;

    // Flags de diálogo
    UPROPERTY() TMap<FName, bool> Flags;

    // Escena activa al guardar
    UPROPERTY() FString EscenaActual;

    static const FString SlotName;
    static const int32   UserIndex;
};
