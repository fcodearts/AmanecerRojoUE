#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UENUM(BlueprintType)
enum class ETipoAtaque : uint8
{
    Normal      UMETA(DisplayName="Normal"),
    Garza       UMETA(DisplayName="Forma de la Garza"),   // tecnica_dorado > 30
    Lykos       UMETA(DisplayName="Ira de Lykos"),        // fuerza_rojo > 30
};

USTRUCT(BlueprintType)
struct FResultadoCombate
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) int32 Danio = 0;
    UPROPERTY(BlueprintReadOnly) bool  bGolpeCritico = false;
    UPROPERTY(BlueprintReadOnly) bool  bRompeEscudo = false;
    UPROPERTY(BlueprintReadOnly) ETipoAtaque TipoUsado = ETipoAtaque::Normal;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGolpeConectado, const FResultadoCombate&);
DECLARE_MULTICAST_DELEGATE(FOnCombatienteDerotado);

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class AMANECEROJO_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCombatComponent();

    // Stats de combate del propietario (se asignan desde el GameInstance o enemigos.json)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    int32 FuerzaBase = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    int32 Defensa = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    bool bTieneEscudo = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    int32 HpActual = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    int32 HpMaximo = 100;

    // Delegates
    FOnGolpeConectado       OnGolpeConectado;
    FOnCombatienteDerotado  OnDerrotado;

    // Resolver un ataque contra un objetivo (duck-typing via componente)
    UFUNCTION(BlueprintCallable, Category="Combat")
    FResultadoCombate ResolverAtaque(ETipoAtaque Tipo, UCombatComponent* Objetivo);

    // Recibir daño desde fuera (enemigos, trampas, etc.)
    UFUNCTION(BlueprintCallable, Category="Combat")
    void RecibirDanio(int32 Cantidad, bool bIgnoraEscudo = false);

    UFUNCTION(BlueprintPure, Category="Combat")
    bool EstaVivo() const { return HpActual > 0; }

    UFUNCTION(BlueprintPure, Category="Combat")
    float PorcentajeVida() const { return HpMaximo > 0 ? (float)HpActual / HpMaximo : 0.f; }

private:
    int32 CalcularDanio(ETipoAtaque Tipo, int32 DefensaObjetivo) const;
};
