#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AmanecerGameMode.generated.h"

UCLASS()
class AMANECEROJO_API AAmanecerGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AAmanecerGameMode();

    virtual void BeginPlay() override;

    // Cargar el capítulo inicial al arrancar
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
    FString CapituloInicial = TEXT("capitulo_1_instituto.json");

private:
    void InicializarSistemas();
};
