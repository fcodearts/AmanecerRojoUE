#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AmanecerGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChanged, int32);
DECLARE_MULTICAST_DELEGATE(FOnActoChanged);
DECLARE_MULTICAST_DELEGATE(FOnPlayerDefeated);

UCLASS()
class AMANECEROJO_API UAmanecerGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UAmanecerGameInstance();

    // --- Stats de Darrow ---
    UPROPERTY(BlueprintReadWrite, Category="Stats")
    int32 FuerzaRojo = 10;

    UPROPERTY(BlueprintReadWrite, Category="Stats")
    int32 TecnicaDorado = 10;

    UPROPERTY(BlueprintReadWrite, Category="Stats")
    int32 Prestigio = 0;

    UPROPERTY(BlueprintReadWrite, Category="Stats")
    int32 ConflictoInterno = 0;

    UPROPERTY(BlueprintReadWrite, Category="Stats")
    int32 HpActual = 100;

    UPROPERTY(BlueprintReadWrite, Category="Stats")
    int32 HpMaximo = 100;

    UPROPERTY(BlueprintReadWrite, Category="Stats")
    int32 ActoActual = 0;

    // --- Delegates (equivalente a señales de Godot) ---
    FOnStatChanged OnPrestigioCambiado;
    FOnStatChanged OnConflictoCambiado;
    FOnStatChanged OnVidaCambiada;
    FOnActoChanged OnActoCambiado;
    FOnPlayerDefeated OnJugadorDerrotado;

    // --- Métodos Blueprint-callable ---
    UFUNCTION(BlueprintCallable, Category="Stats")
    void ModificarPrestigio(int32 Cantidad);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void ModificarConflicto(int32 Cantidad);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void RecibirDanio(int32 Cantidad);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void Curar(int32 Cantidad);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void AvanzarActo();

    UFUNCTION(BlueprintPure, Category="Stats")
    bool TieneFuerzaRojo(int32 Umbral) const { return FuerzaRojo >= Umbral; }

    UFUNCTION(BlueprintPure, Category="Stats")
    bool TieneTecnicaDorado(int32 Umbral) const { return TecnicaDorado >= Umbral; }

    // --- Flags de diálogo (equivalente a DialogueManager._flags) ---
    UPROPERTY(BlueprintReadWrite, Category="Flags")
    TMap<FName, bool> Flags;

    UFUNCTION(BlueprintCallable, Category="Flags")
    void SetFlag(FName Flag, bool Valor) { Flags.Add(Flag, Valor); }

    UFUNCTION(BlueprintPure, Category="Flags")
    bool GetFlag(FName Flag) const { return Flags.Contains(Flag) && Flags[Flag]; }

    // --- Guardado / Carga ---
    UFUNCTION(BlueprintCallable, Category="Save")
    void Guardar();

    UFUNCTION(BlueprintCallable, Category="Save")
    bool Cargar();

    // --- Utilidad ---
    UFUNCTION(BlueprintPure, Category="Stats")
    static UAmanecerGameInstance* Get(const UObject* WorldContext);
};
