#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AmanecerPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class AMANECEROJO_API AAmanecerPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AAmanecerPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    // Enhanced Input — contexto y acciones
    UPROPERTY(EditDefaultsOnly, Category="Input")
    TObjectPtr<UInputMappingContext> ContextoMovimiento;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    TObjectPtr<UInputAction> AccionMover;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    TObjectPtr<UInputAction> AccionAtacar;

    UPROPERTY(EditDefaultsOnly, Category="Input")
    TObjectPtr<UInputAction> AccionInteractuar;

private:
    void OnMover(const FInputActionValue& Value);
    void OnAtacar(const FInputActionValue& Value);
    void OnInteractuar(const FInputActionValue& Value);
};
