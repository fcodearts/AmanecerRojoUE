#include "CombatComponent.h"
#include "Core/AmanecerGameInstance.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

FResultadoCombate UCombatComponent::ResolverAtaque(ETipoAtaque Tipo, UCombatComponent* Objetivo)
{
    FResultadoCombate Resultado;
    if (!Objetivo || !Objetivo->EstaVivo()) return Resultado;

    Resultado.TipoUsado  = Tipo;
    Resultado.Danio      = CalcularDanio(Tipo, Objetivo->Defensa);
    Resultado.bRompeEscudo = (Tipo == ETipoAtaque::Lykos);

    // La Garza desactiva el escudo; Lykos lo destruye completamente
    if (Tipo == ETipoAtaque::Garza && Objetivo->bTieneEscudo)
    {
        Objetivo->bTieneEscudo = false;
        Resultado.bGolpeCritico = true;
    }

    Objetivo->RecibirDanio(Resultado.Danio, Resultado.bRompeEscudo);

    OnGolpeConectado.Broadcast(Resultado);
    return Resultado;
}

void UCombatComponent::RecibirDanio(int32 Cantidad, bool bIgnoraEscudo)
{
    if (bTieneEscudo && !bIgnoraEscudo)
    {
        // El escudo absorbe el daño
        bTieneEscudo = false;
        return;
    }

    HpActual = FMath::Clamp(HpActual - Cantidad, 0, HpMaximo);

    // Si el propietario es el jugador, sincronizar con GameInstance
    auto* GI = UAmanecerGameInstance::Get(this);
    if (GI && GetOwner() && GetOwner()->ActorHasTag("Player"))
    {
        GI->HpActual = HpActual;
        GI->OnVidaCambiada.Broadcast(HpActual);
    }

    if (HpActual <= 0)
    {
        OnDerrotado.Broadcast();
        if (GI && GetOwner() && GetOwner()->ActorHasTag("Player"))
            GI->OnJugadorDerrotado.Broadcast();
    }
}

int32 UCombatComponent::CalcularDanio(ETipoAtaque Tipo, int32 DefensaObjetivo) const
{
    // Fórmulas del ADR-001 de Godot — mantenemos la misma lógica
    float Multiplicador = 1.0f;
    switch (Tipo)
    {
        case ETipoAtaque::Garza: Multiplicador = 1.5f; break;
        case ETipoAtaque::Lykos: Multiplicador = 2.0f; break;
        default: break;
    }

    int32 DanioBase = FMath::Max(1, FuerzaBase - DefensaObjetivo / 2);
    return FMath::RoundToInt(DanioBase * Multiplicador);
}
