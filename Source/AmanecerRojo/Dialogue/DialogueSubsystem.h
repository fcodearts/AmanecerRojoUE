#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Dom/JsonObject.h"
#include "DialogueSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FDialogueOpcion
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FText Texto;
    UPROPERTY(BlueprintReadOnly) FName SiguienteId;
    UPROPERTY(BlueprintReadOnly) FName FlagRequerido;
    UPROPERTY(BlueprintReadOnly) FName StatRequerido;
    UPROPERTY(BlueprintReadOnly) int32 ValorRequerido = 0;
};

USTRUCT(BlueprintType)
struct FDialogueEscena
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FName Id;
    UPROPERTY(BlueprintReadOnly) FText Personaje;
    UPROPERTY(BlueprintReadOnly) FText Texto;
    UPROPERTY(BlueprintReadOnly) TArray<FDialogueOpcion> Opciones;
    UPROPERTY(BlueprintReadOnly) FName SiguienteId;
    UPROPERTY(BlueprintReadOnly) FName SetFlag;
    UPROPERTY(BlueprintReadOnly) FName ModStat;
    UPROPERTY(BlueprintReadOnly) int32 ModValor = 0;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEscenaMostrada, const FDialogueEscena&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOpcionesDisponibles, const TArray<FDialogueOpcion>&);
DECLARE_MULTICAST_DELEGATE(FOnDialogoTerminado);

UCLASS()
class AMANECEROJO_API UDialogueSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Delegates (equivalente a señales de Godot)
    FOnEscenaMostrada    OnEscenaMostrada;
    FOnOpcionesDisponibles OnOpcionesDisponibles;
    FOnDialogoTerminado  OnDialogoTerminado;

    // Cargar capítulo desde JSON en Content/Data/Dialogos/
    UFUNCTION(BlueprintCallable, Category="Dialogue")
    bool CargarCapitulo(const FString& NombreArchivo);

    // Iniciar diálogo desde una escena concreta
    UFUNCTION(BlueprintCallable, Category="Dialogue")
    void IniciarDesde(FName IdEscena);

    // El jugador elige una opción (índice)
    UFUNCTION(BlueprintCallable, Category="Dialogue")
    void ElegirOpcion(int32 Indice);

    // Avanzar al siguiente (sin opciones)
    UFUNCTION(BlueprintCallable, Category="Dialogue")
    void Avanzar();

    UFUNCTION(BlueprintPure, Category="Dialogue")
    bool EstaActivo() const { return bActivo; }

private:
    TMap<FName, FDialogueEscena> Escenas;
    FDialogueEscena EscenaActual;
    bool bActivo = false;

    void MostrarEscena(FName Id);
    void AplicarEfectos(const FDialogueEscena& Escena);
    bool EvaluarCondicion(const FDialogueOpcion& Opcion) const;
    FDialogueEscena ParsearEscena(const TSharedPtr<FJsonObject>& Json) const;
};
