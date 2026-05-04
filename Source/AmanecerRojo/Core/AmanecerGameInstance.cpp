#include "AmanecerGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Save/AmanecerSaveGame.h"

UAmanecerGameInstance::UAmanecerGameInstance() {}

void UAmanecerGameInstance::ModificarPrestigio(int32 Cantidad)
{
    Prestigio = FMath::Clamp(Prestigio + Cantidad, 0, 100);
    OnPrestigioCambiado.Broadcast(Prestigio);
}

void UAmanecerGameInstance::ModificarConflicto(int32 Cantidad)
{
    ConflictoInterno = FMath::Clamp(ConflictoInterno + Cantidad, 0, 100);
    OnConflictoCambiado.Broadcast(ConflictoInterno);
}

void UAmanecerGameInstance::RecibirDanio(int32 Cantidad)
{
    HpActual = FMath::Clamp(HpActual - Cantidad, 0, HpMaximo);
    OnVidaCambiada.Broadcast(HpActual);
    if (HpActual <= 0)
    {
        OnJugadorDerrotado.Broadcast();
    }
}

void UAmanecerGameInstance::Curar(int32 Cantidad)
{
    HpActual = FMath::Clamp(HpActual + Cantidad, 0, HpMaximo);
    OnVidaCambiada.Broadcast(HpActual);
}

void UAmanecerGameInstance::AvanzarActo()
{
    ActoActual++;
    OnActoCambiado.Broadcast();
}

void UAmanecerGameInstance::Guardar()
{
    auto* Save = Cast<UAmanecerSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UAmanecerSaveGame::StaticClass()));

    Save->FuerzaRojo       = FuerzaRojo;
    Save->TecnicaDorado    = TecnicaDorado;
    Save->Prestigio        = Prestigio;
    Save->ConflictoInterno = ConflictoInterno;
    Save->HpActual         = HpActual;
    Save->HpMaximo         = HpMaximo;
    Save->ActoActual       = ActoActual;
    Save->Flags            = Flags;

    UGameplayStatics::SaveGameToSlot(Save, UAmanecerSaveGame::SlotName, UAmanecerSaveGame::UserIndex);
    UE_LOG(LogTemp, Log, TEXT("GameInstance: partida guardada (Acto %d)"), ActoActual);
}

bool UAmanecerGameInstance::Cargar()
{
    if (!UGameplayStatics::DoesSaveGameExist(UAmanecerSaveGame::SlotName, UAmanecerSaveGame::UserIndex))
        return false;

    auto* Save = Cast<UAmanecerSaveGame>(
        UGameplayStatics::LoadGameFromSlot(UAmanecerSaveGame::SlotName, UAmanecerSaveGame::UserIndex));
    if (!Save) return false;

    FuerzaRojo       = Save->FuerzaRojo;
    TecnicaDorado    = Save->TecnicaDorado;
    Prestigio        = Save->Prestigio;
    ConflictoInterno = Save->ConflictoInterno;
    HpActual         = Save->HpActual;
    HpMaximo         = Save->HpMaximo;
    ActoActual       = Save->ActoActual;
    Flags            = Save->Flags;

    UE_LOG(LogTemp, Log, TEXT("GameInstance: partida cargada (Acto %d)"), ActoActual);
    return true;
}

UAmanecerGameInstance* UAmanecerGameInstance::Get(const UObject* WorldContext)
{
    if (!WorldContext) return nullptr;
    return Cast<UAmanecerGameInstance>(
        UGameplayStatics::GetGameInstance(WorldContext)
    );
}
