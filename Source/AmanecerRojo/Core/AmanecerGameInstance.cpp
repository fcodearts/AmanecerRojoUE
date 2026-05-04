#include "AmanecerGameInstance.h"
#include "Kismet/GameplayStatics.h"

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

UAmanecerGameInstance* UAmanecerGameInstance::Get(const UObject* WorldContext)
{
    if (!WorldContext) return nullptr;
    return Cast<UAmanecerGameInstance>(
        UGameplayStatics::GetGameInstance(WorldContext)
    );
}
