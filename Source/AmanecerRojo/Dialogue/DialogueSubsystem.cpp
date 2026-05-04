#include "DialogueSubsystem.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Core/AmanecerGameInstance.h"

bool UDialogueSubsystem::CargarCapitulo(const FString& NombreArchivo)
{
    Escenas.Empty();

    FString Ruta = FPaths::ProjectContentDir() / TEXT("Data/Dialogos") / NombreArchivo;
    FString JsonStr;

    if (!FFileHelper::LoadFileToString(JsonStr, *Ruta))
    {
        UE_LOG(LogTemp, Error, TEXT("DialogueSubsystem: No se pudo cargar %s"), *Ruta);
        return false;
    }

    TArray<TSharedPtr<FJsonValue>> Root;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonStr);

    if (!FJsonSerializer::Deserialize(Reader, Root))
    {
        UE_LOG(LogTemp, Error, TEXT("DialogueSubsystem: JSON inválido en %s"), *NombreArchivo);
        return false;
    }

    for (const auto& Val : Root)
    {
        FDialogueEscena Escena = ParsearEscena(Val->AsObject());
        Escenas.Add(Escena.Id, Escena);
    }

    UE_LOG(LogTemp, Log, TEXT("DialogueSubsystem: %d escenas cargadas desde %s"), Escenas.Num(), *NombreArchivo);
    return true;
}

void UDialogueSubsystem::IniciarDesde(FName IdEscena)
{
    bActivo = true;
    MostrarEscena(IdEscena);
}

void UDialogueSubsystem::MostrarEscena(FName Id)
{
    if (!Escenas.Contains(Id))
    {
        bActivo = false;
        OnDialogoTerminado.Broadcast();
        return;
    }

    EscenaActual = Escenas[Id];
    OnEscenaMostrada.Broadcast(EscenaActual);

    // Filtrar opciones según condiciones
    TArray<FDialogueOpcion> OpcionesValidas;
    for (const auto& Op : EscenaActual.Opciones)
    {
        if (EvaluarCondicion(Op))
            OpcionesValidas.Add(Op);
    }

    if (OpcionesValidas.Num() > 0)
        OnOpcionesDisponibles.Broadcast(OpcionesValidas);
}

void UDialogueSubsystem::ElegirOpcion(int32 Indice)
{
    TArray<FDialogueOpcion> OpcionesValidas;
    for (const auto& Op : EscenaActual.Opciones)
        if (EvaluarCondicion(Op)) OpcionesValidas.Add(Op);

    if (!OpcionesValidas.IsValidIndex(Indice)) return;

    AplicarEfectos(EscenaActual);
    MostrarEscena(OpcionesValidas[Indice].SiguienteId);
}

void UDialogueSubsystem::Avanzar()
{
    AplicarEfectos(EscenaActual);

    if (EscenaActual.SiguienteId.IsNone())
    {
        bActivo = false;
        OnDialogoTerminado.Broadcast();
        return;
    }
    MostrarEscena(EscenaActual.SiguienteId);
}

void UDialogueSubsystem::AplicarEfectos(const FDialogueEscena& Escena)
{
    auto* GI = UAmanecerGameInstance::Get(this);
    if (!GI) return;

    if (!Escena.SetFlag.IsNone())
        GI->SetFlag(Escena.SetFlag, true);

    if (!Escena.ModStat.IsNone() && Escena.ModValor != 0)
    {
        if      (Escena.ModStat == "prestigio")        GI->ModificarPrestigio(Escena.ModValor);
        else if (Escena.ModStat == "conflicto_interno") GI->ModificarConflicto(Escena.ModValor);
        else if (Escena.ModStat == "fuerza_rojo")      GI->FuerzaRojo   = FMath::Clamp(GI->FuerzaRojo   + Escena.ModValor, 0, 100);
        else if (Escena.ModStat == "tecnica_dorado")   GI->TecnicaDorado = FMath::Clamp(GI->TecnicaDorado + Escena.ModValor, 0, 100);
    }
}

bool UDialogueSubsystem::EvaluarCondicion(const FDialogueOpcion& Opcion) const
{
    auto* GI = UAmanecerGameInstance::Get(this);
    if (!GI) return true;

    if (!Opcion.FlagRequerido.IsNone() && !GI->GetFlag(Opcion.FlagRequerido))
        return false;

    if (!Opcion.StatRequerido.IsNone())
    {
        int32 Valor = 0;
        if      (Opcion.StatRequerido == "fuerza_rojo")      Valor = GI->FuerzaRojo;
        else if (Opcion.StatRequerido == "tecnica_dorado")    Valor = GI->TecnicaDorado;
        else if (Opcion.StatRequerido == "prestigio")         Valor = GI->Prestigio;
        else if (Opcion.StatRequerido == "conflicto_interno") Valor = GI->ConflictoInterno;

        if (Valor < Opcion.ValorRequerido) return false;
    }

    return true;
}

FDialogueEscena UDialogueSubsystem::ParsearEscena(const TSharedPtr<FJsonObject>& Json) const
{
    FDialogueEscena Escena;
    Escena.Id         = FName(*Json->GetStringField(TEXT("id")));
    Escena.Personaje  = FText::FromString(Json->GetStringField(TEXT("personaje")));
    Escena.Texto      = FText::FromString(Json->GetStringField(TEXT("texto")));
    Escena.SiguienteId = FName(*Json->GetStringField(TEXT("siguiente")));

    FString Flag, Stat;
    if (Json->TryGetStringField(TEXT("set_flag"), Flag))   Escena.SetFlag = FName(*Flag);
    if (Json->TryGetStringField(TEXT("mod_stat"), Stat))   Escena.ModStat = FName(*Stat);
    Escena.ModValor = (int32)Json->GetNumberField(TEXT("mod_valor"));

    const TArray<TSharedPtr<FJsonValue>>* Opts;
    if (Json->TryGetArrayField(TEXT("opciones"), Opts))
    {
        for (const auto& OptVal : *Opts)
        {
            auto OptObj = OptVal->AsObject();
            FDialogueOpcion Op;
            Op.Texto        = FText::FromString(OptObj->GetStringField(TEXT("texto")));
            Op.SiguienteId  = FName(*OptObj->GetStringField(TEXT("siguiente")));
            FString FR, SR;
            if (OptObj->TryGetStringField(TEXT("requiere_flag"), FR)) Op.FlagRequerido = FName(*FR);
            if (OptObj->TryGetStringField(TEXT("requiere_stat"), SR)) Op.StatRequerido = FName(*SR);
            Op.ValorRequerido = (int32)OptObj->GetNumberField(TEXT("valor_requerido"));
            Escena.Opciones.Add(Op);
        }
    }

    return Escena;
}
